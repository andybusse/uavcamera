#include "spi_debug.h"
#include "payload_base.h"
#include "mod/comms.h"

SPIDebug spiDebug(12, 13, 14);

File sdFile;

void wait_for_tx_buffer_empty() {
	while(payload_stream.tx_fifo.head != payload_stream.tx_fifo.tail);
}
