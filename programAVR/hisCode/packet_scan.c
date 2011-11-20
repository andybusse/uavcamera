// Packet scan functionality

#include <avr/io.h>
//#include <mod/base.h>
#include "base.h"
//#include <mod/module.h>
#include "module.h"
//#include <mod/comms.h>
#include "comms.h"
#include "io_pins.h"
#include "adc.h"

#include "packet_scan.h"

void packet_scan(uint8_t *data, uint8_t length)
{
	if (length > 0)
	{
		switch (data[0])
		{
			case 0:
				send_text("hello");
				break;
		}
	}
}

void packet_tx_request()
{
	PORTC ^= STATUS_LED;
	//adc_pack_buffer();
	
	// send output packet
	//send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 0, adc_z_buffer_packed, 31);
}
