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

#ifndef __COMMS_H
#define __COMMS_H

#include "fifo.h" // for fifo_struct definition

// Definitions

// comms packet bytes

#define COMMS_START_BYTE_BASE                     0xC0

#define COMMS_START_SINGLE_CLASS                  0x01
#define COMMS_START_PAYLOAD_FIRST_BYTE_MSB_HIGH   0x02
#define COMMS_START_HAS_DESTINATION_ADDRESS       0x04
#define COMMS_START_HAS_SOURCE_ADDRESS            0x08
#define COMMS_START_HAS_ADDRESS                   0x0C
#define COMMS_START_RESERVED_B                    0x10
#define COMMS_START_RESERVED_A                    0x20

#define COMMS_ETX_BYTE                            0xBE
#define COMMS_DLE_BYTE                            0xBD

// buffer max lengths (all expected to be < 256)

#define PAYLOAD_TX_FIFO_LENGTH            				96
#define PAYLOAD_RX_FIFO_LENGTH            				96
#define PAYLOAD_COMMAND_BYTES_MAX_LENGTH  				64

#define TEMP_PAYLOAD_BUFFER_LENGTH        				64 // length of temporary payload buffer for send packet forming functions
#define TEMP_TEXT_BUFFER_LENGTH           				64 // length of text buffer used by send_text()

// Class and class item flags

#define CLASS_FLAG_HAS_INDEX                        0x80
#define CLASS_FLAG_IS_SET                           0x40
#define CLASS_ID_MASK                               0x3F

#define CLASS_ITEM_FLAG_HAS_INDEX                   0x80
#define CLASS_ITEM_ID_MASK                          0x7F

// class item defines used for send_text()

#define CLASS_SYSTEM                							3
#define CLASS_SYSTEM_TEXT                 				0

// comms stream is a two-way communications link
typedef struct comms_stream_t
{
  fifo_struct rx_fifo;
  fifo_struct tx_fifo;
  uint32_t rx_packet_errors;
  boolean_t unknown_start_bytes;
	boolean_t is_single_class;
	uint8_t *command_bytes;
	uint8_t start_byte;
	uint8_t command_bytes_max_length;
  uint8_t command_bytes_available;
  uint8_t last_pre_dle;
  uint8_t scan_state;
  uint8_t scan_bytecheck;
  uint8_t source_address;
  uint8_t destination_address;
} comms_stream_struct;

// Public globals

extern comms_stream_struct payload_stream; // PAYLOAD comms stream, linked to RS-485
extern uint8_t module_id;

// Function prototypes

void comms_setup();
void comms_update();
void send_set_item(uint8_t class_id, uint8_t item_id, void *item, uint8_t item_length);
void send_set_class_indexed_item(uint8_t class_id, uint8_t class_index, uint8_t item_id, void *item, uint8_t item_length);
void SSC_start(uint8_t class_id);
void SSC_add_item(uint8_t item_id, void *item, uint8_t item_length);
void SSC_add_data(void *data, uint8_t data_length);
void SSC_send();
void SMC_start();
void SMC_add_item(uint8_t class_id, uint8_t item_id, void *item, uint8_t item_length);
void SMC_add_class_indexed_item(uint8_t class_id, uint8_t class_index, uint8_t item_id, void *item, uint8_t item_length);
void SMC_send();
void send_text(const char *text, ...);
void send_packet(uint8_t start_byte, uint8_t *payload, uint8_t payload_length);

#endif // __COMMS_H
