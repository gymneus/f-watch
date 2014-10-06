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

#include <drivers/gps.h>
#include <nmea/nmea.h>

#ifdef DEBUG
#include <usbdbg.h>
#endif

static char rxbuf[GPS_RXBUF_SIZE];
static volatile int idx = 0;
static volatile int framerdy = 0;

static nmeaINFO info;
static nmeaPARSER parser;

__attribute__((__weak__))
void LEUART0_IRQHandler()
{
    if (LEUART0->IF & LEUART_IF_RXDATAV) {
        rxbuf[idx++] = LEUART_Rx(LEUART0);
        if ((rxbuf[idx-2] == '\r') && (rxbuf[idx-1] == '\n')) {
            rxbuf[idx] = '\0';
            idx = 0;
            gps_set_framerdy(1);
            gps_parse_nmea(rxbuf);
        }
    }
}

void gps_init(int pulse_onoff)
{
    int i;

    /* Init GPS control pins & delay before ON_OFF pulse */
    USB->ROUTE &= ~(USB_ROUTE_VBUSENPEN);
    GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1);
    GPIO_PinModeSet(gpioPortA, 1, gpioModeInput, 0);
    GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0);

    if (pulse_onoff) {
        for (i = 0; i < 100000; i++)
            ;
        gps_on_off_pulse();
    }

    /* LEUART0 config */
    LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

    GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 1);
    GPIO_PinModeSet(gpioPortE, 15, gpioModeInput, 0);

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
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
}

void gps_on_off_pulse()
{
    int i;

    // TODO: change delays to UDELAYs or smth
    /* Pulse */
    GPIO_PinOutSet(gpioPortE, 13);
    for (i = 0; i < 100000; i++)
            ;
    GPIO_PinOutClear(gpioPortE, 13);

    /* Delay to make sure GPS module sees this as a pulse */
    for (i = 0; i < 100000; i++)
        ;
}

void gps_reset(int val)
{
    val ? GPIO_PinOutClear(gpioPortF, 5) : GPIO_PinOutSet(gpioPortF, 5);
}

int gps_get_framerdy()
{
    return framerdy;
}

void gps_set_framerdy(int param)
{
    framerdy = param;
}

void gps_parse_nmea(const char *buf)
{
    // TODO: check return of nmea_parse
    nmea_parse(&parser, buf, strlen(buf), &info);
#ifdef DEBUG
    /*
     * NOTE: usbdbg_init() should be called EXTERNALLY, BEFORE gps_init() is
     * called
     */
    usbdbg_puts(buf);
#endif
}

int gps_fixed()
{
    return info.sig;
}

void gps_get_utc(struct gps_utc *utc)
{
    utc->yr  = info.utc.year;
    utc->mon = info.utc.mon;
    utc->day = info.utc.day;
    utc->hr  = info.utc.hour;
    utc->min = info.utc.min;
    utc->sec = info.utc.sec;
}

void gps_get_coord(struct gps_coord *coord, int format)
{
    if (format == 0) {
        /* Raw [deg][min].[sec/60] */
        coord->lat = info.lat;
        coord->lon = info.lon;
    } else if (format == 1) {
        /* [deg][min].[sec] */
        coord->lat = (int)info.lat + 0.6 * (
            info.lat - (int)info.lat);
        coord->lon = (int)info.lon + 0.6 * (
            info.lon - (int)info.lon);
    } else if (format == 2) {
        /* [deg].[min/60] */
        float tmp;
        tmp = info.lat/100;
        coord->lat = (int)tmp + (tmp - (int)tmp) / 0.6;
        tmp = info.lon/100;
        coord->lon = (int)tmp + (tmp - (int)tmp) / 0.6;
    }
    coord->elev = info.elv;
}

void gps_get_speed(double *spd)
{
    *spd = info.speed;
}

void gps_get_direction(double *dir)
{
    *dir = info.direction;
}

int gps_puts(const char *s)
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

