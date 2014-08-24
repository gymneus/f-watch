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

/*=====================*/
/* Function prototypes */
/*=====================*/
void    gps_init();
void    gps_on_off_pulse();
void    gps_reset(int val);
int     gps_puts(char *s);
int     gps_nmea_crc(const char *nmeastr);


#endif // __GPS_H_
