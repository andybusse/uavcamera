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

extern bool recTxToken;

extern SPIDebug spiDebug;

#endif /* PAYLOAD_BASE_H_ */
