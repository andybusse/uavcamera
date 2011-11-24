
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
}


void send_IMAGE_DATA_packet() {

	// wait until we can send a packet

	wait_for_send_message();


	uint8_t currPacketLength;

	if((imageSendState.currentPacket == imageSendState.numPackets - 1) && (imageSendState.imageFileSize % IMAGE_PACKET_SIZE != 0)) {
		currPacketLength = imageSendState.imageFileSize % IMAGE_PACKET_SIZE;
		//DLOG("Last packet\n\r");
	} else {
		currPacketLength = IMAGE_PACKET_SIZE;
	}

	currPacketLength += 3;

	//if(currPacketNum > 1140) {
	//	DLOG("#");
	//	DLOG(currPacketNum);
	//	DLOG(": ");
	//}

	messageToSend[0] = currPacketLength - 1;
	messageToSend[1] = (uint8_t)imageSendState.currentPacket;
	messageToSend[2] = (uint8_t)(imageSendState.currentPacket >> 8);

	messageToSendLength = currPacketLength;


	for(int currByteNum = 3; currByteNum < currPacketLength; currByteNum++) {
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


	flag_want_to_send_message();
	imageSendState.currentPacket++;

}
