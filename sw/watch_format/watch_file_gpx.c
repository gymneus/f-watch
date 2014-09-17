#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "watch_file_format.h"

static const char* string_xml_header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n";
static const char* string_gpx_header = "<gpx xmlns=\"http://www.topografix.com/GPX/1/1\" creator=\"freewatch\" version=\"1.1\"\n" \
"   xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n" \
"   xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">";

static const char* string_track_tag = "trk";

uint32_t watch_gpx_write_header(uint8_t* dest) {
	char* str_dest = (char*) dest;
	int n = sprintf(str_dest, "%s\n%s\n  <%s>\n", string_xml_header, string_gpx_header, string_track_tag);
	return (uint32_t) n;
}

static const char* string_track_segment_tag = "trkseg";

uint32_t watch_gpx_write_track_segment_begin(uint8_t* dest) {
	char* str_dest = (char*) dest;
	int n = sprintf(str_dest, "<%s>\n", string_track_segment_tag);
	return (uint32_t) n;
}
uint32_t watch_gpx_write_track_segment_end(uint8_t* dest) {
	char* str_dest = (char*) dest;
	int n = sprintf(str_dest, "</%s>\n", string_track_segment_tag);
	return (uint32_t) n;
}

static const char* string_trackpoint_tag = "trkpt";
static const char* string_elevation_tag = "ele";
static const char* string_time_tag = "time";
static const char* string_extensions_tag = "extensions";
static const char* string_extaltgps_tag = "fwtch:alt_gps";
static const char* string_extaltfrompressure_tag = "fwtch:alt_from_pressure";
static const char* string_extpressure_tag = "fwtch:pressure";

uint32_t watch_gpx_write_data(watch_format_data* data, uint8_t* dest) {
	int n = 0;
	n += sprintf(((char*) dest + n), "<%s lon=\"%lf\" lat=\"%lf\">\n", string_trackpoint_tag,
		(data->fields_mask & (1 << WATCH_PRESENCE_LAT_GPS) ? data->lon_gps : 0),
		(data->fields_mask & (1 << WATCH_PRESENCE_LON_GPS) ? data->lat_gps : 0));
	if (	(data->fields_mask & (
			(1 << WATCH_PRESENCE_ALT_GPS) ||
			(1 << WATCH_PRESENCE_ALT_FROM_PRESSURE))) ) {
		double altitude = (data->fields_mask & (1 << WATCH_PRESENCE_ALT_FROM_PRESSURE) ?
					data->alt_from_pressure : data->alt_gps);//pressure more prioritar
		n += sprintf(((char*) dest + n), "  <%s>%ld</%s>\n", string_elevation_tag, (long) altitude, string_elevation_tag);
	}
	n += sprintf(((char*) dest + n), "  <%s>", string_time_tag);
	{
		struct tm date_and_time;
		time_t seconds = data->time_sec;
		localtime_r(&seconds, &date_and_time);
		strftime(((char*) dest + n), 22, "%Y-%m-%dT%H:%M:%S ", &date_and_time);
		n += 19;//added manually because strftime returns 0
	}
	n += sprintf(((char*) dest + n), "</%s>\n", string_time_tag);
	if (	(data->fields_mask & (
			(1 << WATCH_PRESENCE_ALT_GPS) ||
			(1 << WATCH_PRESENCE_ALT_FROM_PRESSURE) ||
			(1 << WATCH_PRESENCE_PRESSURE))) ) {
		n += sprintf(((char*) dest + n), "  <%s>\n", string_extensions_tag);
		if (data->fields_mask & (1 << WATCH_PRESENCE_ALT_GPS)) {
			n += sprintf(((char*) dest + n), "    <%s>%ld</%s>\n", string_extaltgps_tag, (long) data->alt_gps, string_extaltgps_tag);
		}
		if (data->fields_mask & (1 << WATCH_PRESENCE_ALT_FROM_PRESSURE)) {
			n += sprintf(((char*) dest + n), "    <%s>%ld</%s>\n", string_extaltfrompressure_tag, (long) data->alt_gps, string_extaltfrompressure_tag);
		}
		if (data->fields_mask & (1 << WATCH_PRESENCE_PRESSURE)) {
			n += sprintf(((char*) dest + n), "    <%s>%ld</%s>\n", string_extpressure_tag, (long) data->pressure, string_extpressure_tag);
		}
		n += sprintf(((char*) dest + n), "  </%s>\n", string_extensions_tag);
	}
	n += sprintf(((char*) dest + n), "</%s>\n", string_trackpoint_tag);
	return (uint32_t) n;
}

static const char* string_gpx_footer = "</gpx>";

uint32_t watch_gpx_write_footer(uint8_t* dest) {
	char* str_dest = (char*) dest;
	int n = sprintf(str_dest, "  </%s>\n%s", string_track_tag, string_gpx_footer);
	return (uint32_t) n;
}

