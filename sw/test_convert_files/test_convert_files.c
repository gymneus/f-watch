#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <watch_file_format.h>

#ifndef VERBOSE
#define VERBOSE 0
#endif

#define IS_VERBOSE() (VERBOSE)

static uint32_t bufferStatic[1000];

#define CHECK_DATA_BIT_PRINT(data, bit, COMMONSTRING, IFBIT, IFNOTBIT) \
printf(COMMONSTRING); \
if (data->fields_mask & (1 << bit)) { \
	printf IFBIT; \
} else { \
	printf IFNOTBIT; \
}

void printData(watch_format_data* data) {
	printf(" time %10lu", (unsigned long) data->time_sec);
	CHECK_DATA_BIT_PRINT(data, WATCH_PRESENCE_TIME_MILLIS, "", (".%03lu", (unsigned long) data->time_ms), (".---"))
	CHECK_DATA_BIT_PRINT(data, WATCH_PRESENCE_LAT_GPS, " - lat ", ("% 9.4lf", data->lat_gps), ("----.----"))
	CHECK_DATA_BIT_PRINT(data, WATCH_PRESENCE_LON_GPS, " - lon ", ("% 9.4lf", data->lon_gps), ("----.----"))
	CHECK_DATA_BIT_PRINT(data, WATCH_PRESENCE_ALT_GPS, " - alt ", ("% 9.4lf", data->alt_gps), ("----.----"))
	CHECK_DATA_BIT_PRINT(data, WATCH_PRESENCE_ALT_FROM_PRESSURE, " - alt(pres) ", ("% 9.4lf", data->alt_from_pressure), ("----.----"))
	CHECK_DATA_BIT_PRINT(data, WATCH_PRESENCE_PRESSURE, " - pressure ", ("% 9.4lf", data->pressure), ("----.----"))
}
void constructData(watch_format_data* data) {
	static int iFirstTime = 1;
	static watch_format_data staticdata = {0};
	if (iFirstTime) {
		iFirstTime = 0;
		staticdata.fields_mask = -1;
	}
	*data = staticdata;
	staticdata.time_sec++;
	staticdata.time_ms+=2;
	staticdata.lat_gps+=1.0101;
	staticdata.lon_gps+=0.0202;
	staticdata.alt_gps-=1.0101;
	staticdata.pressure+=1;
	if (staticdata.fields_mask & (1 << WATCH_PRESENCE_PRESSURE)) {
		staticdata.fields_mask ^= (1 << WATCH_PRESENCE_PRESSURE);
	} else {
		staticdata.fields_mask |= (1 << WATCH_PRESENCE_PRESSURE);
	}
}
static uint8_t* pointer_to_save;
static uint8_t* pointer_to_read;
static uint8_t size_of_data;

void saveData(watch_format_data* data) {
	uint8_t buff[size_of_data + 1];
	uint32_t bytes_written;
	buff[0] = WATCH_NEXT_DATA_DATA;
	bytes_written = convert_data_to_file(data, &buff[1]);
	{
		int i;
		for (i = 0 ; i < bytes_written + 1 ; i++) pointer_to_save[i] = buff[i];
	}
	pointer_to_save += (bytes_written + 1);
}
watch_next_data_type readNextDataType(void) {
	watch_next_data_type data_type = *pointer_to_read;
	pointer_to_read++;
	return data_type;
}
void readData(watch_format_data* data) {
	uint32_t bytes_read = convert_file_to_data(pointer_to_read, data);
	pointer_to_read += bytes_read;
}

int main() {
	size_of_data = watch_get_size_of_data_type(WATCH_NEXT_DATA_DATA);
	printf("main() : starting. Constructing false set of data and saving it...\n");
	{
		//be sure the buffer is empty
		uint32_t i = (sizeof(bufferStatic) / sizeof(bufferStatic[0]));
		for (; i > 0 ; i--) bufferStatic[i-1] = 0;
	}
	{
		//save the data
		int i;
		printf("\n\nSaving data...\n");
		pointer_to_save = (uint8_t*) &bufferStatic[0];
		//save preliminary info : we start a new file
		*pointer_to_save = WATCH_NEXT_DATA_BEGIN;
		pointer_to_save++;
		for (i = 0 ; i < 20 ; i++) {
			watch_format_data data;
			constructData(&data);
			saveData(&data);
			if (IS_VERBOSE()) {
				printData(&data);
				printf("\n");
			}
			if (i % 6 == 0) {
				//new lap sometimes
				*pointer_to_save = WATCH_NEXT_DATA_NEWLAP;
				pointer_to_save++;
				if (IS_VERBOSE()) {
					printf("new lap \n");
				}
			}
		}
		//save sentinel info : we finished saving data
		*pointer_to_save = WATCH_NEXT_DATA_END;
		pointer_to_save++;
	}
	{
		//check we can retrieve the data
		char bufferGpx[10000] = {0};
		int continue_reading = 1;
		int begin_happened = 0;
		watch_format_data data;
		uint8_t* pointer_gpx = (uint8_t*) &bufferGpx[0];
		uint32_t bytes_written_gpx;
		printf("\n\nRetrieving data...\n");
		pointer_to_read = (uint8_t*) &bufferStatic[0];
		while (continue_reading) {
			watch_next_data_type data_type = readNextDataType();
			switch (data_type) {
			case WATCH_NEXT_DATA_BEGIN:
				printf("BEGIN detected\n");
				bytes_written_gpx = watch_gpx_write_header(pointer_gpx);
				pointer_gpx += bytes_written_gpx;
				bytes_written_gpx = watch_gpx_write_track_segment_begin(pointer_gpx);
				pointer_gpx += bytes_written_gpx;
				if (begin_happened) {
					continue_reading = 0;
					printf(" ==> already happened, this is an error !\n");
				}
				begin_happened = 1;
				break;
			case WATCH_NEXT_DATA_END:
				printf("END detected\n");
				bytes_written_gpx = watch_gpx_write_track_segment_end(pointer_gpx);
				pointer_gpx += bytes_written_gpx;
				bytes_written_gpx = watch_gpx_write_footer(pointer_gpx);
				pointer_gpx += bytes_written_gpx;
				continue_reading = 0;
				break;
			case WATCH_NEXT_DATA_DATA:
				//printf("DATA detected\n");
				readData(&data);
				if (IS_VERBOSE()) {
					printData(&data);
					printf("\n");
				}
				bytes_written_gpx = watch_gpx_write_data(&data, pointer_gpx);
				pointer_gpx += bytes_written_gpx;
				break;
			case WATCH_NEXT_DATA_NEWLAP:
				printf("NEWLAP detected\n");
				bytes_written_gpx = watch_gpx_write_track_segment_end(pointer_gpx);
				pointer_gpx += bytes_written_gpx;
				bytes_written_gpx = watch_gpx_write_track_segment_begin(pointer_gpx);
				pointer_gpx += bytes_written_gpx;
				break;
			default:
				printf("UNKNOWN data type, exiting...\n");
				continue_reading = 0;
				break;
			}
			if ((unsigned long) pointer_to_read > (unsigned long) (((char*) bufferStatic) + sizeof(bufferStatic))) {
				printf("STOPPING : we are no more in the bufferStatic\n");
				printf("pointer_to_read = %p ; &bufferStatic[0] = %p ; sizeof(bufferStatic) = %lu\n",
					pointer_to_read, bufferStatic, (unsigned long) sizeof(bufferStatic));
				continue_reading = 0;
			}
		}
		printf("\n\n GPX output :\n\n%s\n", bufferGpx);
	}
	return 0;
}

