// Timer functions

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <mod/base.h>
#include "base.h"
//#include <mod/comms.h>
#include "comms.h"
#include "io_pins.h"
#include "adc.h"

#include "timer.h"

ISR(TIMER1_COMPA_vect)
{
	adc_update();
}

void timer1_setup()
{
	TCCR1A = (0 << WGM11) | (0 << WGM10); // set WGM1 to 4, this clears timer counter on OCR1A
	TCCR1B = (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10); // set rest of WGM1 and set CS1 to 3 (64 prescaler)
	OCR1A = (uint16_t) 249; // want 1 ms timer, which is 1000 Hz. Must half this, since f_oc1a period is TWICE timer length, to that's 500 Hz. Therefore: i.e. (f_clk / (2 * prescaler * f_oc1a)) - 1 = (16000000 / (2 * 64 * 500)) - 1 = 249
	TIMSK1 = (1 << OCIE1A); // enabled interrupt on OCR1A match (TIMER1_COMPA_vect)
}

