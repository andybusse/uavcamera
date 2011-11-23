// First in, first out (FIFO) buffer functionality

#include "base.h"

#include "fifo.h"



void fifo_setup(fifo_struct *fifo, uint8_t *buffer, uint8_t buffer_length)
{
  fifo->head = 0;
  fifo->tail = 0;
  fifo->buffer = buffer;
  fifo->buffer_length = buffer_length;
}
/*
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

*/
