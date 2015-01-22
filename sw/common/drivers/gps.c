/*
 * Copyright (C) 2014 Julian Lewis
 * @author Theodor Stana <theodor.stana@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Antenova M10478 GPS module driver
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
static volatile int irq_sync = 0;

static nmeaINFO info;
static nmeaPARSER parser;

static int fixed;

__attribute__((__weak__))
void LEUART0_IRQHandler()
{
    if (LEUART0->IF & LEUART_IF_RXDATAV) {
        rxbuf[idx++] = LEUART_Rx(LEUART0);
        if ((rxbuf[idx-2] == '\r') && (rxbuf[idx-1] == '\n')) {
            rxbuf[idx] = '\0';
            idx = 0;
            gps_set_irq_sync(1);
            gps_parse_nmea(rxbuf);
        }
    }
}

/**
 * @brief GPS initialization routine
 * @param[in] pulse_onoff
 *      Pulse the M10478 ON_OFF pin to turn it on at startup.
 */
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

    /* We shouldn't have fix after init */
    fixed = 0;
}

/**
 * @brief
 *      Pulse M10478 ON_OFF pin to turn it on and off
 */
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

    /* Turn on and off should mean we lost fix */
    fixed = 0;
}

/**
 * @brief
 *      Set state of the GPS reset pin.
 * @param[in] val
 *      Value to set at reset pin
 *      '0' to assert the reset
 *      '1' to de-assert the reset
 */
void gps_reset(int val)
{
    val ? GPIO_PinOutClear(gpioPortF, 5) : GPIO_PinOutSet(gpioPortF, 5);
    fixed = 0;
}

/**
 * @brief
 *      Get the the state of synchronization with the GPS ISR
 * @return
 *      '1' when the ISR has set the synchronization variable, signaling the
 *      external code that a frame has been parsed
 *      '0' when the application should wait for a frame to be received
 */
int gps_get_irq_sync()
{
    return irq_sync;
}

/**
 * @brief
 *      Set the state of IRQ synchronization mechanism. An ISR should use this
 *      function with parameter '1' to signal external code that a frame has
 *      been received and is ready for parsing.
 * @param[in] param
 *      '1' to be set by the ISR to signal external code that a frame has been
 *      received
 *      '0' to be set by the external code to signal the ISR that it may
 *      continue receiving frames
 */
void gps_set_irq_sync(int param)
{
    irq_sync = param;
}

/**
 * @brief
 *      Parse an NMEA frame received from the GPS receiver.
 * @param[in] buf
 *      Buffer containing NMEA data to be processed
 */
void gps_parse_nmea(const char *buf)
{
    // TODO: check return of nmea_parse
    nmea_parse(&parser, buf, strlen(buf), &info);

    fixed = info.sig;

    /* On turn-off string from GPS module, set fixed to zero */
    if (strcmp(buf, "$PSRF150,0*3F\r\n") == 0) {
        fixed = 0;
    }

#ifdef DEBUG
    /*
     * NOTE: usbdbg_init() should be called EXTERNALLY, BEFORE gps_init() is
     * called
     */
    usbdbg_puts(buf);
#endif
}

/**
 * @brief
 *      Return the fix state of the GPS receiver
 * @return
 *      0 when the GPS is not fixed
 *      1 when we have fix
 *      2 differential GPS
 *      3 sensitive
 */
int gps_fixed()
{
    return fixed;
}

/**
 * @brief
 *      Get the UTC time from GPS
 * @param[out] utc
 *      Pointer to UTC structure to change. The GPS UTC time is applied to this
 *      structure.
 */
void gps_get_utc(struct gps_utc *utc)
{
    utc->yr  = info.utc.year;
    utc->mon = info.utc.mon;
    utc->day = info.utc.day;
    utc->hr  = info.utc.hour;
    utc->min = info.utc.min;
    utc->sec = info.utc.sec;
}

/**
 * @brief
 *      Get the GPS coordinates of the receiver
 * @param[out] coord
 *      Pointer to coordinate structure to change. The GPS coordinates are
 *      a set of double values applied as fields of this structure, according
 *      to the format parameter.
 * @param[in] format
 *      Format in which the coordinate structure should be presented.
 *      0 [deg][min].[sec/60]
 *      1 [deg][min].[sec]
 *      2 [deg].[min/60]
 */
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

/**
 * @brief
 *      Get the speed of the GPS receiver
 * @param[out] spd
 *      Pointer to double value to apply the speed to.
 */
void gps_get_speed(double *spd)
{
    *spd = info.speed;
}

/**
 * @brief
 *      Get the direction of the GPS receiver
 * @param[out] dir
 *      Pointer to double value to apply the direction to.
 */
void gps_get_direction(double *dir)
{
    *dir = info.direction;
}

/**
 * @brief
 *      Send a string to the GPS receiver
 * @param[in] s
 *      Character string to send to GPS
 * @return
 *      EOF when the terminator char ('\0') is received
 */
static int gps_puts(const char *s)
{
    while (*s++) {
        if (*s == 0)
            return EOF;
        LEUART_Tx(LEUART0, *s);
    }

    return 1;
}

static int gps_nmea_crc(const char *nmeastr)
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

