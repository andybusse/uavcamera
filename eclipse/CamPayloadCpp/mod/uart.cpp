/* Copyright 2011 Michael Hodgson, Piyabhum Sornpaisarn, Andrew Busse, John Charlesworth, Paramithi Svastisinha, Matt Bennett

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

// UART (serial) functions

#include <avr/io.h>
#include <avr/interrupt.h>
#include "base.h"
#include "comms.h"
#include "module.h"

#include "uart.h"


volatile boolean_t uart0_tx_idle;
volatile boolean_t sent_etx_token;

// only to be called if ((payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail)&&(uart0_tx_idle))
static inline void uart0_start_tx()
{
	payload_tx_enable(TRUE); // enable payload TX
  uart0_tx_idle = FALSE;

	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0) | (1 << UCSZ02) | (1 << UDRIE0) | (1 << TXCIE0); // enable TX register empty interrupt
}



// fires when transmit buffer is ready to receive new data
ISR(USART0_UDRE_vect)
{
	if (payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail)
	{
		// get byte to be transmitted from payload_stream.tx_fifo (more efficient to duplicate fifo_get_byte() code here)
		UDR0 = payload_stream.tx_fifo.buffer[payload_stream.tx_fifo.tail++];
		if (payload_stream.tx_fifo.tail == payload_stream.tx_fifo.buffer_length)
			payload_stream.tx_fifo.tail = 0;

	}
	else if (sent_etx_token == FALSE)
	{
		// send end of TX token byte - 9th bit must be high to flag this
		sent_etx_token = TRUE;
		UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0) | (1 << UCSZ02) | (1 << UDRIE0) | (1 << TXB80); // set TXB80 high
		UDR0 = module_id;
	}
	else
	{
		UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0) | (1 << UCSZ02) | (1 << TXCIE0); // disable TX register empty interrupt
		sent_etx_token = FALSE; // reset flag
		uart0_tx_idle = TRUE;


	}
}

// fires when transmit is complete
ISR(USART0_TX_vect) {

	if(uart0_tx_idle == TRUE) {
		payload_tx_enable(FALSE); // disable payload TX
	}

}

ISR(USART0_RX_vect)
{
	uint8_t rx_byte;
	
	if (UCSR0B & 0x02)
	{
		char resh = UCSR0B;
		rx_byte = UDR0;
		
		if (((rx_byte & 0x7F) == module_id) || ((rx_byte & 0x7F) == MODULE_BROADCAST_ID))
		{
			// address is intended for this module
			UCSR0A = 0; // disable multi-processor comms mode, i.e. continue to receive next set of non-address RX data
		
			if (rx_byte & 0x80)
			{
				// received a TX token
				
				packet_tx_request();
			
				if (uart0_tx_idle)
					uart0_start_tx(); // start TX immediately (even if it's just a "nothing to TX" byte)
			}
		}
		else
			UCSR0A = (1 << MPCM0); // enable multi-processor comms mode, i.e. ignore subsequent RX data until next address byte (9th bit is high)
	}
	else
	{
		// add received byte to payload_stream.rx_fifo (more efficient to duplicate fifo_add_byte() code here)
		// since multi-processor comms mode used, assume this data is intended for this module
		payload_stream.rx_fifo.buffer[payload_stream.rx_fifo.head++] = UDR0;
		if (payload_stream.rx_fifo.head == payload_stream.rx_fifo.buffer_length)
			payload_stream.rx_fifo.head = 0;
	}
	


}


void uart0_setup()
{
	// set for 38400 baud assuming 16 MHz clock
	UBRR0H = 0x00;
	UBRR0L = 0x19;

	UCSR0A = (1 << MPCM0); // disable double TX speed and enable multi-processor comms mode (i.e. ignore all RX data until address bit received (9th bit is high))
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0) | (1 << UCSZ02); // enable RX and TX and the RX interrupt (do not enable TX interrupt until data ready to send), set UCSZ02 high (for 9 bit character size)
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // set as asynchronous, format as 9N1 (9, not 8, given UCSZ02 in UCSR0B)

  uart0_tx_idle = TRUE;
	sent_etx_token = FALSE;


	// set for 38400 baud assuming 12 MHz clock // UBRR must be 0x13
	/*UBRR0H = 0x00;
	UBRR0L = 0x13;
	
	UCSR0A = (1 << MPCM0); // disable double TX speed and enable multi-processor comms mode (i.e. ignore all RX data until address bit received (9th bit is high))
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0) | (1 << UCSZ02) | (1 << TXCIE0); // enable RX and TX and the RX interrupt (do not enable TX interrupt until data ready to send), set UCSZ02 high (for 9 bit character size) and enable the tx complete interrupt
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // set as asynchronous, format as 9N1 (9, not 8, given UCSZ02 in UCSR0B)

	uart0_tx_idle = TRUE;
	sent_etx_token = FALSE;*/

}

