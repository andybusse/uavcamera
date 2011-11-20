#include <avr/io.h>
#include <avr/interrupt.h>
#include "mod/base.h"
#include "mod/module.h"
#include "mod/comms.h"
#include "io_pins.h"


/*
int main()
{
	io_pins_setup();
	module_setup();

	DDRA = 0xFF;
	PORTA = 0x00;
	
	// enable global interrupts
	sei();
	
	while (1)
	{
		comms_update();
	}

	return (0);
}
*/
