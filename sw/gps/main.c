/*
 *==============================================================================
 * CERN (BE-CO-HT)
 * Source file for Main
 *==============================================================================
 *
 * author: Theodor Stana (t.stana@cern.ch)
 *
 * date of creation: 2014-08-14
 *
 * version: 1.0
 *
 * description:
 *
 * dependencies:
 *
 * references:
 *
 *==============================================================================
 * GNU LESSER GENERAL PUBLIC LICENSE
 *==============================================================================
 * This source file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version. This source is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details. You should have
 * received a copy of the GNU Lesser General Public License along with this
 * source; if not, download it from http://www.gnu.org/licenses/lgpl-2.1.html
 *==============================================================================
 * last changes:
 *    2014-08-14   Theodor Stana     File created
 *==============================================================================
 * TODO: -
 *==============================================================================
 */

#include <string.h>
#include <stdio.h>

#include "em_device.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_leuart.h"
#include "em_gpio.h"

#include "usbdbg.h"


int main()
{
        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        usbdbg_init();
        gps_init();
        usbdbg_puts("init done!\r\n");

        int i, one = 1;
        static char tmp[64];
        static char c[2];
        static char cmd[64];

        for (;;) {
                GPIO_PinInGet(gpioPortA, 1) ?
                        GPIO_PinOutSet(gpioPortE, 11) :
                        GPIO_PinOutClear(gpioPortE, 11);

                LEUART_Tx(LEUART0, 0x54);
//                if (one) {
//                        one = 0;
//                        for (i = 0; i < 20000000; i++)
//                                ;
////                        usbdbg_puts("\r\n\r\n\r\n\r\n"); //strcpy(tmp, "$PSRF105,1*");
////                        sprintf(c, "%02X", nmea_crc(tmp));
////                        strcat(tmp, c);
////                        strcat(tmp, "\r\n");
////                        usbdbg_puts(tmp);
////                        gps_puts(tmp);
////                        usbdbg_puts("\r\n\r\n\r\n\r\n");
//                        /* Send init command */
////                        strcpy(tmp, "$PSRF104,46.22954,6.06156,100,0,55020,1807,12,1*");
//                        strcpy(tmp, "$PSRF125*");
//                        sprintf(c, "%02X", nmea_crc(tmp));
//                        strcpy(cmd, tmp);
//                        strcat(cmd, c);
//                        strcat(cmd, "\r\n");
//                        if (gps_puts(cmd)) {
//                                usbdbg_puts("\r\n\r\n\r\n\r\n");
//                                usbdbg_puts(cmd);
//                                usbdbg_puts("\r\n\r\n\r\n\r\n");
//                        }
//                }
        }

        return 0;
}

