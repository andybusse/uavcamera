#include <avr/io.h>
#include <avr/interrupt.h>
#include <mod/base.h>
#include <mod/module.h>
#include <mod/comms.h>
#include "io_pins.h"
#include "adc.h"
#include "timer.h"

int main()
{
	io_pins_setup();
	module_setup();
	adc_setup();
	timer1_setup();
	
	// enable global interrupts
	sei();
	
	while (1)
	{
		comms_update();
		
		if (adc_z_want_update)
			adc_buffer_update();
	}

	return (0);
}
