/*
 * Copyright (C) 2014 Julian Lewis
 * @author Theodor Stana <theodor.stana@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <event.h>
#include <drivers/gps/gps.h>

#include <em_gpio.h>

#include "application.h"

#include <usbdbg.h>

#define dbg()                       \
    int i;                          \
    GPIO_PinOutSet(gpioPortE, 11);  \
    for (i = 0; i < 1000000; i++)   \
        ;                           \
    GPIO_PinOutClear(gpioPortE, 11);

extern volatile char gps_rxbuf[GPS_RXBUF_SIZE];

extern xSemaphoreHandle sem_gps;

void gpsbkgrnd_main(void *params)
{
    /* suppress compiler warning */
    (void) params;

    while (1) {
        if (xSemaphoreTake(sem_gps, portMAX_DELAY)) {
            gps_parse_nmea(gps_rxbuf);
        }
    }
}

application gpsbkgrnd = {
    .main = gpsbkgrnd_main,
    .name = "GPS background"
};
