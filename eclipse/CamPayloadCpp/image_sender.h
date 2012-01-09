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

#define IMAGE_ACK_RESEND_SIZE	50

struct image_send_state_t {
	bool sendingImage;
	uint16_t currentPacket;
	uint16_t numPackets;
	File imageFile;
	uint32_t imageFileSize;
	bool waitingForAck;
	uint8_t numRetries;
};

/* public globals */

extern image_send_state_t imageSendState;


/* function prototypes */

void init_image_sender();
void send_IMAGE_DATA_packet();

#endif /* IMAGE_SENDER_H_ */
