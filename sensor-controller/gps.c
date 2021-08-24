/*
 * gps.c
 */

#include "gps.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>

#define GPS_UART UART1
#define GPS_FRAME_BUFFER_SIZE 85

char gps_frameBuffer[GPS_FRAME_BUFFER_SIZE + 1];

void gps_handleFrame(unsigned char length);

void gps_init() {
    uart_init(GPS_UART, (unsigned char*)gps_frameBuffer, GPS_FRAME_BUFFER_SIZE, &gps_handleFrame);
    uart_write(GPS_UART, (unsigned char*)"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n", 51); // report less data
}

void gps_handleFrame(unsigned char length) {
    gps_frameBuffer[length] = 0;
    char *sentence = strstr(gps_frameBuffer, "$GPRMC");
    if (sentence != NULL) {
        char *parts[11];
        unsigned char i = 0;
        char *position = sentence;
        unsigned char complete = 1;
        for(; i < 11; i++) {
            position = strchr(position, ',');
            if (position == NULL) {
                complete = 0;
                break;
            }
            *position = 0;
            position++;
            parts[i] = position;
        }
        if (complete) {
            if (*parts[1] == 'A') {
                hasGPSfix = 1;
                float lat = (int)(*parts[2] - '0') * 10 + (*(parts[2] + 1) - '0');
                lat += strtof(parts[2] + 2, NULL) / 60.0;
                if (*parts[3] == 'S') {
                    lat = -lat;
                }
                float lon = (int)(*parts[4] - '0') * 100 + (*(parts[4] + 1) - '0') * 10 + (*(parts[4] + 2) - '0');
                lon += strtof(parts[4] + 3, NULL) / 60.0;
                if (*parts[5] == 'W') {
                    lon = -lon;
                }
                latitude = lat;
                longitude = lon;
            } else {
                hasGPSfix = 0;
            }
        }
    }
}
