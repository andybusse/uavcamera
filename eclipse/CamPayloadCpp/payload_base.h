/*
 * payload_base.h
 *
 *  Created on: Nov 21, 2011
 *      Author: mh23g08
 */

#ifndef PAYLOAD_BASE_H_
#define PAYLOAD_BASE_H_

#include "config.h"
#include "spi_debug.h"
#include "SD/SD.h"

/* DEFINITIONS */

// for writing to the autopilot's payload memory
#define CLASS_PAYLOAD               7
#define CLASS_PAYLOAD_MEM_BYTES     10

// DLOG is debug log and ILOG is info log, so you should use ILOG for info you'd
// normally want to display to the user and developer specific debug info using DLOG
//#define DLOG(...)  sSerial.print(__VA_ARGS__)
//#define DLOG(...) send_debug_spi(__VA_ARGS__)
#define DLOG(...) spiDebug.print(__VA_ARGS__)
#define ILOG(...) spiDebug.print(__VA_ARGS__)


/* IGNORE THIS COMMENT when using send_text it seems most sensible (and safe) to wait for the tx fifo to be empty before
 * we add anything to it so we don't overwrite anything in there and wait afterwards so our text
 * is always sent and not overridden */
//#define DLOG(...) wait_for_tx_buffer_empty(); send_text(__VA_ARGS__); wait_for_tx_buffer_empty();
//#define ILOG(...) wait_for_tx_buffer_empty(); send_text(__VA_ARGS__); wait_for_tx_buffer_empty();

//#define DLOG(...) send_text(__VA_ARGS__);
//#define ILOG(...) send_text(__VA_ARGS__);


/* PUBLIC GLOABALS */

extern SPIDebug spiDebug;

// since there can only ever be one file open at any one time on the SD card we will store it here
// you should use this whenever you want to open a file on the sd card, but you should first check
// to see if it is open and close it first
extern File sdFile;


extern uint8_t colourType;
extern uint8_t rawRes;
extern uint8_t jpegRes;

/* FUNCTION PROTOTYPES */

extern void send_text(const char * text, ...);

void wait_for_tx_buffer_empty();




#endif /* PAYLOAD_BASE_H_ */
