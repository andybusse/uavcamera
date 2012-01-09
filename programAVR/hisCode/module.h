/* Copyright 2011 Matt Bennett

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

// Standard payload module functionality

#ifndef __MODULE_H
#define __MODULE_H

// Definitions

#define MODULE_BROADCAST_ID							127
#define MODULE_INITIAL_ID								127
#define MODULE_EEPROM_ADDR_MODULE_ID		0

#define MODULE_PACKET_CLASS_CHANGE_MODULE_ID		255

// Public globals

// Function prototypes

void module_setup();
void module_packet_scan(uint8_t *data, uint8_t length);

// Function prototypes to be supplied externally

extern void payload_tx_enable(boolean_t want_enable);
extern void packet_scan(uint8_t *data, uint8_t length);
extern void packet_tx_request();

#endif // __MODULE_H
