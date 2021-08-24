/*
 * uart.c
 */

#include <msp430.h>
#include "uart.h"

#define UART0_PIN_RX BIT6
#define UART0_PIN_TX BIT7
#define UART1_PIN_RX BIT2
#define UART1_PIN_TX BIT3

#define UART_TIMEOUT_TICKS 2

unsigned char* uart0Buffer;
unsigned char* uart1Buffer;
unsigned char uart0BufferLength;
unsigned char uart1BufferLength;
volatile unsigned char uart0BufferIndex = 0;
volatile unsigned char uart1BufferIndex = 0;
volatile unsigned char uart0IdleTicks = 0;
volatile unsigned char uart1IdleTicks = 0;
void (*uart0TimeoutCallback)(unsigned char length);
void (*uart1TimeoutCallback)(unsigned char length);

void uart_init(unsigned char index, unsigned char* buffer, unsigned char bufferLength, void (*timeoutCallback)(unsigned char length)) {
    // UART 0
    if (index == UART0) {
        uart0Buffer = buffer;
        uart0BufferLength = bufferLength;
        uart0TimeoutCallback = timeoutCallback;
        UCA0CTLW0 |= UCSSEL_2; // SMCLK
        UCA0BRW = 13; // 9600 baud
        UCA0MCTLW |= UCOS16 | UCBRF_0 | UCBRS; // 9600 baud
        UCA0MCTLW_H = 0x49; // 9600 baud
        P1SEL0 |= UART0_PIN_RX | UART0_PIN_TX;
        UCA0CTLW0 &= ~UCSWRST; // start
        UCA0IE |= UCRXIE; // rx interrupt enable
    }

    // UART 1
    if (index == UART1) {
        uart1Buffer = buffer;
        uart1BufferLength = bufferLength;
        uart1TimeoutCallback = timeoutCallback;
        UCA1CTLW0 |= UCSSEL_2; // SMCLK
        UCA1BRW = 13; // 9600 baud
        UCA1MCTLW |= UCOS16 | UCBRF_0 | UCBRS; // 9600 baud
        UCA1MCTLW_H = 0x49; // 9600 baud
        P4SEL0 |= UART1_PIN_RX | UART1_PIN_TX;
        UCA1CTLW0 &= ~UCSWRST; // start
        UCA1IE |= UCRXIE; // rx interrupt enable
    }
}

void uart_tick() {
    if (uart0BufferIndex > 0) {
        if (uart0IdleTicks >= UART_TIMEOUT_TICKS) {
            (*uart0TimeoutCallback)(uart0BufferIndex);
            uart0BufferIndex = 0;
            uart0IdleTicks = 0;
        }
        uart0IdleTicks++;
    }
    if (uart1BufferIndex > 0) {
        if (uart1IdleTicks >= UART_TIMEOUT_TICKS) {
            (*uart1TimeoutCallback)(uart1BufferIndex);
            uart1BufferIndex = 0;
            uart1IdleTicks = 0;
        }
        uart1IdleTicks++;
    }
}

void uart_write(unsigned char index, unsigned char *data, unsigned char length) {
    if (index == UART0) {
        while(UCA0STATW & UCBUSY);
        unsigned char i = 0;
        for (; i < length; i++) {
            while(!(UCA0IFG & UCTXIFG));
            UCA0TXBUF = data[i];
        }
    }
    if (index == UART1) {
        while(UCA1STATW & UCBUSY);
        unsigned char i = 0;
        for (; i < length; i++) {
            while(!(UCA1IFG & UCTXIFG));
            UCA1TXBUF = data[i];
        }
    }
}


#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR (void) {
    if (uart0BufferIndex < uart0BufferLength) {
        uart0Buffer[uart0BufferIndex] = UCA0RXBUF;
        uart0IdleTicks = 0;
        uart0BufferIndex++;
    }
    UCA0IFG &= ~UCRXIFG;
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR (void) {
    if (uart1BufferIndex < uart1BufferLength) {
        uart1Buffer[uart1BufferIndex] = UCA1RXBUF;
        uart1IdleTicks = 0;
        uart1BufferIndex++;
    }
    UCA1IFG &= ~UCRXIFG;
}
