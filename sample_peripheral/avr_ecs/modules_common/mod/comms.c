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

// Communication base functionality

#include <stdarg.h> // for va_arg
#include <stdio.h> // for vprintf
#include "base.h"
#include "fifo.h"
#include "uart.h"
#include "module.h"

#include "comms.h"

static uint8_t payload_command_bytes[PAYLOAD_COMMAND_BYTES_MAX_LENGTH];

// define global comms stream
comms_stream_struct payload_stream; // linked to RS-485

// define general-purpose temporary payload buffer, can be used globally (except in ISRs)
uint8_t temp_payload_buffer[TEMP_PAYLOAD_BUFFER_LENGTH];
int8_t temp_payload_buffer_count;

uint8_t module_id;

// define local temporary payload variable for packet forming
static uint8_t *temp_payload_void_bytes;

// define local temporary text buffer, used by send_text()
static uint8_t temp_text_buffer[TEMP_TEXT_BUFFER_LENGTH];
static int8_t temp_text_buffer_count;

// define local PAYLOAD comms stream FIFO storage buffers
static uint8_t payload_tx_fifo_buffer[PAYLOAD_TX_FIFO_LENGTH];
static uint8_t payload_rx_fifo_buffer[PAYLOAD_RX_FIFO_LENGTH];

static FILE comms_putchar_file;

// include for printf and vprintf functionality (used by send_text function)
// uses temp_text_buffer and temp_text_buffer_count (and assumes the buffer never overflows)
static int comms_putchar(char ch, FILE *stream)
{ 
  if (ch == '\n')
		temp_text_buffer[temp_text_buffer_count++] = '\r';

  temp_text_buffer[temp_text_buffer_count++] = ch;

  return (ch); // assumes always successful (would otherwise return EOF)
}

static void comms_stream_setup(comms_stream_struct *cs, uint8_t *tx_fifo_buffer, uint8_t tx_fifo_length, uint8_t *rx_fifo_buffer, uint8_t rx_fifo_length, uint8_t *command_bytes, uint8_t command_bytes_max_length)
{
  fifo_setup(&(cs->tx_fifo), tx_fifo_buffer, tx_fifo_length);
  fifo_setup(&(cs->rx_fifo), rx_fifo_buffer, rx_fifo_length);
  cs->scan_state = 0;
  cs->unknown_start_bytes = FALSE;
  cs->rx_packet_errors = 0;
	
	cs->command_bytes = command_bytes;
  cs->command_bytes_max_length = command_bytes_max_length;
  cs->command_bytes_available = 0;
  
	// cs->is_single_class should be set by command source itself (i.e. from start byte of incoming comms packet)
  // however, if this does not dynamically change default to cs->is_single_class is FALSE
  cs->is_single_class = FALSE;
}


