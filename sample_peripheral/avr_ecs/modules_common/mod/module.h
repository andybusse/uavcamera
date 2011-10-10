// Standard payload module functionality

#ifndef __MODULE_H
#define __MODULE_H

// Definitions

#define MODULE_BROADCAST_ID							127
#define MODULE_INITIAL_ID								127
#define MODULE_EEPROM_ADDR_MODULE_ID		0

#define MODULE_PACKET_CLASS_CHANGE_MODULE_ID		255

// Public globals

// Function prototypes

void module_setup();
void module_packet_scan(uint8_t *data, uint8_t length);

// Function prototypes to be supplied externally

extern void payload_tx_enable(boolean_t want_enable);
extern void packet_scan(uint8_t *data, uint8_t length);
extern void packet_tx_request();

#endif // __MODULE_H
