/* Copyright 2011 Michael Hodgson, Piyabhum Sornpaisarn, Andrew Busse, John Charlesworth, Paramithi Svastisinha, Matt Bennett

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
#include "mod/base.h"
#include "mod/comms.h"
#include "mod/module.h"
#include "io_pins.h"
#include <avr/delay.h>

#include <stdbool.h>

#include "packet_scan.h"

#include "payload_base.h"

#include "camera.h"

#include "SD/SD.h"

#include <stdio.h>

#include "image_sender.h"

uint8_t messageToSend[MAX_MESSAGE_LENGTH];
uint8_t messageToSendLength;
volatile bool messageStartSendPending = false;
volatile bool ackReceived = false;
volatile uint8_t ackCommandID = 120;

volatile bool nakReceived = false;
volatile uint8_t nakCommandID = 120;

volatile uint16_t numTokens;

volatile bool onlyGetAcks = false;

void packet_scan(uint8_t *data, uint8_t length)
{
	if (length > 0)
	{
		//DLOG("Got message, length: ");
		//DLOG((int)length);
		//DLOG("\n\r");
		switch (data[0])
		{
			case MID_TAKE_PICTURE:
				if(onlyGetAcks == false) {
					if(length == 1) {
						send_ACK_message(MID_TAKE_PICTURE);
						DLOG("TAKE_PICTURE message received\n\r");
						if(imageSendState.sendingImage == false) {
							int takePictureImageID;
							takePictureImageID = take_picture();
							if(takePictureImageID >= 0) {
								ILOG("Picture taken with image ID: ");
								ILOG(takePictureImageID);
								ILOG("\n\r");
								send_PICTURE_TAKEN_message(takePictureImageID);
								DLOG("Sent picture taken message.\n\r");
							} else {
								ILOG("Picture taking failed!\n\r");
							}
						}
					} else {
						DLOG("Invalid TAKE_PICTURE message received\n\r");
					}
				}
				break;

			case MID_IMAGE_DOWNLOAD_REQUEST:
				if(onlyGetAcks == false) {
					if(length == 3) {
						send_ACK_message(MID_IMAGE_DOWNLOAD_REQUEST);
						uint16_t downloadRequestImageID;
						downloadRequestImageID = (uint16_t)data[1] + (uint16_t)(data[2] << 8);
						DLOG("IMAGE_DOWNLOAD_REQUEST message received with image ID ");
						DLOG(downloadRequestImageID);
						DLOG("\n\r");
						// we need to see if an image with the id specified actually exists
						char imgFileName[MAX_FILE_NAME_LENGTH];
						snprintf(imgFileName, sizeof(imgFileName), "%s%i%s", filePrefix, downloadRequestImageID, fileExt);
						DLOG(imgFileName);
						DLOG("\n\r");


						if(SD.exists(imgFileName)) {
							sdFile.close(); // ensure old file is closed
							sdFile = SD.open(imgFileName);
							if(sdFile == false) {
								ILOG("ERROR: JPEG file failed to open!\n\r");
								imageSendState.sendingImage = false;
								send_IMAGE_DOWNLOAD_INFO_message(0);
							} else {
								init_image_sender();
								imageSendState.currentPacket = 0;
								uint32_t size = sdFile.size();
								imageSendState.imageFileSize = size;
								DLOG("imageSendState.imageFileSize: ");
								DLOG(imageSendState.imageFileSize);
								DLOG("\n\r");
								if(imageSendState.imageFileSize > 0) {
									imageSendState.numPackets = imageSendState.imageFileSize / IMAGE_PACKET_SIZE;
									if((imageSendState.imageFileSize % IMAGE_PACKET_SIZE) != 0)
										imageSendState.numPackets += 1;

									DLOG("Number of packets: ");
									DLOG(imageSendState.numPackets);
									DLOG("\n\r");
									imageSendState.imageFile = sdFile;
									imageSendState.sendingImage = true;
									send_IMAGE_DOWNLOAD_INFO_message(imageSendState.numPackets);
								} else {
									ILOG("ERROR: Image file size was zero.");
									imageSendState.sendingImage = false;
									send_IMAGE_DOWNLOAD_INFO_message(0);
									sdFile.close();
								}
							}

						} else {
							ILOG("ERROR: JPEG file does not exist!\n\r");
							// send image download message with number of packets = 0 to represent the fact that the file does not exist
							send_IMAGE_DOWNLOAD_INFO_message(0);
						}

					} else {
						DLOG("Invalid IMAGE_DOWNLOAD_REQUEST message received\n\r");
					}
				}
				break;

			case MID_CONFIGURE_CAMERA:
				if(onlyGetAcks == false) {
					if(length == 4) {
						send_ACK_message(MID_CONFIGURE_CAMERA);
						colourType = data[1];
						rawRes = data[2];
						jpegRes = data[3];
						DLOG("Configuring camera. Colour Type: ");
						DLOG((int)colourType);
						DLOG(" Raw Resolution: ");
						DLOG((int)rawRes);
						DLOG(" JPEG Resolution ");
						DLOG((int)jpegRes);
						DLOG("\n\r");
					} else {
						DLOG("Invalid CONFIGURE_CAMERA message received\n\r");
					}
				}
				break;

			case MID_CANCEL_DOWNLOAD:
				send_ACK_message(MID_CANCEL_DOWNLOAD);
				DLOG("CANCEL_DOWNLOAD message received\n\r");
				imageSendState.sendingImage = false;
				sdFile.close();
				break;

			case MID_ACK:
				//DLOG("Got Ack, ID: ");
				//DLOG((int)data[1]);
				//DLOG("\n\r");
				ackReceived = true;
				ackCommandID = data[1];
				break;

			case MID_NAK:
				nakReceived = true;
				nakCommandID = data[1];
				break;

			default: // not a valid message
				DLOG("Invalid message, length: ");
				DLOG(length);
				DLOG("\n\r");
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

	numTokens++;

	STATUS_LED_PORT ^= STATUS_LED;
	/*if(messageStartSendPending)
		PORTC |= STATUS_LED;
	else
		PORTC &= ~STATUS_LED;*/
	if(messageStartSendPending) {
		send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 0, messageToSend, messageToSendLength);
		//send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 2, data, 4);
		messageStartSendPending = false;
	}

}

