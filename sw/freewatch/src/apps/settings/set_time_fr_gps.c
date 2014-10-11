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

#include "application.h"

#include <time.h>
#include <drivers/gps.h>

#include "clock.h"
#include "settings.h"

void set_time_fr_gps_main(void *params)
{
    /* Avoid compiler warning */
    (void) params;

    struct tm time;
    struct gps_utc gpstime;

    if (gps_fixed()) {
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
}

application set_time_fr_gps = {
    .name = "Set time fr GPS",      // this will be shown in menu
    .main = set_time_fr_gps_main
};