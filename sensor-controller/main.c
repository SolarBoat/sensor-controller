/**
 * main.c
 */

#include <msp430.h>
#include "motor.h"
//#include "temp.h"
#include "pc.h"
#include "uart.h"
#include "gps.h"
#include "scComInterface.h"
#include "i2c.h"

#define PIN_DEBUG BIT1

void init_system_clock();

int temp1, temp2, temp3, temp4;

SCDataFrame dataFrame;
unsigned char result = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= PIN_DEBUG;
	P1OUT |= PIN_DEBUG;

	PM5CTL0 &= ~LOCKLPM5;

	__enable_interrupt();

	init_system_clock();

	motor_init();
	//temp_init();
	pc_init();
	gps_init();
	i2c_init();

	unsigned int count = 0;
	while(1) {
	    uart_tick();
	    __delay_cycles(10000); // 5 ms
	    if (count == 200) {
	        count = 0;
	        dataFrame.frameID = SC_FULL_DATA_FRAME_ID;
	        dataFrame.full.hasGPSfix = hasGPSfix;
	        dataFrame.full.latitude = latitude;
	        dataFrame.full.buckPWM = buckPWM;
	        dataFrame.full.cellVoltage1 = cellVoltage1;
	        dataFrame.full.cellVoltage2 = cellVoltage2;
	        dataFrame.full.cellVoltage3 = cellVoltage3;
	        dataFrame.full.cellVoltage4 = cellVoltage4;
	        dataFrame.full.solarVoltageN = solarVoltageN;
	        dataFrame.full.solarVoltageP = solarVoltageP;
	        dataFrame.full.motorCurrent1 = motorCurrent1;
	        dataFrame.full.motorCurrent2 = motorCurrent2;
	        dataFrame.full.buckCurrent = buckCurrent;
	        dataFrame.full.systemCurrent = systemCurrent;
	        result = i2c_write(BORON_I2C_ADDRESS, SC_FULL_DATA_FRAME_LENGTH, dataFrame.data, 1);
	    }
	    count++;
	}
}

void init_system_clock(void) {
    __bis_SR_register(SCG0);                // disable FLL
    CSCTL3 |= SELREF__REFOCLK;              // Set REFO as FLL reference source
    CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM_3 | DCORSEL_1;// DCOFTRIM=3, DCO Range = 2MHz
    CSCTL2 = FLLD_0 + 60;                  // DCODIV = 2MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                // enable FLL
}
