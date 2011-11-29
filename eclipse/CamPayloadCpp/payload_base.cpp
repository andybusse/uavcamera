#include "spi_debug.h"
#include "payload_base.h"
#include "mod/comms.h"

SPIDebug spiDebug(28, 27, 26);

File sdFile;

uint8_t colourType = 0x07;
uint8_t rawRes = 0x07;
uint8_t jpegRes = 0x07;

void wait_for_tx_buffer_empty() {
	while(payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail);
}
