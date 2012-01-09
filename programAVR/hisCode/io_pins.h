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

// IO pin functions

#ifndef __IO_PINS_H
#define __IO_PINS_H

// Definitions

#define PAYLOAD_TX_ENABLE		0x01	// PC0
#define STATUS_LED					0x02	// PC1
#define nSPI_CS_ADC					0x03	// PC2

// Public globals

// Function prototypes

// extern void payload_tx_enable(boolean_t want_enable) already defined by mod/module.h but must be implemented locally
void io_pins_setup();
void toggle_status_led();
void status_led_on();
void status_led_off();

#endif // __IO_PINS_H
