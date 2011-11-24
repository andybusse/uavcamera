
#include "spi_debug.h"

#include <WProgram.h>

SPIDebug::SPIDebug(int MOSIPin, int CLKPin, int SSPin) {
	dSPIMOSIPin = MOSIPin;
	dSPISCLKPin = CLKPin;
	dSPISS = SSPin;
}

void SPIDebug::begin() {
	pinMode(dSPIMOSIPin, OUTPUT);
	pinMode(dSPISCLKPin, OUTPUT);
	pinMode(dSPISS, OUTPUT);
	digitalWrite(dSPIMOSIPin, HIGH);
	digitalWrite(dSPISCLKPin, HIGH);
	digitalWrite(dSPISS, HIGH);
}

void SPIDebug::write(uint8_t b) {
	digitalWrite(dSPISS, LOW);
	delayMicroseconds(10);
	for(int j = 0; j < 8; j++) {
		if((b & (0x01 << j)) >> j)
			digitalWrite(dSPIMOSIPin, LOW);
		else
			digitalWrite(dSPIMOSIPin, HIGH);

		digitalWrite(dSPISCLKPin, LOW);
		delayMicroseconds(10);
		digitalWrite(dSPISCLKPin, HIGH);
		delayMicroseconds(10);
	}
	digitalWrite(dSPISS, HIGH);
}

