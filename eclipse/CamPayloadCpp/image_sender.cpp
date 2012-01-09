/* Copyright 2011 Michael Hodgson, Piyabhum Sornpaisarn, Andrew Busse, John Charlesworth, Paramithi Svastisinha

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

#include "image_sender.h"
#include "payload_base.h"
#include "packet_scan.h"

image_send_state_t imageSendState;

void init_image_sender() {
	imageSendState.currentPacket = 0;
	imageSendState.imageFileSize = 0;
	imageSendState.numPackets = 0;
	imageSendState.sendingImage = false;
	imageSendState.imageFileSize = 0;
	imageSendState.waitingForAck = false;
	imageSendState.numRetries = 0;
}


void send_IMAGE_DATA_packet() {

	// wait until we can send a packet

	wait_for_send_message();


	uint8_t currPacketLength;

	if((imageSendState.currentPacket == imageSendState.numPackets - 1) && (imageSendState.imageFileSize % IMAGE_PACKET_SIZE != 0)) {
		currPacketLength = imageSendState.imageFileSize % IMAGE_PACKET_SIZE;
	} else {
		currPacketLength = IMAGE_PACKET_SIZE;
	}

	if(imageSendState.currentPacket == imageSendState.numPackets - 1) {
		DLOG("Last packet\n\r");
	}


	currPacketLength += 4; // add the length required for the packet size, message id and the current packet number data

	//if(currPacketNum > 1140) {
	//	DLOG("#");
	//	DLOG(currPacketNum);
	//	DLOG(": ");
	//}

	messageToSend[0] = currPacketLength - 1;
	messageToSend[1] = MID_IMAGE_DATA;
	messageToSend[2] = (uint8_t)imageSendState.currentPacket;
	messageToSend[3] = (uint8_t)(imageSendState.currentPacket >> 8);

	messageToSendLength = currPacketLength;


	for(int currByteNum = 4; currByteNum < currPacketLength; currByteNum++) {
		uint8_t currByte = (uint8_t)sdFile.read();
		messageToSend[currByteNum] = currByte;
		//if(currPacketNum > 1140) {
		//	DLOG((int)imagePacketToSend[currByteNum]);
		//	DLOG(" ");
		//}
		if(messageToSend[currByte] == -1) {
			DLOG("ERROR: Trying to read over the end of the file!\n\r");
		}
	}

	//for(int currByteNum = 0; currByteNum < currPacketLength; currByteNum++) {
	//	if(currPacketNum > 1140) {
	//		DLOG((int)imagePacketToSend[currByteNum]);
	//		DLOG(" ");
//			}
//		}

//		if(currPacketNum > 1140) {
//			DLOG("\n\r");
//		}

	//DLOG("Waiting for tx token\n\r");
	//recTxToken = false;
	//while(!recTxToken)
	//	comms_update();
	//DLOG("Got tx token\n\r");
	//send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 0, imagePacketToSend, currPacketLength);
	//DLOG("Waiting for all data to be transmitted\n\r");
	//DLOG("Class item sent\n\r");
	//while(payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail); // spin until all the data has been transmitted
	//DLOG("Transmitted\n\r");


	//flag_want_to_send_message();
	send_message(false, NUM_ACKFAIL_RETRIES);
	imageSendState.currentPacket++;

}
