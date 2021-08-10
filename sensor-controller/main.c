/**
 * main.c
 */

#include <msp430.h>
#include "motor.h"

#define PIN_DEBUG BIT1

void init_system_clock();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= PIN_DEBUG;
	P1OUT |= PIN_DEBUG;

	PM5CTL0 &= ~LOCKLPM5;

	__enable_interrupt();

	init_system_clock();

	motor_init();
	motor_enable();

	while(1) {
	    P1OUT ^= PIN_DEBUG;
	    motor_enable();
	    motor_set_speed(100, 100);
	    __delay_cycles(2000000);
	    motor_disable();
	    __delay_cycles(2000000);
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
