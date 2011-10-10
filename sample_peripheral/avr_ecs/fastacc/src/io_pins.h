// IO pin functions

#ifndef __IO_PINS_H
#define __IO_PINS_H

// Definitions

#define PAYLOAD_TX_ENABLE		0x01	// PC0
#define STATUS_LED					0x02	// PC1
#define nSPI_CS_ADC					0x03	// PC2

// Public globals

// Function prototypes

// extern void payload_tx_enable(boolean_t want_enable) already defined by mod/module.h but must be implemented locally
void io_pins_setup();
void toggle_status_led();
void status_led_on();
void status_led_off();

#endif // __IO_PINS_H