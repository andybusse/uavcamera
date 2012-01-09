/* Copyright 2011 Matt Bennett

    This file is part of uavcamera.

    uavcamera is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    uavcamera is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uavcamera.  If not, see <http://www.gnu.org/licenses/>.

*/

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
