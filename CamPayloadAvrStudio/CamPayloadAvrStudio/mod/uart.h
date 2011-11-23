// UART (serial) functions

#ifndef __UART_H
#define __UART_H

// Definitions

// Public globals

extern volatile boolean_t uart0_tx_idle;

// Function prototypes

void uart0_setup();

#endif // __UART_H
