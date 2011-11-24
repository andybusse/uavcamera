#include "spi_debug.h"

volatile bool recTxToken = false;

SPIDebug spiDebug(12, 13, 14);