bool send_PICTURE_TAKEN_message(uint16_t imageID) {
	//for (int i = 0; i < NUM_ACKFAIL_RETRIES; i++) {
		wait_for_send_message();
	//	ackReceived = false;
		messageToSend[0] = 3; /* length */
		messageToSend[1] = MID_PICTURE_TAKEN; /* message ID */
		messageToSend[2] = (uint8_t)imageID; /* image ID LSB */
		messageToSend[3] = (uint8_t)(imageID >> 8); /* image ID MSB */
		messageToSendLength = 4;
		//flag_want_to_send_message();
		//wait_for_send_message();

	//	if(wait_for_ACK(MID_PICTURE_TAKEN))
		//	return true;
	//}
	//DLOG("ACK failed\n\r");
	//return false;
	return send_message(true, NUM_ACKFAIL_RETRIES);
}

bool send_IMAGE_DOWNLOAD_INFO_message(uint16_t numPackets) {
	//for (int i = 0; i < NUM_ACKFAIL_RETRIES; i++) {
		wait_for_send_message();
		//ackReceived = false;
		messageToSend[0] = 3;
		messageToSend[1] = MID_IMAGE_DOWNLOAD_INFO;
		messageToSend[2] = (uint8_t)numPackets;
		messageToSend[3] = (uint8_t)(numPackets >> 8);
		messageToSendLength = 4;
		//flag_want_to_send_message();
		//wait_for_send_message();

		//if(wait_for_ACK(MID_IMAGE_DOWNLOAD_INFO))
			//return true;
	//}

	//DLOG("ACK failed\n\r");
	//return false;
    return send_message(true, NUM_ACKFAIL_RETRIES);
}

void send_ACK_message(uint8_t commandIDToAck) {
	wait_for_send_message();
	messageToSend[0] = 3;
	messageToSend[1] = MID_ACK;
	messageToSend[2] = commandIDToAck;
	messageToSendLength = 3;
	send_message(false, 1);
	//	send_message();
}

bool send_message(bool needsAck, uint8_t numRetries) {
	if(needsAck) {
		for (int i = 0; i < numRetries; i++) {
			ackReceived = false;
			nakReceived = false;
			flag_want_to_send_message();

			wait_for_send_message();

			if(wait_for_ACK(messageToSend[1]))
				return true;

			//DLOG("Retrying send of command ");
			//DLOG((int)messageToSend[1]);
			//DLOG("\n\r");
		}
		DLOG("Sending message failed, no ACKs found \n\r");
		return false;
	} else {
		flag_want_to_send_message();
		return true;
	}
}

bool wait_for_ACK(uint8_t commandID) {
	numTokens = 0; // reset the number of tokens passed to 0
	while(numTokens < ACK_WAIT_TOKENS) {
		if (ackReceived == true && ackCommandID == commandID) {
			//DLOG("ACK found and correct.\n\r");
			return true;
		} else if(nakReceived && nakCommandID == commandID) {
			return false;
		}
		onlyGetAcks = true;
		comms_update();
		onlyGetAcks = false;
	}
	return false;
}

void wait_for_send_message() {
	while(messageStartSendPending) { // wait until the current pending messageToSend has been put into the fifo
		/* NB: Had an INCREDIBLY annoying bug here where this loop was not exiting despite messageStartSendPending being set to false by an ISR.
		 * Turns out that something odd was happening, perhaps the compiler had optimised away this loop to while(1) since it thought messageStartPending
		 * could not be changed in an empty loop (perhaps it does not know about ISRs)
		 *
		 * The fix was to make messageStartSendPending volatile! */

		//DLOG("[");
		//DLOG(messageStartSendPending);
		//DLOG("]");
		//if(messageStartSendPending)
		//	PORTC |= STATUS_LED;
		//else
		//	PORTC &= ~STATUS_LED;
	}
	//while(payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail);
}

void flag_want_to_send_message() {
	messageStartSendPending = true; // flag that we want to send a message
}
