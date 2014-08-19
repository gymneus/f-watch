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

#include "em_device.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_leuart.h"
#include "em_gpio.h"

#include "usbdesc.h"

#define RXBUFSIZE 8
volatile char rxbuf[RXBUFSIZE] = {'0', '1', '2', '3', '4', '5', '6', '7'};
volatile char idx = 0;

static void gps_init();
static void gps_on_off_pulse();

void LEUART0_IRQHandler()
{
        if (LEUART0->IF & LEUART_IF_RXDATAV) {
                rxbuf[idx++] = LEUART_Rx(LEUART0);
                if (idx == RXBUFSIZE) {
                        idx = 0;
                        USBD_Write(USBDESC_EP_DATA_OUT, rxbuf, RXBUFSIZE, NULL);
                }
        }
}

int main()
{
        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        USBD_Init(&initstruct);

        gps_init();

        USBD_Write(USBDESC_EP_DATA_OUT, "init done!\r\n", 12, NULL);

        int i;

        for (;;) {
                for (i = 0; i < 3000000; i++) {
                        GPIO_PinInGet(gpioPortA, 1) ?
                                GPIO_PinOutSet(gpioPortE, 11) :
                                GPIO_PinOutClear(gpioPortE, 11);
                }
                USBD_Write(USBDESC_EP_DATA_OUT, "\r\nOFF\r\n", 7, NULL);
                gps_on_off_pulse();


                for (i = 0; i < 3000000; i++) {
                        GPIO_PinInGet(gpioPortA, 1) ?
                                GPIO_PinOutSet(gpioPortE, 11) :
                                GPIO_PinOutClear(gpioPortE, 11);
                }
                USBD_Write(USBDESC_EP_DATA_OUT, "\r\nON\r\n", 6, NULL);
                gps_on_off_pulse();

        }

        return 0;
}

static void gps_init()
{
        /* GPS reset pin */
        USB->ROUTE &= ~(USB_ROUTE_VBUSENPEN);
        GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1);

        /* GPS WAKUP pin */
        GPIO_PinModeSet(gpioPortA, 1, gpioModeInput, 0);

        /* ON_OFF _double_ pulse */
        GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0);
        gps_on_off_pulse();
        gps_on_off_pulse();

        /* LEUART0 config */
        LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

        /* To avoid false start, configure output as high */
        GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 1);
        GPIO_PinModeSet(gpioPortE, 15, gpioModeInput, 0);

        CMU_ClockEnable(cmuClock_CORELE, true);
        CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
        CMU_ClockEnable(cmuClock_LEUART0, true);
        CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);

        init.enable = leuartDisable;
        init.baudrate = 4800;
        init.databits = leuartDatabits8;
        init.stopbits = leuartStopbits1;

        LEUART_Init(LEUART0, &init);

        LEUART0->ROUTE = LEUART_ROUTE_RXPEN |
                         LEUART_ROUTE_TXPEN |
                         LEUART_ROUTE_LOCATION_LOC2;

        LEUART_IntClear(LEUART0, LEUART_IF_RXDATAV);
        NVIC_ClearPendingIRQ(LEUART0_IRQn);

        LEUART_IntEnable(LEUART0, LEUART_IF_RXDATAV);
        NVIC_EnableIRQ(LEUART0_IRQn);

        LEUART_Enable(LEUART0, leuartEnable);

}

static void gps_on_off_pulse()
{
        int i;
        /* Pulse */
        GPIO_PinOutSet(gpioPortE, 13);
        GPIO_PinOutSet(gpioPortE, 12);
        for (i = 0; i < 100000; i++)
                ;
        GPIO_PinOutClear(gpioPortE, 13);
        GPIO_PinOutClear(gpioPortE, 12);
        /* Delay to make sure GPS module sees this as a pulse */
        for (i = 0; i < 100000; i++)
                ;
}
