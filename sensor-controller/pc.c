/*
 * pc.c
 */

#include "pc.h"
#include "uart.h"
#include "pcComInterface.h"

PCDataFrame dataFrame;

void handleFrame();

void pc_init() {
    uart_init(UART0, dataFrame.data, PC_DATA_FRAME_LENGTH, &handleFrame);
}

void handleFrame() {
    if (dataFrame.frameID == PC_DATA_FRAME_ID) {
        buckPWM = dataFrame.buckPWM;
        cellVoltage1 = dataFrame.cellVoltage1;
        cellVoltage2 = dataFrame.cellVoltage2;
        cellVoltage3 = dataFrame.cellVoltage3;
        cellVoltage4 = dataFrame.cellVoltage4;
        solarVoltageN = dataFrame.solarVoltageN;
        solarVoltageP = dataFrame.solarVoltageP;
        motorCurrent1 = dataFrame.motorCurrent1;
        motorCurrent2 = dataFrame.motorCurrent2;
        buckCurrent = dataFrame.buckCurrent;
        systemCurrent = dataFrame.systemCurrent;
    }
}