void comms_setup()
{
	// set up stdio write redirect to comms_putchar
	fdev_setup_stream(&comms_putchar_file, &comms_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &comms_putchar_file;

	// set up comms stream
  comms_stream_setup(&payload_stream, payload_tx_fifo_buffer, PAYLOAD_TX_FIFO_LENGTH, payload_rx_fifo_buffer, PAYLOAD_RX_FIFO_LENGTH, payload_command_bytes, PAYLOAD_COMMAND_BYTES_MAX_LENGTH);
}

static boolean_t comms_packet_scan(comms_stream_struct *cs) // returns TRUE when a complete and validated packet has been received
{
  uint8_t rx_byte;

  // read waiting byte in RX fifo
  fifo_get_byte(&(cs->rx_fifo), &rx_byte);

  switch (cs->scan_state)
  {
    case 0:
      // wait for START byte (two MSB's are high) to signify start of packet
      if (rx_byte >= COMMS_START_BYTE_BASE)
      {
        cs->start_byte = rx_byte;
        cs->scan_state = 1;
      }
      else
        cs->unknown_start_bytes = TRUE;
      break;

    case 1:
      // expect DLE after START byte

      if (cs->unknown_start_bytes)
      {
        cs->rx_packet_errors++;
        cs->unknown_start_bytes = FALSE;
      }

      if (rx_byte == COMMS_DLE_BYTE)
      {
        cs->is_single_class = (cs->start_byte & COMMS_START_SINGLE_CLASS) ? TRUE : FALSE;
        if (cs->start_byte & COMMS_START_HAS_ADDRESS)
          cs->scan_state = 2;
        else
        {
          cs->source_address = 255;
          cs->destination_address = 255;
          cs->scan_state = 4;
        }
      }
      else if (rx_byte >= COMMS_START_BYTE_BASE)
      {
        cs->start_byte = rx_byte;
        // stay in scan_state 1
      }
      else
      {
        cs->rx_packet_errors++;
        cs->scan_state = 0;
      }
      break;

    case 2:
      // expect source or destination address
      if (rx_byte & 0x80)
      {
        // invalid address
        cs->rx_packet_errors++;
        if (rx_byte >= COMMS_START_BYTE_BASE)
        {
          cs->start_byte = rx_byte;
          cs->scan_state = 1;
        }
        else
          cs->scan_state = 0;
      }
      else if (cs->start_byte & COMMS_START_HAS_SOURCE_ADDRESS)
      {
        cs->source_address = rx_byte;
        cs->scan_state = 3;
      }
      else
      {
        cs->source_address = 255;
        cs->destination_address = rx_byte;
        cs->scan_state = 4;
      }
      break;

    case 3:
      // expect destination address
      if (rx_byte & 0x80)
      {
        // invalid address
        cs->rx_packet_errors++;
        if (rx_byte >= COMMS_START_BYTE_BASE)
        {
          cs->start_byte = rx_byte;
          cs->scan_state = 1;
        }
        else
          cs->scan_state = 0;
      }
      else
      {
        cs->destination_address = rx_byte;
        cs->scan_state = 4;
      }
      break;

    case 4:
      // expect first byte of payload, or DLE for zero-length payload
      if (rx_byte & 0x80)
      {
        if (rx_byte == COMMS_DLE_BYTE)
          cs->scan_state = 7;
        else
        {
          cs->rx_packet_errors++;
          if (rx_byte >= COMMS_START_BYTE_BASE)
          {
            cs->start_byte = rx_byte;
            cs->scan_state = 1;
          }
          else
            cs->scan_state = 0;
        }
      }
      else
      {
        if (cs->start_byte & COMMS_START_PAYLOAD_FIRST_BYTE_MSB_HIGH)
          cs->command_bytes[0] = rx_byte | 0x80;
        else
          cs->command_bytes[0] = rx_byte;

        cs->command_bytes_available = 1;
        cs->scan_bytecheck = 1;
        cs->last_pre_dle = 0; // flag for whether a START byte was received prior to a DLE in the payload (it is assumed no START byte is 0)
        cs->scan_state = 5;
      }
      break;

    case 5:
      // beyond first byte of payload, previous byte was not DLE
      if (rx_byte == COMMS_DLE_BYTE)
        cs->scan_state = 6; // "destuff" DLE byte
      else if (cs->command_bytes_available >= cs->command_bytes_max_length)
      {
        // payload has overflowed payload buffer, abort packet
        cs->rx_packet_errors++;
        cs->scan_state = 0;
      }
      else
      {
        // add byte to payload buffer
        cs->command_bytes[cs->command_bytes_available++] = rx_byte;
        cs->last_pre_dle = rx_byte; // set to detect START DLE sequence in scan_state 5

        if (cs->scan_bytecheck >= 127)
          cs->scan_bytecheck = 0; // bytecheck cannot be > 127, reset counter
        else
          cs->scan_bytecheck++;

        // stay in scan_state 5
      }
      break;

    case 6:
      // beyond first byte of payload, previous byte was DLE
      if (rx_byte == COMMS_DLE_BYTE)
      {
        if (cs->command_bytes_available >= cs->command_bytes_max_length)
        {
          // payload has overflowed payload buffer, abort packet
          cs->rx_packet_errors++;
          cs->scan_state = 0;
        }
        else
        {
          // add byte to payload buffer
          cs->command_bytes[cs->command_bytes_available++] = rx_byte;
            cs->last_pre_dle = 0; // no START byte was received prior to this DLE

          if (cs->scan_bytecheck >= 127)
            cs->scan_bytecheck = 0; // bytecheck cannot be > 127, reset counter
          else
            cs->scan_bytecheck++;

          cs->scan_state = 5;
        }
      }
      else if (rx_byte == COMMS_ETX_BYTE)
        cs->scan_state = 8;
      else
      {
        cs->rx_packet_errors++; // assume last packet was incomplete and increment error count
        if (rx_byte >= COMMS_START_BYTE_BASE)
        {
          cs->start_byte = rx_byte;
          cs->scan_state = 1;
        }
        else if (cs->last_pre_dle >= COMMS_START_BYTE_BASE)
        {
          // assume have actually found the START DLE of a new packet, and rx_byte is the next byte
          cs->start_byte = cs->last_pre_dle;
          cs->is_single_class = (cs->start_byte & COMMS_START_SINGLE_CLASS) ? TRUE : FALSE;
          if (cs->start_byte & COMMS_START_HAS_ADDRESS)
          {
            // do scan_state 2 work

            // expect source or destination address
            if (rx_byte & 0x80)
            {
              // invalid address
              cs->rx_packet_errors++;
              if (rx_byte >= COMMS_START_BYTE_BASE)
              {
                cs->start_byte = rx_byte;
                cs->scan_state = 1;
              }
              else
                cs->scan_state = 0;
            }
            else if (cs->start_byte & COMMS_START_HAS_SOURCE_ADDRESS)
            {
              cs->source_address = rx_byte;
              cs->scan_state = 3;
            }
            else
            {
              cs->source_address = 255;
              cs->destination_address = rx_byte;
              cs->scan_state = 4;
            }
          }
          else
          {
            cs->source_address = 255;
            cs->destination_address = 255;
            // do scan_state 4 work

            // expect first byte of payload, or DLE for zero-length payload
            if (rx_byte & 0x80)
            {
              if (rx_byte == COMMS_DLE_BYTE)
                cs->scan_state = 7;
              else
              {
                cs->rx_packet_errors++;
                if (rx_byte >= COMMS_START_BYTE_BASE)
                {
                  cs->start_byte = rx_byte;
                  cs->scan_state = 1;
                }
                else
                  cs->scan_state = 0;
              }
            }
            else
            {
              if (cs->start_byte & COMMS_START_PAYLOAD_FIRST_BYTE_MSB_HIGH)
                cs->command_bytes[0] = rx_byte | 0x80;
              else
                cs->command_bytes[0] = rx_byte;
      
              cs->command_bytes_available = 1;
              cs->scan_bytecheck = 1;
              cs->last_pre_dle = 0; // flag for whether a START byte was received prior to a DLE in the payload (it is assumed no START byte is 0)
              cs->scan_state = 5;
            }
          }
        }
        else
          cs->scan_state = 0;
      }
      break;

    case 7:
      // first byte of payload was DLE
      if (rx_byte == COMMS_ETX_BYTE)
      {
        cs->command_bytes_available = 0;
        cs->scan_bytecheck = 0;
        cs->scan_state = 8;
      }
      else
      {
        cs->rx_packet_errors++;
        if (rx_byte >= COMMS_START_BYTE_BASE)
        {
          cs->start_byte = rx_byte;
          cs->scan_state = 1;
        }
        else
          cs->scan_state = 0;
      }
      break;

    case 8:
      // expect bytecheck byte
      if (rx_byte > 127)
      {
        cs->rx_packet_errors++;
        if (rx_byte >= COMMS_START_BYTE_BASE)
        {
          cs->start_byte = rx_byte;
          cs->scan_state = 1;
        }
        else
          cs->scan_state = 0;
      }
      else if (rx_byte != cs->scan_bytecheck)
      {
        // bytecheck mismatch; some payload bytes have been missed
        cs->rx_packet_errors++;
        cs->scan_state = 0;
      }
      else
      {
        cs->scan_state = 0; // reset scan_state
        return (TRUE); // flag complete and verified packet has been received
      }
      break;

  } // end of scan_state switch

  return (FALSE); // flag no complete packet has been found yet
}

void comms_update()
{
  // check PAYLOAD RX FIFO buffer
  if (payload_stream.rx_fifo.tail != payload_stream.rx_fifo.head)
  {
    if (comms_packet_scan(&payload_stream))
      module_packet_scan(payload_stream.command_bytes, payload_stream.command_bytes_available);
  }

  // no need to check PAYLOAD TX FIFO buffer - it will be done automatically when a TX token is received
}

void send_set_item(uint8_t class_id, uint8_t item_id, void *item, uint8_t item_length)
{
  temp_payload_buffer[0] = class_id | CLASS_FLAG_IS_SET; // add SET flag
  temp_payload_buffer[1] = item_id;
  temp_payload_buffer_count = 2;
  temp_payload_void_bytes = item;
  while (item_length-- > 0)
    temp_payload_buffer[temp_payload_buffer_count++] = *temp_payload_void_bytes++;
  send_packet(COMMS_START_BYTE_BASE | COMMS_START_SINGLE_CLASS, temp_payload_buffer, temp_payload_buffer_count);
}

void send_set_class_indexed_item(uint8_t class_id, uint8_t class_index, uint8_t item_id, void *item, uint8_t item_length)
{
  temp_payload_buffer[0] = class_id | CLASS_FLAG_HAS_INDEX | CLASS_FLAG_IS_SET; // add indexed and SET flag
  temp_payload_buffer[1] = class_index;
  temp_payload_buffer[2] = item_id;
  temp_payload_buffer_count = 3;
  temp_payload_void_bytes = item;
  while (item_length-- > 0)
    temp_payload_buffer[temp_payload_buffer_count++] = *temp_payload_void_bytes++;
  send_packet(COMMS_START_BYTE_BASE | COMMS_START_SINGLE_CLASS, temp_payload_buffer, temp_payload_buffer_count);
}

void SSC_start(uint8_t class_id)
{
  temp_payload_buffer[0] = class_id | CLASS_FLAG_IS_SET; // add SET flag
  temp_payload_buffer_count = 1;
}

void SSC_add_item(uint8_t item_id, void *item, uint8_t item_length)
{
  temp_payload_void_bytes = item;
  temp_payload_buffer[temp_payload_buffer_count++] = item_id;
  while (item_length-- > 0)
    temp_payload_buffer[temp_payload_buffer_count++] = *temp_payload_void_bytes++;
}

void SSC_add_data(void *data, uint8_t data_length)
{
  temp_payload_void_bytes = data;
  while (data_length-- > 0)
    temp_payload_buffer[temp_payload_buffer_count++] = *temp_payload_void_bytes++;
}

void SSC_send()
{
  send_packet(COMMS_START_BYTE_BASE | COMMS_START_SINGLE_CLASS, temp_payload_buffer, temp_payload_buffer_count);
}

void SMC_start()
{
  temp_payload_buffer_count = 0;
}

void SMC_add_item(uint8_t class_id, uint8_t item_id, void *item, uint8_t item_length)
{
  temp_payload_void_bytes = item;
  temp_payload_buffer[temp_payload_buffer_count++] = class_id | CLASS_FLAG_IS_SET; // add SET flag
  temp_payload_buffer[temp_payload_buffer_count++] = item_id;
  while (item_length-- > 0)
    temp_payload_buffer[temp_payload_buffer_count++] = *temp_payload_void_bytes++;
}

void SMC_add_class_indexed_item(uint8_t class_id, uint8_t class_index, uint8_t item_id, void *item, uint8_t item_length)
{
  temp_payload_void_bytes = item;
  temp_payload_buffer[temp_payload_buffer_count++] = class_id | CLASS_FLAG_HAS_INDEX | CLASS_FLAG_IS_SET; // add indexed and SET flag
  temp_payload_buffer[temp_payload_buffer_count++] = class_index;
  temp_payload_buffer[temp_payload_buffer_count++] = item_id;
  while (item_length-- > 0)
    temp_payload_buffer[temp_payload_buffer_count++] = *temp_payload_void_bytes++;
}

void SMC_send()
{
  send_packet(COMMS_START_BYTE_BASE, temp_payload_buffer, temp_payload_buffer_count);
}

void send_text(const char *text, ...)
{
	// send as CLASS_SYSTEM_TEXT
  va_list args;

  temp_text_buffer[0] = CLASS_SYSTEM | CLASS_FLAG_IS_SET; // add SET flag
  temp_text_buffer[1] = CLASS_SYSTEM_TEXT;

  temp_text_buffer_count = 3;

  va_start(args, text);
  vprintf(text, args);
  va_end(args);

  temp_text_buffer[2] = temp_text_buffer_count - 3;

  send_packet(COMMS_START_BYTE_BASE | COMMS_START_SINGLE_CLASS, temp_text_buffer, temp_text_buffer_count);
}

// low-level send packet function, used by all intermediate packet-forming functions
void send_packet(uint8_t start_byte, uint8_t *payload, uint8_t payload_length)
{
  uint8_t bytecheck = 0;
  uint8_t count;
	
	if ((payload_length > 0)&&(payload[0] & 0x80))
  {
    start_byte |= COMMS_START_PAYLOAD_FIRST_BYTE_MSB_HIGH;
    payload[0] &= 0x7F;
  }

  fifo_add_byte(&(payload_stream.tx_fifo), start_byte);
  fifo_add_byte(&(payload_stream.tx_fifo), COMMS_DLE_BYTE);

  for (count = 0; count < payload_length; count++)
  {
    fifo_add_byte(&(payload_stream.tx_fifo), payload[count]);
    if (payload[count] == COMMS_DLE_BYTE)
      fifo_add_byte(&(payload_stream.tx_fifo), COMMS_DLE_BYTE); // "stuff" DLE byte
    
    if (bytecheck >= 127)
      bytecheck = 0; // bytecheck cannot be > 127, reset counter
    else
      bytecheck++;
  }

  fifo_add_byte(&(payload_stream.tx_fifo), COMMS_DLE_BYTE);
  fifo_add_byte(&(payload_stream.tx_fifo), COMMS_ETX_BYTE); // assume no originator byte to pass on for now..
  fifo_add_byte(&(payload_stream.tx_fifo), bytecheck);
}

