// Packet scan functionality

#include <avr/io.h>
#include "mod/base.h"
#include "mod/comms.h"
#include "mod/module.h"
#include "io_pins.h"

#include <stdbool.h>

#include "packet_scan.h"

#include "payload_base.h"

#include "camera.h"


uint8_t messageToSend[MAX_MESSAGE_LENGTH];
uint8_t messageToSendLength;
bool messageSendPending = false;

void packet_scan(uint8_t *data, uint8_t length)
{
	if (length > 0)
	{
		DLOG("Got message, length: %u", data[0], length);
		switch (data[0])
		{
			case MID_TAKE_PICTURE:
				DLOG("TAKE_PICTURE message received");
				int imageID;
				imageID = take_picture();
				if(imageID >= 0) {
					ILOG("Picture taken with image ID: %d", imageID);
					send_PICTURE_TAKEN_message(imageID);
					DLOG("Sent picture taken message.");
				} else {
					ILOG("Picture taking failed!");
				}
				break;
			default: // not a valid message
				DLOG("Invalid message, length: %u", data[0], length);
				break;
		}
	}
}

void packet_tx_request()
{
	/*uint8_t payload[35];
	uint8_t count;

	
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
	*/

	//recTxToken = true;

	//testDataToSend[1] = testInt;
	//send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 0, testDataToSend, 2);

	//testInt++;

	PORTC ^= STATUS_LED;

	if(messageSendPending) {
		send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 0, messageToSend, messageToSendLength);
		//send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 2, data, 4);
		messageSendPending = false;
	}

}

void send_PICTURE_TAKEN_message(uint16_t imageID) {
	messageToSend[0] = 3; /* length */
	messageToSend[1] = MID_PICTURE_TAKEN; /* message ID */
	messageToSend[2] = (uint8_t)(imageID >> 8); /* image ID MSB */
	messageToSend[3] = (uint8_t)imageID; /* image ID LSB */
	messageToSendLength = 4;
	send_message();
}

void send_IMAGE_DOWNLOAD_INFO_message(uint16_t numPackets) {
	messageToSend[0] = 3;
	messageToSend[1] = MID_IMAGE_DOWNLOAD_INFO;
	messageToSend[2] = (uint8_t)(numPackets >> 8);
	messageToSend[3] = (uint8_t)numPackets;
	messageToSendLength = 4;
	send_message();
}

void send_message() {
	while(messageSendPending); // wait until the current pending messageToSend has been sent

	messageSendPending = true;
}
