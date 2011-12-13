// IO pin functions

#ifndef __IO_PINS_H
#define __IO_PINS_H

// Definitions

#define PAYLOAD_TX_ENABLE		0x02	// PC1
#define STATUS_LED_PORT			PORTD
#define STATUS_LED					0x80	// PD7

// Public globals

// Function prototypes

// extern void payload_tx_enable(boolean_t want_enable) already defined by mod/module.h but must be implemented locally
void io_pins_setup();
void toggle_status_led();
void status_led_on();
void status_led_off();

#endif // __IO_PINS_H
