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
//#include "timer.h"
#include "packet_scan.h"

uint8_t my_data[8]={0x07,0x42,0x42,0x42,0x42,0x42,0x42,0x42};

int main()
{
	
	io_pins_setup();
	module_setup();
	//adc_setup();
	//timer1_setup();
	
	// enable global interrupts
	sei();
	
	DDRB = 0xFF;
	PORTB = 0xFF;
	
	//DDRB |= 0x02;
	//PORTB &= ~(0x02);
	send_set_class_item(2,3,my_data,8);
	while (1)
	{
		PORTB = 0x00;
		comms_update();

		//send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 0, adc_z_buffer_packed, 31);
		PORTB = 0xFF;
	}


	

	return (0);
}
