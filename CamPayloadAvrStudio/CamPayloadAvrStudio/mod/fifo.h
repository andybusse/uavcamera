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
extern void fifo_add_byte(fifo_struct *fifo, uint8_t byte_in); // inline
extern void fifo_get_byte(fifo_struct *fifo, uint8_t *byte_out); // inline

#endif // __FIFO_H
