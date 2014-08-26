#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "nmea/nmea.h"

int main()
{
        char    buf[256];
        FILE    *f;
        int     i = 0;

        nmeaINFO info;
        nmeaPARSER p;

        nmea_zero_INFO(&info);
        nmea_parser_init(&p);

        f = fopen("/dev/ttyUSB0", "r");

        while (1) {
                while (!((buf[i-2] == '\r') && (buf[i-1] == '\n'))) {
                        fread(&buf[i++], 1, 1, f);
                }
                buf[i] = '\0';
                i = 0;
                nmea_parse(&p, buf, strlen(buf), &info);
                printf("UTC      : %d:%d:%d\n", info.utc.hour,
                                              info.utc.min,
                                              info.utc.sec);
                printf("Latitude : %f\n", info.lat);
                printf("Longitude: %f\n", info.lon);
                printf("Elevation: %f\n", info.elv);
                printf("Speed    : %f\n", info.speed);
                printf("Direction: %f\n", info.direction);
                printf("\n");
        }

        fclose(f);

        return 0;
}
