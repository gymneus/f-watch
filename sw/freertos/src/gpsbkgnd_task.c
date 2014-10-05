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

#include <FreeRTOS.h>
#include <timers.h>

#include "settings/settings.h"

#include <drivers/gps.h>
#include <event.h>

#include <usbdbg.h>
#include <stdio.h>
#include <time.h>

#include "application.h"
#include "clock.h"

#define GPSBKGND_TIMER_PERIOD (1000 / portTICK_RATE_MS)

static xTimerHandle timerGps;

static int firstrun, firstfix;
static int cgpson, pgpson;

static void gpsbkgnd_task(void *params)
{
    (void) params;
    struct event e;
    struct tm time;
    struct gps_utc gpstime;

    /* Previous and current state of GPS on setting at timer tick */
    pgpson = cgpson;
    cgpson = setting_gps_on.val;

    /* Pulse GPS ON_OFF pin if setting changed */
    if ((pgpson != cgpson) && !firstrun)
        gps_on_off_pulse();
    if (firstrun)
        firstrun = 0;

    if (!cgpson) {
        /* Turn off status bar icon if GPS turns off */
        if (pgpson) {
            e.type = GPS_OFF;
            xQueueSendToBack(appQueue, (void *)&e, 0);
        }
        return;
    }

    char b[64];

    /* Set time from GPS at first fix or midday */
    if (setting_gps_sets_time.val && gps_fixed()) {
        time = clock_get_time();

//        sprintf(b, "BEF: %d-%d-%d %d:%d:%d\r\n",
//                        time.tm_year, time.tm_mon, time.tm_wday,
//                        time.tm_hour, time.tm_min, time.tm_sec);
//        usbdbg_puts(b);

        if (firstfix ||
                ((time.tm_hour == 12) && (time.tm_min == 0))) {
            gps_get_utc(&gpstime);

            time.tm_year = gpstime.yr;
            time.tm_mon = gpstime.mon;
            time.tm_mday = gpstime.day;
            time.tm_hour = gpstime.hr + setting_gmt_ofs.tm_hour;
            time.tm_min = gpstime.min + setting_gmt_ofs.tm_min;
            time.tm_sec = gpstime.sec;

            clock_set_time(&time);
        }

        if (firstfix) firstfix = 0;
    }

    e.type = GPS_TICK;
    xQueueSendToBack(appQueue, (void *)&e, 0);
}

void gpsbkgnd_init()
{
    firstrun = 1;
    firstfix = 1;
    timerGps = xTimerCreate((signed char *)"timerGps",
                                GPSBKGND_TIMER_PERIOD, pdTRUE,
                                (void *)0, gpsbkgnd_task);
    xTimerStart(timerGps, 0);
}

