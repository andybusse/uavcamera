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

#include "config.h"
#include "spi_debug.h"

#include <avr/io.h>
#include <avr/interrupt.h>
//extern "C" {
#include "mod/base.h"
#include "mod/module.h"
#include "mod/comms.h"
#include "io_pins.h"
//}


#include "camera.h"
#include <WProgram.h>

#include <HardwareSerial.h>

#include "mod/comms.h"
#include "payload_base.h"

#include "SD/SD.h"

#include "mod/uart.h"
#include "image_sender.h"
#include "packet_scan.h"

bool init_sd();



//uint8_t imagePacketToSend[IMAGE_PACKET_SIZE + 3];

int main()
{



	io_pins_setup();

	DDRD |= 0x80;
	PORTD |= 0x80;
	spiDebug.begin();
	module_setup();
	init_image_sender();


	DDRC = 0xFF;
	PORTC = 0x00;

	sei();

	ILOG("Payload starting...");
	ILOG("Initialising SD card...");

	init_sd();

	ILOG("Initialising camera...");

	init_cam();



	DLOG("Entering main loop...");
	while(1) {
		//STATUS_LED_PORT ^= 0x20;
		// main event loop

		/* comms update will process any commands and packet_scan in packet_scan.cpp
		 * will be called to handle any incoming data and messages
		 */
		comms_update();
		/* now the messages have been taken care of we need to process any flags the messages may have set
		 */


		if(imageSendState.sendingImage == true && imageSendState.waitingForAck == true) {
			if(ackReceived && ackCommandID == MID_IMAGE_DATA) {
				imageSendState.waitingForAck = false;
				ackReceived = false;
				//DLOG("Got IMAGE_DATA ACK.\n\r");
			}
		}

		if(imageSendState.sendingImage == true && imageSendState.waitingForAck == true) {

			//if(numTokens > ACK_WAIT_TOKENS || nakReceived == true) {
				if(wait_for_ACK(MID_IMAGE_DATA) == false) {
					if(imageSendState.numRetries > 40) {
						imageSendState.sendingImage = false;
						sdFile.close();
						DLOG("Aborting image send.\n\r");
					} else {
						// we have not got an ack so we need to resend
						if(imageSendState.numPackets < IMAGE_ACK_RESEND_SIZE)
							imageSendState.currentPacket = 0;
						else
							imageSendState.currentPacket = imageSendState.currentPacket - IMAGE_ACK_RESEND_SIZE;

						unsigned long byteNumber = imageSendState.currentPacket * IMAGE_PACKET_SIZE;
						sdFile.seek(byteNumber);
						imageSendState.waitingForAck = false;
						imageSendState.numRetries++;
						DLOG("Resending.\n\r");
					}
				}
			//}
		}

		if(imageSendState.sendingImage == true) {// && imageSendState.waitingForAck == false) {
			//if(imageSendState.currentPacket == 0)
				//DLOG("Starting to send image.\n\r");
			// we are in the middle of sending an image
			// so send the next packet

			if(imageSendState.currentPacket < imageSendState.numPackets) {
				//DLOG("Sending packet ");
				//DLOG(imageSendState.currentPacket);
				//DLOG("\n\r");
				send_IMAGE_DATA_packet();

				if(((imageSendState.currentPacket % IMAGE_ACK_RESEND_SIZE) == 0 && imageSendState.currentPacket != 0) || imageSendState.currentPacket == imageSendState.numPackets) {
					// wait for ack or nak
					imageSendState.waitingForAck = true;
					ackReceived = false;
					nakReceived = false;
					numTokens = 0;
				}
			} else {
				imageSendState.sendingImage = false;
				sdFile.close();
				DLOG("Finished sending image.\n\r");
			}
		}
	}
	return 0;
}

