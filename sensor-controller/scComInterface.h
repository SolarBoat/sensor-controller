/*
 * scComInterface.h
 */

#include <stdint.h>


#ifndef SCCOMINTERFACE_H_
#define SCCOMINTERFACE_H_

#define BORON_I2C_ADDRESS 0x42

#define SC_FULL_DATA_FRAME_ID 0x01
#define SC_FREQUENT_DATA_FRAME_ID 0x02

#define SC_FULL_DATA_FRAME_LENGTH 32
#define SC_FREQUENT_DATA_FRAME_LENGTH 0

#pragma pack(push,1)
typedef union SCDataFrame {
    struct {
        uint8_t frameID;
        union {
            struct{
                //gps
                uint8_t hasGPSfix;
                float latitude;
                float longitude;
                // power controller
                uint16_t buckPWM;
                uint16_t cellVoltage1, cellVoltage2, cellVoltage3, cellVoltage4;
                uint16_t solarVoltageN, solarVoltageP;
                uint16_t buckCurrent;
                uint16_t systemCurrent;
                uint16_t motorCurrent1, motorCurrent2;
            } full;
        };
    };
    uint8_t data[SC_FREQUENT_DATA_FRAME_LENGTH];
} SCDataFrame;
#pragma pack(pop)

#endif /* SCCOMINTERFACE_H_ */
