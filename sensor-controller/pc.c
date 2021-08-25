/*
 * pc.c
 */

#include "pc.h"
#include "uart.h"
#include "pcComInterface.h"

PCDataFrame ps_dataFrame;

void pc_handleFrame(unsigned char length);

void pc_init() {
    uart_init(UART0, ps_dataFrame.data, PC_DATA_FRAME_LENGTH, &pc_handleFrame);
}

void pc_handleFrame(unsigned char length) {
    if (ps_dataFrame.frameID == PC_DATA_FRAME_ID) {
        buckPWM = ps_dataFrame.buckPWM;
        cellVoltage1 = ps_dataFrame.cellVoltage1;
        cellVoltage2 = ps_dataFrame.cellVoltage2;
        cellVoltage3 = ps_dataFrame.cellVoltage3;
        cellVoltage4 = ps_dataFrame.cellVoltage4;
        solarVoltageN = ps_dataFrame.solarVoltageN;
        solarVoltageP = ps_dataFrame.solarVoltageP;
        motorCurrent1 = ps_dataFrame.motorCurrent1;
        motorCurrent2 = ps_dataFrame.motorCurrent2;
        buckCurrent = ps_dataFrame.buckCurrent;
        systemCurrent = ps_dataFrame.systemCurrent;
    }
}
