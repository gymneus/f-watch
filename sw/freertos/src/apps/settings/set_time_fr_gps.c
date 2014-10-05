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
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "application.h"

#include <time.h>
#include <drivers/gps.h>

#include "clock.h"
#include "settings.h"

#include <usbdbg.h>

void set_time_fr_gps_main(void *params)
{
    struct tm time;
    struct gps_utc gpstime;
    char b[32];

    if (gps_fixed()) {
        gps_get_utc(&gpstime);
        sprintf(b, "GPS: %d-%d-%d %d:%d:%d\r\n",
                        gpstime.yr, gpstime.mon, gpstime.day,
                        gpstime.hr, gpstime.min, gpstime.sec);
        usbdbg_puts(b);

        sprintf(b, "OFS: %d-%d-%d %d:%d:%d\r\n",
            setting_gmt_ofs.tm_year, setting_gmt_ofs.tm_mon, setting_gmt_ofs.tm_wday,
            setting_gmt_ofs.tm_hour, setting_gmt_ofs.tm_min, setting_gmt_ofs.tm_sec);
        usbdbg_puts(b);

        time.tm_year = gpstime.yr;
        time.tm_mon = gpstime.mon;
        time.tm_wday = gpstime.day;
        time.tm_hour = gpstime.hr + setting_gmt_ofs.tm_hour;
        time.tm_min = gpstime.min + setting_gmt_ofs.tm_min;
        time.tm_sec = gpstime.sec;
        time.tm_isdst = 0;

        sprintf(b, "AFT: %d-%d-%d %d:%d:%d\r\n",
                        time.tm_year, time.tm_mon, time.tm_wday,
                        time.tm_hour, time.tm_min, time.tm_sec);
        usbdbg_puts(b);

        clock_set_time(&time);
    }
}

application set_time_fr_gps = {
    .name = "Set time fr GPS",      // this will be shown in menu
    .main = set_time_fr_gps_main
};
