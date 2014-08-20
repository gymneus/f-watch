#include <stdint.h>
#include "watch_file_format.h"


double gps_raw_to_double(uint32_t raw) {
	double latitude = (raw  / 8388608.0) - 180.0;
	return latitude;
}
uint32_t gps_double_to_raw(double latitude) {
	uint32_t raw = (latitude + 180) * 8388608;
	return raw;
}
/** How watch data is saved into files */
typedef struct watch_file_format_data_private
{
  uint16_t fields_mask; /*!< bit mask to know what are the available data */
  uint32_t time_sec; /*!< time in seconds since epoch (from GPS or not) */
  uint16_t time_ms; /*!< milliseconds time */
  uint32_t raw_lat_gps; /*!< latitude in degrees */
  uint32_t raw_lon_gps; /*!< longitude in degrees */
  int32_t raw_alt_gps; /*!< altitude in millimeters, can be negative */
  uint32_t raw_alt_from_pressure;
  uint32_t raw_pressure;
  //uint8_t heart_rate; /*!< we don't have such thing in the freewatch project */
} watch_file_format_data_private;

uint8_t watch_get_size_of_data_type(watch_next_data_type data_type) {
	switch (data_type) {
	case WATCH_NEXT_DATA_DATA:
		return (sizeof(watch_file_format_data_private) / sizeof(uint8_t));
	default:
		return 0;
	}
}

#define IS_FIELD_PRESENT(mask, pos) (mask & (1 << pos))

uint32_t convert_data_to_file(watch_format_data* data, uint8_t* dest) {
	watch_file_format_data_private* raw_data = (watch_file_format_data_private*) dest;
	uint16_t mask = data->fields_mask;
	raw_data->fields_mask = mask;
	raw_data->time_sec = data->time_sec;
	raw_data->time_ms = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_TIME_MILLIS) ? data->time_ms : 0);
	raw_data->raw_lat_gps = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_LAT_GPS) ? gps_double_to_raw(data->lat_gps) : 0);
	raw_data->raw_lon_gps = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_LON_GPS) ? gps_double_to_raw(data->lon_gps) : 0);
	raw_data->raw_alt_gps = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_ALT_GPS) ? gps_double_to_raw(data->alt_gps) : 0);
	raw_data->raw_alt_from_pressure = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_ALT_FROM_PRESSURE) ? gps_double_to_raw(data->alt_from_pressure) : 0);
	raw_data->raw_pressure = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_PRESSURE) ? gps_double_to_raw(data->pressure) : 0);
	return watch_get_size_of_data_type(WATCH_NEXT_DATA_DATA);
}
uint32_t convert_file_to_data(uint8_t* src, watch_format_data* data) {
	watch_file_format_data_private* raw_data = (watch_file_format_data_private*) src;
	uint16_t mask = raw_data->fields_mask;
	data->fields_mask = mask;
	data->time_sec = raw_data->time_sec;
	data->time_ms = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_TIME_MILLIS) ? raw_data->time_ms : 0);
	data->lat_gps = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_LAT_GPS) ? gps_raw_to_double(raw_data->raw_lat_gps) : 0);
	data->lon_gps = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_LON_GPS) ? gps_raw_to_double(raw_data->raw_lon_gps) : 0);
	data->alt_gps = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_ALT_GPS) ? gps_raw_to_double(raw_data->raw_alt_gps) : 0);
	data->alt_from_pressure = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_ALT_FROM_PRESSURE) ? gps_raw_to_double(raw_data->raw_alt_from_pressure) : 0);
	data->pressure = (IS_FIELD_PRESENT(mask, WATCH_PRESENCE_PRESSURE) ? gps_raw_to_double(raw_data->raw_pressure) : 0);
	return watch_get_size_of_data_type(WATCH_NEXT_DATA_DATA);
}
