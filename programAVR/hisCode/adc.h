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

// ADC functions

#ifndef __ADC_H
#define __ADC_H

// Definitions

// for writing to the autopilot's payload memory
#define CLASS_PAYLOAD               7
#define CLASS_PAYLOAD_MEM_BYTES     10

// Public globals

extern volatile boolean_t adc_z_want_update;
extern uint8_t adc_z_buffer_packed[31]; // includes first size byte
extern volatile uint16_t adc_z_new;

// Function prototypes

void adc_update();
void adc_setup();
void adc_buffer_update();
void adc_pack_buffer();

#endif // __ADC_H
