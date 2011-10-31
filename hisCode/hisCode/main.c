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

int main()
{
	io_pins_setup();
	module_setup();
	adc_setup();
	timer1_setup();
	
	// enable global interrupts
	sei();
	uint8_t my_data[8]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};
	while (1)
	{
		comms_update();
		send_set_class_item(2,3,my_data,8);
		//send_text("Hello World!");
		if (adc_z_want_update)
			adc_buffer_update();
	}

	return (0);
}
