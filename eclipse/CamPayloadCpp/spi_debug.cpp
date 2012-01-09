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