#if 0
int old_main() {
	//init_debug_spi();

	DDRA = 0xFF;

	spiDebug.begin();
	io_pins_setup();
	module_setup();

	init_sd();

	// enable global interrupts
	sei();
	
	// init our camera
	//init_cam();

	// open test1.jpg on the SD card and start reading bytes out of it
	DLOG("Opening jpeg file.\n\r");

	File testJPEGSD = SD.open("TEST1.JPG", FILE_READ);
	if(testJPEGSD == false) {
		DLOG("ERROR: Jpeg file failed to open.\n\r Quitting!\n\r");
		return -1;
	}
	DLOG("Jpeg file opened.\n\r");
	uint32_t testJPEGSDSize = testJPEGSD.size();
	DLOG("TestJPEGDSize: ");
	DLOG("%u", testJPEGSDSize);
	DLOG("\n\r");
	uint16_t numPackets = testJPEGSDSize / IMAGE_PACKET_SIZE;
	if((testJPEGSDSize % IMAGE_PACKET_SIZE) != 0)
		numPackets += 1;
	DLOG("numPackets: ");
	DLOG("%u", numPackets);
	DLOG("\n\r");

	// wait until we start getting tx tokens addressed to us
	DLOG("Waiting for tx token\n\r");
	//while (!recTxToken)
	//	comms_update();
	DLOG("Tx token received\n\r");

	delay(2000);

	DLOG("Starting send\n\r");
/*
	long byteNum = 0;
	int currByte = 0;
	int packetNum = -1;
	do {
		if((byteNum % 20) == 0) {
			packetNum++;
			if(packetNum > 1040) {
				DLOG("\n\r");
				DLOG(packetNum);
				DLOG("# ");
			}
		}

		currByte = testJPEGSD.read();
		if(packetNum > 1040) {
			DLOG(currByte);
			DLOG(" ");
		}
		byteNum++;

	} while(currByte != -1);
*/




	for(uint16_t currPacketNum = 0; currPacketNum < numPackets; currPacketNum++) {
		uint8_t currPacketLength;

		if((currPacketNum == numPackets - 1) && (testJPEGSDSize % IMAGE_PACKET_SIZE != 0)) {
			currPacketLength = testJPEGSDSize % IMAGE_PACKET_SIZE;
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

		imagePacketToSend[0] = currPacketLength - 1;
		imagePacketToSend[1] = (uint8_t)currPacketNum;
		imagePacketToSend[2] = (uint8_t)(currPacketNum >> 8);


		for(int currByteNum = 3; currByteNum < currPacketLength; currByteNum++) {
			uint8_t currByte =	(uint8_t)testJPEGSD.read();
			imagePacketToSend[currByteNum] = currByte;
			//if(currPacketNum > 1140) {
			//	DLOG((int)imagePacketToSend[currByteNum]);
			//	DLOG(" ");
			//}
			if(imagePacketToSend[currByte] == -1) {
				DLOG("ERROR: Trying to read over the end of the file!\n\r");
				return -2;
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
		send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 0, imagePacketToSend, currPacketLength);
		//DLOG("Waiting for all data to be transmitted\n\r");
		//DLOG("Class item sent\n\r");
		while(payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail); // spin until all the data has been transmitted
		//DLOG("Transmitted\n\r");
	}


	DLOG("Fin!");

	while(1);
	// now we have got at least one tx token so we know we are connected and ready to send stuff


	uint32_t numBytesTrans = 0;


	for(uint16_t currPacketNum = 0; currPacketNum < numPackets; currPacketNum++) {
		//DLOG("Sending data\n\r");
		// wait for the next tx token
		//DLOG("Sending packet #");
		//DLOG(currPacketNum);
		//DLOG("\n\r");

		uint8_t currPacketSize = IMAGE_PACKET_SIZE;
		/*if((uint32_t)((uint32_t)currPacketNum * (uint32_t)IMAGE_PACKET_SIZE) > testJPEGSDSize) {
			// if we are at the end of the image and we don't want to send all IMAGE_PACKET_SIZE bytes
			currPacketSize = (uint32_t)IMAGE_PACKET_SIZE - ((uint32_t)((uint32_t)currPacketNum * (uint32_t)IMAGE_PACKET_SIZE) - testJPEGSDSize);
			DLOG("Last packet\n\r");
		}*/

		if(currPacketNum == numPackets - 1) {
			currPacketSize = testJPEGSDSize % IMAGE_PACKET_SIZE;
			//DLOG("Last packet\n\r");
		}
		currPacketSize += 2; // take into account the current packet number

		//DLOG("Current package size: ");
		//DLOG((int)currPacketSize);
		//DLOG("\n\r");

		imagePacketToSend[0] = currPacketSize;

		imagePacketToSend[1] = (uint8_t)currPacketNum;
		imagePacketToSend[2] = (uint8_t)(currPacketNum >> 8);
		for(int currByte = 3; currByte < currPacketSize + 3; currByte++) {
			imagePacketToSend[currByte] = (uint8_t)testJPEGSD.read();
			numBytesTrans++;
			if(imagePacketToSend[currByte] == -1) {
				DLOG("ERROR: Trying to read over the end of the file!\n\r");
				return -2;
			}
		}

		while(uart0_tx_idle == FALSE) {
					comms_update();
		}

		//DLOG("Waiting for tx token\n\r");
		//recTxToken = false;
		//while(!recTxToken)
		//	comms_update();
		//DLOG("Got tx token\n\r");
		PORTA = 0x00;
		PORTA = 0xFF;
		PORTA = 0x00;
		send_set_class_indexed_item_indexed(CLASS_PAYLOAD, module_id, CLASS_PAYLOAD_MEM_BYTES, 0, imagePacketToSend, currPacketSize + 1);
		PORTA = 0xFF;
		//DLOG("Waiting for all data to be transmitted\n\r");
		//DLOG("Class item sent\n\r");
		while(payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail); // spin until all the data has been transmitted
		//DLOG("Transmitted\n\r");
		PORTA = 0x00;

	}

	DLOG("numBytesTrans: ");
	DLOG("%u", numBytesTrans);
	DLOG("\n\r");

	DLOG("Finished!\n\r");

	// take a single image
	//take_picture();

	return (0);
}
#endif

bool init_sd() {
	// Pin 10 = Chip Select
	pinMode(10, OUTPUT);

	// If SD not connected, stop execution
	if (!SD.begin()) {
		DLOG("SD Failed\n\r");
		return false;
	}
		DLOG("SD Working\n\r");
		return true;
}

/*int dSPIMOSIPin = 12;
int dSPISCLKPin = 13;
int dSPISS = 14;
*/
void init_debug_spi() {

}

void send_debug_spi(char* string) {
	/*digitalWrite(dSPISS, LOW);
	delayMicroseconds(100);
	int i = 0;
	while(string[i] != '\0') {
		for(int j = 0; j < 8; j++) {
			if((string[i] & (0x01 << j)) >> j)
				digitalWrite(dSPIMOSIPin, LOW);
			else
				digitalWrite(dSPIMOSIPin, HIGH);

			digitalWrite(dSPISCLKPin, LOW);
			delayMicroseconds(100);
			digitalWrite(dSPISCLKPin, HIGH);
			delayMicroseconds(100);
		}
		i++;
	}
	digitalWrite(dSPISS, HIGH);*/

	spiDebug.print(string);
}

