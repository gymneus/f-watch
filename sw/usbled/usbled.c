#include <stdio.h>
#include <usb.h>

#define LED_SET  0x11

int main()
{
  static usb_dev_handle *pDevH = NULL;  // USB device handle
  static struct usb_device *pDev;
  static struct usb_bus *pBus;

  int led = 12;
  int val = 0;

  usb_init();

  usb_find_busses();
  usb_find_devices();

  // Enumerate USB devices ...
  for ( pBus = usb_get_busses(); pBus; pBus = pBus->next)
  {
    for ( pDev = pBus->devices; pDev; pDev = pDev->next)
    {
      if ( ( pDev->descriptor.idVendor  == 0x10C4 ) &&
           ( pDev->descriptor.idProduct == 0x00ff ) )
      {
        pDevH = usb_open( pDev );
        if ( pDevH )
        {
          if ( usb_set_configuration(
                    pDevH, pDev->config->bConfigurationValue ) != 0 )
          {
            usb_close( pDevH );
            pDevH = NULL;
            printf("FAIL: usb_set_configuration");
          }

          if ( pDev->descriptor.idProduct == 0x00ff )
          {
            if ( usb_claim_interface( pDevH, 0 ) != 0 )
            {
              usb_close( pDevH );
              pDevH = NULL;
              printf("FAIL: usb_claim_interface");
            }
          }
        }
      }
    }
  }

  while (1) {

        if (led == 11) led = 12;
        if (led == 12) led = 11;
        val ^= 1;

    if ( pDevH )
    {
      printf("led%d\n", led);
      usb_control_msg(
                    pDevH,            // Device handle
                    USB_ENDPOINT_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
                                      // bmRequestType
                    LED_SET,          // bRequest
                    val,              // wValue
                    led,              // wIndex
                    NULL,             // char *pBuffer
                    1,                // wLength
                    1000 );           // Timeout (ms)
      sleep(1);
    }

  }
}
