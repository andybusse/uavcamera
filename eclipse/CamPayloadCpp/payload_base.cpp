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
#include "payload_base.h"
#include "mod/comms.h"

SPIDebug spiDebug(28, 27, 26);

File sdFile;

uint8_t colourType = 0x07;
uint8_t rawRes = 0x07;
uint8_t jpegRes = 0x07;

void wait_for_tx_buffer_empty() {
	while(payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail);
}
