#include <avr/io.h>
#include <avr/interrupt.h>
//#include <mod/base.h>
#include "base.h"
//#include <mod/module.h>
#include "module.h"
//#include <mod/comms.h>
#include "comms.h"
#include "io_pins.h"
#include "adc.h"
#include "timer.h"
#include "peak_test.h"

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
		peak_test_send_int();
		//send_text("Hello World!");
		if (adc_z_want_update)
			adc_buffer_update();
	}

	return (0);
}
