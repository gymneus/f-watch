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

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "application.h"

#include <FreeRTOSConfig.h>
#include <semphr.h>
#include <event.h>

#include <drivers/gps.h>

#include <microsd.h>
#include <diskio.h>
#include <ff.h>

#include <settings/settings.h>

#include <usbdbg.h>

extern xTaskHandle taskGpsTrack;
extern xSemaphoreHandle mutexSdCardAccess;
static int open = 0;
static int mutexours = 0;

static FIL f;
static FATFS fatfs;

DWORD get_fattime(void)
{
  return (28 << 25) | (2 << 21) | (1 << 16);
}

void gpstrack_task(void *params)
{
    (void) params;

    struct event e;
    struct gps_utc gpstime;
    struct gps_coord gpscoord;
    char buf[80];
    char fname[64];
    UINT dummy;

    while(1) {
        if (xQueueReceive(appQueue, (void *)&e, portMAX_DELAY)) {
            switch (e.type) {
            case TRACK_ON:
                usbdbg_puts("trackon\r\n");
                if (xSemaphoreTake(mutexSdCardAccess, 0)) {
                    mutexours = 1;
                    MICROSD_Init();
                    disk_initialize(0);
                    f_mount(0, &fatfs);
                    open = 1;
                }
                break;
            case TRACK_OFF:
                usbdbg_puts("trackoff\r\n");
                f_close(&f);
                MICROSD_Deinit();
                mutexours = 0;
                open = 1;
                xSemaphoreGive(mutexSdCardAccess);
                vTaskSuspend(taskGpsTrack);
                break;
            case GPS_TICK:
                /*
                 * Re-attempt to get mutex and open if we couldn't when tracking
                 * started
                 */
                usbdbg_puts("tick\r\n");
                if (!mutexours) {
                    mutexours = 1;
                    MICROSD_Init();
                    disk_initialize(0);
                    f_mount(0, &fatfs);
                    open = 1;
                } else if (gps_fixed()) {
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
                break;
            default:
                break;
            }
        }
    }
}
