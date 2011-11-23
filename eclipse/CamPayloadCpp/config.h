/*
 * config.h
 *
 *  Created on: Nov 21, 2011
 *      Author: mh23g08
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define CUSTOM_UART_0
#define F_CPU 12000000

#define STD_DELAY

//#define DLOG(...)  sSerial.print(__VA_ARGS__)
//#define DLOG(...) send_debug_spi(__VA_ARGS__)
#define DLOG(...) spiDebug.print(__VA_ARGS__)


#endif /* CONFIG_H_ */
