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

static void take_storage();
static void give_storage();

static void store_track();
void stringify(struct gps_coord *, char *, char *);

void gpsbkgnd_init()
{
    firstrun = 1;
    firstfix = 1;
    timerGps = xTimerCreate((signed char *)"timerGps",
                                GPSBKGND_TIMER_PERIOD, pdTRUE,
                                (void *)0, gpsbkgnd_task);
    xTimerStart(timerGps, 0);
}

static void gpsbkgnd_task(void *params)
{
    (void) params;
    struct event e;
    static int gpson, pgpson;
    static int track, ptrack;

    /* Previous and current state of GPS ON setting */
    pgpson = gpson;
    gpson = setting_get(&setting_gps_on);

    /* Pulse GPS ON_OFF pin if setting changed */
    if ((pgpson != gpson) && !firstrun) {
        gps_on_off_pulse();
    }

    /*
     * Tracking actions based on previous and current value of setting
     * - ON to OFF -- take control of storage so we can store
     * - OFF to ON -- give control of storage so other tasks can store to it
     */
    ptrack = track;
    track = setting_get(&setting_tracking);
    if (track) {
        if (!ptrack || firstrun)
            take_storage();
    } else if (ptrack) {
        give_storage();
    }

    /* Set time and track according to setting */
    if (gps_fixed()) {
        if (setting_get(&setting_gps_sets_time))
            update_time();
        if (track) {
            store_track();
        }
    }

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

static void take_storage()
{
    if (xSemaphoreTake(mutexSdCardAccess, 0)) {
        mutexours = 1;
        MICROSD_Init();
        disk_initialize(0);
        f_mount(0, &fatfs);
        open = 1;
    }
}

static void give_storage()
{
    f_close(&f);
    MICROSD_Deinit();
    xSemaphoreGive(mutexSdCardAccess);
    mutexours = 0;
    open = 1;
}

static void store_track()
{
    struct tm time;

    struct gps_coord gpscoord;
    char lats[16], lons[16];

    char buf[128];
    char fname[64];
    UINT dummy;

    /*
     * Re-attempt to get mutex and open if we couldn't when tracking
     * started
     */
    if (!mutexours) {
        take_storage();
    } else {
        if (open) {
            /*
             * We open the file here so that we can give it a name according
             * to current time
             */
            time = clock_get_time();
            sprintf(fname, "track_%d-%02d-%02d_%02dh%02dm%02ds.txt",
                            1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday,
                            time.tm_hour, time.tm_min, time.tm_sec);
            open = f_open(&f, fname, FA_CREATE_ALWAYS | FA_WRITE);
        } else {
            /*
             * When we've opened (f_open() returns 0 on success), start
             * putting coords into file. We try to massage the float coordinates
             * into strings before printing to file, rather than sprintf-ing to
             * the file directly, as the latter tends to hard-fault the CPU.
             */
            gps_get_coord(&gpscoord, 2);

            stringify(&gpscoord, lats, lons);

            strcpy(buf, lats);
            strcat(buf,",");
            strcat(buf, lons);
            strcat(buf, "\n");

            f_write(&f, buf, strlen(buf), &dummy);
        }
    }
}

void stringify(struct gps_coord *c, char *lats, char *lons)
{
    int lat, lon, shift;
    char tmp1[16], tmp2[16];

    /*
     * Convert latitude and longitude to ints, then to strings;
     * use 7 digits after decimal point for coordinates
     */
    lat = c->lat * 10000000;
    strcpy(lats, "");
    if (lat < 0) {
        *lats++ = '-';
        lat *= -1;
    }
    shift = lat;
    while (lat) {
        lats++;
        lat /= 10;
    }
    *lats = '\0';
    while(shift) {
        *--lats = '0' + (shift % 10);
        shift /= 10;
    }

    lon = c->lon * 10000000;
    strcpy(lons, "");
    if (lon < 0) {
        *lons++ = '-';
        lon *= -1;
    }
    shift = lon;
    while (lon) {
        lons++;
        lon /= 10;
    }
    *lons = '\0';
    while(shift) {
        *--lons = '0' + (shift % 10);
        shift /= 10;
    }

    /*
     * Avoid crashes at global position 0/0, or add the decimal points at the
     * appropriate positions in the strings
     */
    if ((strlen(lats) == 0) && (strlen(lons) == 0)) {
        strcpy(lats, "0.0000000");
        strcpy(lons, "0.0000000");
    } else {
        strncpy(tmp1, lats, strlen(lats)-7);
        strcpy(tmp1+strlen(lats)-7, "\0");
        strncpy(tmp2, lats+(strlen(lats)-7), 7);
        strcpy(tmp2+7, "\0");

        strcpy(lats, tmp1);
        strcat(lats, ".");
        strcat(lats, tmp2);

        strncpy(tmp1, lons, strlen(lons)-7);
        strcpy(tmp1+strlen(lons)-7, "\0");
        strncpy(tmp2, lons+(strlen(lons)-7), 7);
        strcpy(tmp2+7, "\0");

        strcpy(lons, tmp1);
        strcat(lons, ".");
        strcat(lons, tmp2);
    }
}

