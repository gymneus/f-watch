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

#ifndef __GPS_H_
#define __GPS_H_


/*=============*/
/* GPS defines */
/*=============*/
#define GPS_OK_TO_SEND  "$PSRF150,1*3E\r\n"

#define GPS_RXBUF_SIZE  128

/*=================*/
/* Data structures */
/*=================*/
/*
 * Latitude and longitude can have different formats, see gps_get_coord() for
 * details
 */
struct gps_coord {
    double lat;
    double lon;
    double elev;
};

/* UTC time from GPS */
struct gps_utc {
    int yr;         // years since 1900
    int mon;        // months since January (0-11)
    int day;        // day of month
    int hr;         // hours (0-23)
    int min;        // minutes (0-59)
    int sec;        // seconds (0-59)
};

/*=====================*/
/* Function prototypes */
/*=====================*/
void    gps_init(int pulse_onoff);
void    gps_on_off_pulse();
void    gps_reset(int val);
int     gps_get_framerdy();
void    gps_set_framerdy();
void    gps_parse_nmea(const char *buf);
int     gps_fixed();
void    gps_get_utc(struct gps_utc *utc);
void    gps_get_coord(struct gps_coord *coord, int format);
void    gps_get_speed(double *spd);
void    gps_get_direction(double *dir);
int     gps_puts(const char *s);
int     gps_nmea_crc(const char *nmeastr);

#endif // __GPS_H_
