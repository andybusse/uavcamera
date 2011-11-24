/*
 * image_sender.h
 *
 *  Created on: Nov 24, 2011
 *      Author: mh23g08
 */

#ifndef IMAGE_SENDER_H_
#define IMAGE_SENDER_H_

#include "SD/SD.h"

/* definitions */

#define IMAGE_PACKET_SIZE 50

struct image_send_state_t {
	bool sendingImage;
	uint16_t currentPacket;
	uint16_t numPackets;
	File imageFile;
	uint32_t imageFileSize;
};

/* public globals */

extern image_send_state_t imageSendState;


/* function prototypes */

void init_image_sender();
void send_IMAGE_DATA_packet();

#endif /* IMAGE_SENDER_H_ */
