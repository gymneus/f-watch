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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
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

#include <microsd.h>
#include <diskio.h>
#include <ff.h>

#define GPSBKGND_TIMER_PERIOD (1000 / portTICK_RATE_MS)

static xTimerHandle timerGps;
extern xSemaphoreHandle mutexSdCardAccess;
static int mutexours = 0;

static int firstrun, firstfix;
static int gpson, pgpson;
static int track, ptrack;

static FIL f;
static FATFS fatfs;
static int open = 1;

/* This function is needed by the FATFS library */
DWORD get_fattime(void)
{
  return (28 << 25) | (2 << 21) | (1 << 16);
}

/* Local function prototypes */
static void gpsbkgnd_task(void *params);

static void update_time();
static void store_track();
static void track_on();
static void track_off();

void gpsbkgnd_init()
{
    firstrun = 1;
    firstfix = 1;
    timerGps = xTimerCreate((signed char *)"timerGps",
                                GPSBKGND_TIMER_PERIOD, pdTRUE,
                                (void *)0, gpsbkgnd_task);
    xTimerStart(timerGps, 0);
}

static int nr=0;

static void gpsbkgnd_task(void *params)
{
    (void) params;
    struct event e;

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

    /* Turn on tracking icon based on setting */
    ptrack = track;
    track = setting_get(&setting_tracking);
    if (track) {
        if (!ptrack || firstrun) {
            track_on();
//            e.type = GPS_TRACK_ON;
//            xQueueSendToBack(appQueue, &e, 0);
        }
    } else if (ptrack) {
        track_off();
//        e.type = GPS_TRACK_OFF;
//        xQueueSendToBack(appQueue, &e, 0);
    }

//    /* Set time and track according to setting */
//    if (gps_fixed()) {
//        if (setting_get(&setting_gps_sets_time))
//            update_time();
//        if (track)
//            store_track();
//    }

    //UINT dummy;
    //char buf[16];
    //char fname[64];
    //if (nr == 0) {
    //    track_on();
    //}
    //if (nr < 10) {
    //    if (!mutexours) track_on();
    //    sprintf(buf, "%d\n", nr);
    //    usbdbg_puts(buf);
    //    if (open) {
    //        sprintf(fname, "track");
    //        open = f_open(&f, fname, FA_CREATE_ALWAYS | FA_WRITE);
    //    } else {
    //        f_write(&f, buf, strlen(buf), &dummy);
    //    }
    //}
    //if (nr == 10){
    //    track_off();
    //}
    //nr++;

    if (firstrun)
        firstrun = 0;

    /* Tickle tasks waiting for the GPS */
    e.type = GPS_TICK;
    xQueueSendToBack(appQueue, &e, 0);
}

static void update_time()
{
    struct tm time;
    struct gps_utc gpstime;

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

static void store_track()
{
    struct gps_utc gpstime;
    struct gps_coord gpscoord;
    char buf[80];
    char fname[64];
    UINT dummy;

    /*
     * Re-attempt to get mutex and open if we couldn't when tracking
     * started
     */
    if (!mutexours) {
        track_on();
    } else {
        if (open) {
            /*
             * We open the file here so that we can give it a name according
             * to current time
             */
            gps_get_utc(&gpstime);
            sprintf(fname, "track_%d-%02d-%02d_%02dh%02dm%02ds.txt",
                            1900 + gpstime.yr, 1 + gpstime.mon,
                            gpstime.day,
                            gpstime.hr + setting_get(&setting_gmt_ofs_hr),
                            gpstime.min + setting_get(&setting_gmt_ofs_min),
                            gpstime.sec);
            open = f_open(&f, fname, FA_CREATE_ALWAYS | FA_WRITE);
        } else {
            /*
             * When we've opened (f_open() returns 0 on success), start
             * putting coords into file
             */
            gps_get_coord(&gpscoord, 2);
            sprintf(buf, "%3.7f,%3.7f\n", gpscoord.lat, gpscoord.lon);
            f_write(&f, buf, strlen(buf), &dummy);
        }
    }
}

static void track_on()
{
    if (xSemaphoreTake(mutexSdCardAccess, 0)) {
        usbdbg_puts("take\r\n");
        mutexours = 1;
        MICROSD_Init();
        disk_initialize(0);
        f_mount(0, &fatfs);
        open = 1;
    }
}

static void track_off()
{
    usbdbg_puts("give\r\n");
    f_close(&f);
    MICROSD_Deinit();
    xSemaphoreGive(mutexSdCardAccess);
    mutexours = 0;
    open = 1;
}

