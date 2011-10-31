// ADC functions

#ifndef __ADC_H
#define __ADC_H

// Definitions

// for writing to the autopilot's payload memory
#define CLASS_PAYLOAD               7
#define CLASS_PAYLOAD_MEM_BYTES     10

// Public globals

extern volatile boolean_t adc_z_want_update;
extern uint8_t adc_z_buffer_packed[31]; // includes first size byte
extern volatile uint16_t adc_z_new;

// Function prototypes

void adc_update();
void adc_setup();
void adc_buffer_update();
void adc_pack_buffer();

#endif // __ADC_H
