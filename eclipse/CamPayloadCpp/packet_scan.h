// Packet scan functionality

#ifndef __PACKET_SCAN_H
#define __PACKET_SCAN_H

// Definitions


	// NB: remember that we will know the size of a packet due to the underlying protocol

	// PL = payload
	// GS = ground station
	//	- GS > PL: Take Picture [ Take Picture Message ID ]
	//		- PL should send back Picture Taken message
	// asks the PL to take a picture and save it to the SD card
	#define MID_TAKE_PICTURE 			0

	//	- PL > GS: Picture Taken [ Picture Taken Message ID ] [ Image ID ]
	// sent back from the PL to the GS when the picture has been taken, includes the image ID which can be used to download image
	#define MID_PICTURE_TAKEN 			1

	//	- GS > PL: Image Download Request [ Image Download Request Message ID ] [ Image ID ]
	//		- PL should send back IMAGE_DOWNLOAD_INFO and then IMAGE_DATA
	// request the whole image to be downloaded
	#define MID_IMAGE_DOWNLOAD_REQUEST 	2

	//	- PL > GS: [ Image Download Info Message ID ] [ Number of Image Packets ]
	// 		- sent after an IMAGE_DOWNLOAD_REQUEST is sent but before we sent IMAGE_DATA back
	#define MID_IMAGE_DOWNLOAD_INFO 	3

	//	- PL > GS: Image Data [ Image Data Message ID ] [ Image Packet Number ] [ Image Data ]
	#define MID_IMAGE_DATA 				4

	// - GS > PL: Configure Camera [ Configure Camera Message ID ] [ Colour Type ] [ Raw image resolution ] [ JPEG Image Resolution ]
	#define MID_CONFIGURE_CAMERA		5

	// - GS > PL: Request Resend [ Request Resend Message ID ] [ Packets to resend ]
	#define MID_REQUEST_RESEND			6

	#define MID_CANCEL_DOWNLOAD			7

	/*//	- GS > PL: Set Colour Type [ Set Colour Type Message ID ] [ Colour Type ]
	//	- GS > PL: Set Image Resolution [ Set Image Resolution Message ID ] [ Image Resolution ]
	//	- GS > PL: Set Colour Type and Image Resolution [ Set Colour Type and Image Resolution Message ID ] [ Colour Type and Resolution (all one byte) ]*/


#define MAX_MESSAGE_LENGTH 69

// Public globals

extern uint8_t messageToSend[];
extern uint8_t messageToSendLength;

/* this MUST be volatile since it is changed inside an ISR and checked in normal code, otherwise the compiler can do funny things (see comment in
 * wait_for_send_message() in packet_scan.cpp for more details about a bug it was causing
 */
volatile extern bool messageStartSendPending;



// Function prototypes

void send_PICTURE_TAKEN_message(uint16_t imageID);
void send_IMAGE_DOWNLOAD_INFO_message(uint16_t numPackets);
void flag_want_to_send_message();
void wait_for_send_message();

// extern prototypes for packet_scan() and packet_rx_request() prototypes already supplied in common modules file mod/module.h but must be implemented locally

#endif // __PACKET_SCAN_H
