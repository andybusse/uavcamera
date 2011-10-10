// Packet scan functionality

#include <avr/io.h>
#include <mod/base.h>
#include <mod/comms.h>
#include <mod/module.h>
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
				send_text("%d", adc_z_new);
				break;
		}
	}
}

void packet_tx_request()
{
	uint8_t payload[35];
	uint8_t count;

	PORTC ^= STATUS_LED;
	adc_pack_buffer();
	
	// create output packet
	
	payload[0] = CLASS_PAYLOAD | CLASS_FLAG_HAS_INDEX | CLASS_FLAG_IS_SET; // add indexed and SET flag
	payload[1] = module_id;
	payload[2] = CLASS_PAYLOAD_MEM_BYTES | CLASS_ITEM_FLAG_HAS_INDEX; // add indexed flag
	payload[3] = 0; // memory address 0
	payload[4] = 30; // 30 bytes of data
	
	for (count = 0; count < 30; count++)
		payload[count + 5] = adc_z_buffer_packed[count];
	
	send_packet(COMMS_START_BYTE_BASE | COMMS_START_SINGLE_CLASS, payload, 35);
}
