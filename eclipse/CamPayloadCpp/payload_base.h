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
