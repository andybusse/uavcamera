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

#include <avr/io.h>
#include <mod/base.h>
#include "io_pins.h"

#include "adc.h"

static uint16_t adc_z_buffer[20];
static uint8_t adc_z_buffer_head;

volatile uint16_t adc_z_new;
uint8_t adc_z_buffer_packed[30];
volatile boolean_t adc_z_want_update;

static uint8_t adc_rw_blocking(uint8_t write_byte)
{
	// start TX
	SPDR = write_byte;
	
	// wait for TX/RX to complete (blocking)
	while ((SPSR & (1 << SPIF)) == 0);
	
	// read RX
	return (SPDR);
}

void adc_pack_buffer()
{
	uint8_t temp_head = adc_z_buffer_head;
	uint8_t i;
	
	// pack the 12-bit values into 8-bit chunks
	// since 2 12-bit values exactly fit into 3 bytes, do as a loop
	
	for (i = 0; i < 30; i += 3)
	{
		adc_z_buffer_packed[i] = adc_z_buffer[temp_head] & 0x00FF;
		adc_z_buffer_packed[i + 1] = adc_z_buffer[temp_head] >> 8;
		temp_head = (temp_head >= 19) ? 0 : (temp_head + 1);
		adc_z_buffer_packed[i + 1] |= (adc_z_buffer[temp_head] << 4) & 0x00F0;
		adc_z_buffer_packed[i + 2] = adc_z_buffer[temp_head] >> 4;
		temp_head = (temp_head >= 19) ? 0 : (temp_head + 1);
	}
}

void adc_buffer_update()
{
	// add newest ADC Z reading to buffer
	adc_z_buffer[adc_z_buffer_head] = adc_z_new;
	
	// clear update flag
	adc_z_want_update = FALSE;

	// increment adc Z buffer head
	if (adc_z_buffer_head >= 19)
		adc_z_buffer_head = 0;
	else
		adc_z_buffer_head++;
}

void adc_update()
{
	if (!adc_z_want_update)
	{
		// update flag is false, so last data was successfully buffered
	
		// read channel 2 (accelerometer Z axis)
		PORTC &= ~nSPI_CS_ADC; // set ADC CS low
		adc_rw_blocking(0x06);
		adc_z_new = (adc_rw_blocking(0x80) & 0x000F) << 8;
		adc_z_new |= adc_rw_blocking(0x00);
		PORTC |= nSPI_CS_ADC; // set ADC CS high
		
		// set update flag (allows remaining work to be done outside of ISR, and without risk of adc_z_new conflict)
		adc_z_want_update = TRUE;
	}
}

void adc_setup()
{
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // enable SPI, set SPI master, set SPI clock as fosc/16 (1 MHz)
	adc_z_buffer_head = 0;
	adc_z_want_update = FALSE;
}
