#include "usbdesc.h"

/* The serial port LINE CODING data structure, used to carry information  */
/* about serial port baudrate, parity etc. between host and device.       */
EFM32_PACK_START(1)
struct __attribute__ ((packed)) cdc_line_coding {
  uint32_t dwDTERate;               /** Baudrate                            */
  uint8_t  bCharFormat;             /** Stop bits, 0=1 1=1.5 2=2            */
  uint8_t  bParityType;             /** 0=None 1=Odd 2=Even 3=Mark 4=Space  */
  uint8_t  bDataBits;               /** 5, 6, 7, 8 or 16                    */
  uint8_t  dummy;                   /** To ensure size is a multiple of 4 bytes.*/
};
EFM32_PACK_END()

/*** Variables ***/

/*
 * The LineCoding variable must be 4-byte aligned as it is used as USB
 * transmit and receive buffer
 */
EFM32_ALIGN(4)
EFM32_PACK_START(1)
static struct cdc_line_coding __attribute__ ((aligned(4))) line_coding =
{
        4800, 2, 0, 8, 0
};
EFM32_PACK_END()

/**************************************************************************//**
 * @brief
 *   Handle USB setup commands. Implements CDC class specific commands.
 *
 * @param[in] setup Pointer to the setup packet received.
 *
 * @return USB_STATUS_OK if command accepted.
 *         USB_STATUS_REQ_UNHANDLED when command is unknown, the USB device
 *         stack will handle the request.
 *****************************************************************************/
int usbdesc_cb_setup(const USB_Setup_TypeDef *setup)
{
        int r = USB_STATUS_REQ_UNHANDLED;

        if ((setup->Type == USB_SETUP_TYPE_CLASS) &&
                (setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE)) {
                switch (setup->bRequest)
                {
                case USB_CDC_GETLINECODING:
                        /********************/
                        if ((setup->wValue == 0) &&
                          (setup->wIndex == 0) &&
                          (setup->wLength == 7) &&
                          (setup->Direction == USB_SETUP_DIR_IN)) {
                                /* Send current settings to USB host. */
                                USBD_Write(0, (void*) &line_coding, 7, NULL);
                                r = USB_STATUS_OK;
                        }
                        break;

                case USB_CDC_SETLINECODING:
                        /********************/
                        if ((setup->wValue == 0) &&
                          (setup->wIndex == 0) &&
                          (setup->wLength == 7) &&
                          (setup->Direction != USB_SETUP_DIR_IN)) {
                                /* Get new settings from USB host. */
                                USBD_Read(0, (void*) &line_coding, 7, NULL);
                                r = USB_STATUS_OK;
                        }
                        break;

                case USB_CDC_SETCTRLLINESTATE:
                        /********************/
                        if ((setup->wIndex == 0) &&
                          (setup->wLength == 0)) {
                                /* Do nothing ( Non compliant behaviour !! ) */
                                r = USB_STATUS_OK;
                        }
                        break;
                }
        }

        return r;
}

