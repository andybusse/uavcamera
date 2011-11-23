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
