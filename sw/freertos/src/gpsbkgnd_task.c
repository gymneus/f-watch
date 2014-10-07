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

#include <ff.h>
#include <microsd.h>
#include <diskio.h>

#include <stdio.h>
#include <string.h>

#define GPSBKGND_TIMER_PERIOD (1000 / portTICK_RATE_MS)

static xTimerHandle timerGps;
extern xSemaphoreHandle mutexSdCardAccess;

static int firstrun, firstfix;
static int cgpson, pgpson;
static int ctrack, ptrack;

static FIL f;
static FATFS fatfs;

DWORD get_fattime(void)
{
  return (28 << 25) | (2 << 21) | (1 << 16);
}

static void gpsbkgnd_task(void *params)
{
    (void) params;
    struct event e;
    struct tm time;
    struct gps_utc gpstime;
    struct gps_coord gpscoord;
    char buf[80];

    /* Previous and current state of settings */
    pgpson = cgpson;
    cgpson = setting_get(&setting_gps_on);

    /* Pulse GPS ON_OFF pin if setting changed */
    if ((pgpson != cgpson) && !firstrun)
        gps_on_off_pulse();

    if (!cgpson) {
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

    /* Track GPS position if setting tells us to */
    ptrack = ctrack;
    ctrack = setting_get(&setting_tracking);
    if (ctrack) {

        /* Init stuff, open file & take semaphore so that only we write to SD */
        if (firstrun || !ptrack) {
            xSemaphoreTake(mutexSdCardAccess, 0);
            MICROSD_Init();
            disk_initialize(0);
            f_mount(0, &fatfs);
            f_open(&f, "yellow", FA_CREATE_ALWAYS | FA_WRITE);
            f_lseek(&f, 0);
        }

        /* Write to file if gps is fixed */
        if (gps_fixed()) {
            gps_get_coord(&gpscoord, 2);
            sprintf(buf, "%3.7f,%3.7f\n", gpscoord.lat, gpscoord.lon);
            f_write(&f, buf, strlen(buf), NULL);
        }

    } else if (!ctrack && ptrack) {
        /*
         * Turned off tracking setting => close file, deinit microsd driver and
         * give mutex so that other tasks can use the SD card
         */
        f_close(&f);
        MICROSD_Deinit();
        xSemaphoreGive(mutexSdCardAccess);
    }

    if (firstrun)
        firstrun = 0;

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

