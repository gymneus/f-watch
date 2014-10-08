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
#include <semphr.h>

#include "settings/settings.h"

#include <drivers/gps.h>
#include <event.h>

#include <time.h>

#ifdef DEBUG
#include <usbdbg.h>
#endif

#include "application.h"
#include "clock.h"

#include <stdio.h>
#include <string.h>

#define GPSBKGND_TIMER_PERIOD (1000 / portTICK_RATE_MS)

static xTimerHandle timerGps;
extern xSemaphoreHandle mutexSdCardAccess;

static int firstrun, firstfix;
static int gpson, pgpson;
static int track, ptrack;

extern xTaskHandle taskGpsTrack;

static void gpsbkgnd_task(void *params)
{
    (void) params;
    struct event e;
    struct tm time;
    struct gps_utc gpstime;

    /* Previous and current state of GPS ON setting */
    pgpson = gpson;
    gpson = setting_get(&setting_gps_on);

    /* Pulse GPS ON_OFF pin if setting changed */
    if ((pgpson != gpson) && !firstrun)
        gps_on_off_pulse();

    if (!gpson) {
        /* Turn off status bar icon if GPS turns off */
        if (pgpson) {
            e.type = GPS_OFF;
            xQueueSendToBack(appQueue, (void *)&e, 0);
        }
        return;
    }

    /* Set time from GPS at first fix or midday */
    if (setting_get(&setting_gps_sets_time) && gps_fixed()) {
        time = clock_get_time();

        if (firstfix ||
                ((time.tm_hour == 12) && (time.tm_min == 0))) {
            gps_get_utc(&gpstime);

            time.tm_year = gpstime.yr;
            time.tm_mon = gpstime.mon;
            time.tm_mday = gpstime.day;
            time.tm_hour = gpstime.hr + setting_get(&setting_gmt_ofs_hr);
            time.tm_min = gpstime.min + setting_get(&setting_gmt_ofs_min);
            time.tm_sec = gpstime.sec;
            time.tm_isdst = 0;

            clock_set_time(&time);
        }

        if (firstfix) firstfix = 0;
    }

    /*
     * Tickle GPS tracking task and status bar in case of tracking setting
     * change
     */
    ptrack = track;
    track = setting_get(&setting_tracking);
    if (track) {
        if (!ptrack || firstrun) {
            vTaskResume(taskGpsTrack);
            e.type = TRACK_ON;
            xQueueSendToBack(appQueue, &e, 0);
        }
    } else if (ptrack) {
        e.type = TRACK_OFF;
        xQueueSendToBack(appQueue, &e, 0);
    }

    /* Make sure we don't have more than one first run */
    if (firstrun)
        firstrun = 0;

    /* Tickle tasks waiting for the GPS */
    e.type = GPS_TICK;
    xQueueSendToBack(appQueue, &e, 0);
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

