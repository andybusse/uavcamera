/*
 * spi_debug.h
 *
 *  Created on: Nov 21, 2011
 *      Author: mh23g08
 */

#ifndef SPI_DEBUG_H_
#define SPI_DEBUG_H_

#include <stdint.h>
#include <Print.h>

void init_debug_spi();
void send_debug_spi(char* string);

class SPIDebug : public Print
{
  public:
	SPIDebug(int MOSIPin, int CLKPin, int SSPin);
	void begin();
	void write(uint8_t b);
  private:
	int dSPIMOSIPin;
	int dSPISCLKPin;
	int dSPISS;

};



#endif /* SPI_DEBUG_H_ */
