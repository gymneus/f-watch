#include <stdio.h>
#include <unistd.h>
#include <libusb.h>

#define LED_SET  0x11

int main()
{
        int i = 0;
        int led = 12;
        int vals[4] = {0, 1, 1, 0};
        int val;

        libusb_device **devs, *dev;
        libusb_device_handle *devh = NULL;

        libusb_init(NULL);

        libusb_get_device_list(NULL, &devs);

        while ((dev = devs[i++]) != NULL) {
                struct libusb_device_descriptor desc;
                int r = libusb_get_device_descriptor(dev, &desc);
                if (r < 0) {
                        fprintf(stderr, "failed to get device descriptor");
                        break;
                }
                if ((desc.idVendor == 0x10c4) && (desc.idProduct == 0x00ff)) {
                        r = libusb_open(dev, &devh);
                        if (r < 0) {
                                fprintf(stderr, "failed to open device: %d\n", r);
                                goto cleanup;
                        }
                        printf("device open\n");
                        r = libusb_claim_interface(devh, 0);
                        if (r < 0) {
                                fprintf(stderr, "failed to claim interface: %d\n", r);
                                goto cleanup;
                        }
                        printf("interface claimed\n");
                        break;
                }
        }

        if (devh != NULL) {
                for (i = 0; i < 10; i++) {

                        if (led == 11) led = 12;
                        else led = 11;
                        val = vals[i%4];

                        printf("%d:%d:%d\n", i, led, val);
                        libusb_control_transfer (
                                devh,                     // Device handle
                                LIBUSB_ENDPOINT_OUT        |
                                LIBUSB_REQUEST_TYPE_VENDOR |
                                LIBUSB_RECIPIENT_DEVICE,  // bmRequestType
                                LED_SET,                  // bRequest
                                val,                      // wValue
                                led,                      // wIndex
                                NULL,                     // char *data
                                0,                        // wLength
                                1000 );                   // timeout (ms)
                        sleep(1);
                }
        }

cleanup:
        printf("cleanup\n");
        if (devh != NULL) {
                printf("release\n");
                libusb_release_interface(devh, 0);
                libusb_close(devh);
        }
        printf("free\n");
        libusb_free_device_list(devs, 1);
        printf("exit\n");
        libusb_exit(NULL);
        printf("done\n");

        return 0;
}
