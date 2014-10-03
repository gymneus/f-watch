/*
 *==============================================================================
 * CERN (BE-CO-HT)
 * Header file for Antenova M10478 GPS module
 *==============================================================================
 *
 * author: Theodor Stana (t.stana@cern.ch)
 *
 * date of creation: 2014-08-24
 *
 * version: 1.0
 *
 * description:
 *
 * dependencies:
 *
 * references:
 *
 *==============================================================================
 * GNU LESSER GENERAL PUBLIC LICENSE
 *==============================================================================
 * This source file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version. This source is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details. You should have
 * received a copy of the GNU Lesser General Public License along with this
 * source; if not, download it from http://www.gnu.org/licenses/lgpl-2.1.html
 *==============================================================================
 * last changes:
 *    2014-08-24   Theodor Stana     t.stana@cern.ch     File created
 *==============================================================================
 * TODO: -
 *==============================================================================
 */

#ifndef __GPS_H_
#define __GPS_H_


/*=============*/
/* GPS defines */
/*=============*/
#define GPS_DBG         1 

#define GPS_OK_TO_SEND  "$PSRF150,1*3E\r\n"

#define GPS_RXBUF_SIZE  128

/*=================*/
/* Data structures */
/*=================*/
struct gps_coord {
    double lat;
    double lon;
    double elev;
};

struct gps_utc {
    int yr;
    int mon;
    int day;
    int hr;
    int min;
    int sec;
};

/*=====================*/
/* Function prototypes */
/*=====================*/
void    gps_init();
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
