/*
 * i2c.c
 */

#include "i2c.h"
#include "msp430.h"

#define I2C_PIN_SDA BIT2
#define I2C_PIN_SCL BIT3

void i2c_init() {
    P1SEL0 |= I2C_PIN_SDA | I2C_PIN_SCL;

    UCB0CTL1 |= UCSWRST; // reset
    UCB0CTLW0 |= UCMST | UCMODE_3; // Master, i2c
    UCB0BRW = 20; // 100 kHz
    UCB0CTL1 &= ~UCSWRST; // start
}

unsigned char i2c_write(unsigned char address, unsigned char length, unsigned char * txData, unsigned char stop) {
    UCB0I2CSA = address;

    UCB0IFG &= ~UCTXIFG0;
    UCB0CTLW0 |= UCTR | UCTXSTT; // tx, start-condition

    unsigned char i = 0;
    for (; i < length; i++) {
        while (!(UCB0IFG & UCTXIFG0)) {
            if (UCB0IFG & UCNACKIFG) {
                UCB0IFG &= ~UCNACKIFG;
                UCB0CTLW0 |= UCTXSTP;
                while(!(UCB0CTLW0 & UCTXSTP));
                return 0;
            }
        }
        UCB0TXBUF = txData[i];
    }
    while (!(UCB0IFG & UCTXIFG0));
    if (stop) {
        UCB0CTLW0 |= UCTXSTP;
        while(!(UCB0CTLW0 & UCTXSTP));
    } else {
        UCB0IFG &= ~UCTXIFG0;
        while (!(UCB0IFG & UCTXIFG0));
    }
    if (UCB0IFG & UCNACKIFG) {
        UCB0IFG &= ~UCNACKIFG;
        UCB0CTLW0 |= UCTXSTP;
        while(!(UCB0CTLW0 & UCTXSTP));
        return 0;
    }
    return 1;
}

unsigned char i2c_read(unsigned char address, unsigned char length, unsigned char * rxData) {
    UCB0I2CSA = address;

    UCB0IFG &= ~UCRXIFG;
    UCB0CTLW0 &= ~UCTR; // rx
    UCB0CTLW0 |= UCTXSTT; // start-condition
    unsigned char i = 0;
    for (; i < length - 1; i++) {
        while (!(UCB0IFG & UCRXIFG)) {
            if (UCB0IFG & UCNACKIFG) {
                UCB0IFG &= ~UCNACKIFG;
                UCB0CTLW0 |= UCTXSTP;
                while(!(UCB0CTLW0 & UCTXSTP));
                return 0;
            }
        }
        rxData[i] = UCB0RXBUF;
    }
    UCB0CTLW0 |= UCTXSTP;
    while (!(UCB0IFG & UCRXIFG));
    rxData[length-1] = UCB0RXBUF;
    while(!(UCB0CTLW0 & UCTXSTP));
    return 1;
}
