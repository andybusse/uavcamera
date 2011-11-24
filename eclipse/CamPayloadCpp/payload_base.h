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

// for writing to the autopilot's payload memory
#define CLASS_PAYLOAD               7
#define CLASS_PAYLOAD_MEM_BYTES     10

extern SPIDebug spiDebug;

extern void send_text(const char * text, ...);


// DLOG is debug log and ILOG is info log, so you should use ILOG for info you'd
// normally want to display to the user and developer specific debug info using DLOG
//#define DLOG(...)  sSerial.print(__VA_ARGS__)
//#define DLOG(...) send_debug_spi(__VA_ARGS__)
//#define DLOG(...) spiDebug.print(__VA_ARGS__)
#define DLOG(...) send_text(__VA_ARGS__);
#define ILOG(...) send_text(__VA_ARGS__);


#endif /* PAYLOAD_BASE_H_ */
