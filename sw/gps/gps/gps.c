/*
 *==============================================================================
 * CERN (BE-CO-HT)
 * Source file for Antenova M10478 GPS module
 *==============================================================================
 *
 * author: Theodor Stana (t.stana@cern.ch)
 *
 * date of creation: 2014-08-24
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
 *    2014-08-24   Theodor Stana     t.stana@cern.ch     File created
 *==============================================================================
 * TODO: -
 *==============================================================================
 */
#include <stdio.h>
#include <string.h>

#include <em_device.h>
#include <em_cmu.h>
#include <em_leuart.h>
#include <em_gpio.h>

#include "usbdbg.h"

#include "gps.h"
#include "nmea/nmea.h"

#define GPS_OK_TO_SEND "$PSRF150,1*3E\r\n"

#define RXBUFSIZE 128
static char rxbuf[RXBUFSIZE];
volatile int idx = 0;

static nmeaINFO gps_info;
static nmeaPARSER gps_parser;

void LEUART0_IRQHandler()
{
        if (LEUART0->IF & LEUART_IF_RXDATAV) {
                rxbuf[idx++] = LEUART_Rx(LEUART0);
                if ((rxbuf[idx-2] == '\r') && (rxbuf[idx-1] == '\n')) {
                        rxbuf[idx] = '\0';
                        idx = 0;
                        nmea_parse(&gps_parser, rxbuf, strlen(rxbuf),
                                        &gps_info);
                }
        }
}

void gps_init()
{
        int i;

        /* Init GPS control pins & delay before ON_OFF pulse */
        USB->ROUTE &= ~(USB_ROUTE_VBUSENPEN);
        GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1);
        GPIO_PinModeSet(gpioPortA, 1, gpioModeInput, 0);
        GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0);
        for (i = 0; i < 10000000; i++)
                ;
        gps_on_off_pulse();

        /* LEUART0 config */
        LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

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

        /* NMEA parser & info structure init */
        nmea_zero_INFO(&gps_info);
        nmea_parser_init(&gps_parser);
}

void gps_on_off_pulse()
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

void gps_reset(int val)
{
        val ? GPIO_PinOutClear(gpioPortF, 5) : GPIO_PinOutSet(gpioPortF, 5);
}

int gps_puts(char *s)
{
        while (*s++) {
                if (*s == EOF)
                        return EOF;
                LEUART_Tx(LEUART0, *s);
        }

        return 1;
}

int gps_nmea_crc(const char *nmeastr)
{
        int     chksum = 0;
        int     i = 0;
        int     n = 0;
        char    buf[128];

        while (*nmeastr != '*')
                buf[n++] = *nmeastr++;

        for (i = 1; i < n; i++)
                chksum ^= (int)buf[i];

        return chksum;
}

int gps_fixed()
{
        return gps_info.sig;
}

void gps_get_utc(int *yr, int *mon, int *day, int *hr, int *min, int *sec)
{
        *yr  = 1900 + gps_info.utc.year;
        *mon = 1 + gps_info.utc.mon;
        *day = gps_info.utc.day;
        *hr  = gps_info.utc.hour;
        *min = gps_info.utc.min;
        *sec = gps_info.utc.sec;
}

void gps_get_coord(double *lat, double *lon, double *elv)
{
        *lat = gps_info.lat;
        *lon = gps_info.lon;
        *elv = gps_info.elv;
}

void gps_get_speed(double *spd)
{
        *spd = gps_info.speed;
}

void gps_get_direction(double *dir)
{
        *dir = gps_info.direction;
}

