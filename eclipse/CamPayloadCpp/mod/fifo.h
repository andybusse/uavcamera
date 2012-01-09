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

// First in, first out (FIFO) buffer functionality

#ifndef __FIFO_H
#define __FIFO_H

// Definitions

typedef volatile struct fifo_t
{
  uint8_t head, tail;
  uint8_t buffer_length;
  uint8_t *buffer;
} fifo_struct;

// Public globals

// Function prototypes

void fifo_setup(fifo_struct *fifo, uint8_t *buffer, uint8_t buffer_length);
//extern void fifo_add_byte(fifo_struct *fifo, uint8_t byte_in); // inline
//extern void fifo_get_byte(fifo_struct *fifo, uint8_t *byte_out); // inline

inline void fifo_add_byte(fifo_struct *fifo, uint8_t byte_in)
{
  fifo->buffer[fifo->head++] = byte_in;
  if (fifo->head == fifo->buffer_length)
    fifo->head = 0;
}

inline void fifo_get_byte(fifo_struct *fifo, uint8_t *byte_out)
{
  // this assumes tail != head (i.e. there is some data to get)
  *byte_out = fifo->buffer[fifo->tail++];
  if (fifo->tail == fifo->buffer_length)
    fifo->tail = 0;
}

#endif // __FIFO_H
