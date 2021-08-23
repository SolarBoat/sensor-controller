/*
 * uart.h
 */

#ifndef UART_H_
#define UART_H_

#define UART0 0
#define UART1 1

void uart_init(unsigned char index, unsigned char* buffer, unsigned char bufferLength, void (*timeoutCallback)());

// needs to be called every 5 to 200 ms
void uart_tick();


#endif /* UART_H_ */
