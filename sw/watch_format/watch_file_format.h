/*
 * Copyright (C) 2014 Julian Lewis
 * @author Xavier Piroux <xavierpiroux@gmail.com>
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

#ifndef __WATCH_FILE_FORMAT_H
#define __WATCH_FILE_FORMAT_H


/**
 * @brief headers to read, write and interpret data stored in files
 */

#define GPS_FILE_FORMAT_VERSION 1

double gps_raw_to_double(uint32_t raw);
uint32_t gps_double_to_raw(double latitude);

typedef enum watch_next_data_type
{
	WATCH_NEXT_DATA_BEGIN = 0,
	WATCH_NEXT_DATA_END,
	WATCH_NEXT_DATA_DATA,
	WATCH_NEXT_DATA_NEWLAP
} watch_next_data_type;

uint8_t watch_get_size_of_data_type(watch_next_data_type data_type);

typedef enum watch_presence_fields_mask
{
  //WATCH_PRESENCE_TIME_SEC = -1, /*!< the most basic data : is always set */
  WATCH_PRESENCE_TIME_MILLIS = 0, /*!< bit set if milliseconds is available */
  WATCH_PRESENCE_LAT_GPS,
  WATCH_PRESENCE_LON_GPS,
  WATCH_PRESENCE_ALT_GPS,
  WATCH_PRESENCE_ALT_FROM_PRESSURE,
  WATCH_PRESENCE_PRESSURE,
} watch_presence_fields_mask;

/** Structure used to save data of the watch */
typedef struct watch_format_data
{
  uint16_t fields_mask; /*!< bit mask to know what are the available data ( from enum watch_format_fields_mask ) */
  uint32_t time_sec; /*!< time in seconds since epoch (from GPS or not) */
  uint16_t time_ms; /*!< milliseconds time */
  double lat_gps; /*!< latitude in degrees */
  double lon_gps; /*!< longitude in degrees */
  double alt_gps; /*!< altitude in meters, can be negative */
  double alt_from_pressure;
  double pressure;
  //uint8_t heart_rate; /*!< we don't have such thing in the freewatch project */
} watch_format_data;

/**
 * @brief write into dest the data converted into format used in the files stored in the SD
 * @param data data to convert
 * @param dest buffer to store converted data
 * @return number of written bytes
 */
uint32_t convert_data_to_file(watch_format_data* data, uint8_t* dest);



uint32_t watch_gpx_write_header(uint8_t* dest);
uint32_t watch_gpx_write_track_segment_begin(uint8_t* dest);
uint32_t watch_gpx_write_track_segment_end(uint8_t* dest);
uint32_t watch_gpx_write_data(watch_format_data* data, uint8_t* dest);
uint32_t watch_gpx_write_footer(uint8_t* dest);


#endif /* __WATCH_FILE_FORMAT_H */
