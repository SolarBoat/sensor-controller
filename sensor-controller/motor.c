/*
 * motor.c
 */

#include <msp430.h>
#include "motor.h"

#define PIN_MOTOR_ENABLE BIT2
#define PIN_MOTOR_1 BIT0
#define PIN_MOTOR_2 BIT1

#define MOTOR_ENABLE_ALWAYS_ON true


#ifdef MOTOR_ENABLE_ALWAYS_ON
unsigned char motorTimerStopFlag;
#endif

void motor_init() {
    // IO
#ifdef MOTOR_ENABLE_ALWAYS_ON
    P2OUT |= PIN_MOTOR_ENABLE;
#else
    P2OUT &= ~PIN_MOTOR_ENABLE;
#endif
    P2DIR |= PIN_MOTOR_ENABLE;



    // Timer
    TB1CTL |= TBSSEL_2 | ID_1; // SMCLK, divide by 2
    TB1CCTL1 |= OUTMOD_7 | CLLD_1;
    TB1CCTL2 |= OUTMOD_7 | CLLD_1;
    TB1CCR0 = 20000; // 50 Hz
    motor_set_speed(0, 0);
}

void motor_enable() {
    motor_set_speed(0, 0);

    P2OUT |= PIN_MOTOR_ENABLE;
    P2OUT &= ~(PIN_MOTOR_1 | PIN_MOTOR_2);
    P2DIR |= PIN_MOTOR_1 | PIN_MOTOR_2;
    P2SEL0 |= PIN_MOTOR_1 | PIN_MOTOR_2;

#ifdef MOTOR_ENABLE_ALWAYS_ON
    TB1CCTL1 |= OUTMOD1; // reset/set
    TB1CCTL2 |= OUTMOD1; // reset/set
#endif

    TB1CTL |= MC_1; // start timer
}

void motor_disable() {
#ifndef MOTOR_ENABLE_ALWAYS_ON
    P2OUT &= ~PIN_MOTOR_ENABLE;

    P2DIR &= ~(PIN_MOTOR_1 | PIN_MOTOR_2);
    P2SEL0 &= ~(PIN_MOTOR_1 | PIN_MOTOR_2);
    TB1CTL &= ~MC_1; // stop timer
#else
    motor_set_speed(0, 0);
    motorTimerStopFlag = 0;
    TB1CCTL0 &= ~CCIFG; // clear interrupt flag
    TB1CCTL0 |= CCIE; // enable interrupt
#endif
}

void motor_set_speed(signed char left, signed char right) {
    TB1CCR1 = 1500 + ((int) left) * 5;
    TB1CCR2 = 1500 + ((int) right) * 5;
}

#ifdef MOTOR_ENABLE_ALWAYS_ON
#pragma vector=TIMER1_B0_VECTOR
__interrupt void TIMER1_B0_ISR (void) {
    if (motorTimerStopFlag) {
        TB1CCTL0 &= ~CCIE; // disable interrupt
        P2DIR &= ~(PIN_MOTOR_1 | PIN_MOTOR_2);
        P2SEL0 &= ~(PIN_MOTOR_1 | PIN_MOTOR_2);
        TB1CTL &= ~MC_1; // stop timer
    } else {
        motorTimerStopFlag = 1;
        TB1CCTL1 &= ~OUTMOD1; // reset
        TB1CCTL2 &= ~OUTMOD1; // reset
    }
}
#endif
