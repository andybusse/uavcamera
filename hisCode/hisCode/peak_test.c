/*
 * peakTest.c
 *
 * Created: 10/28/2011 1:43:10 PM
 *  Author: peak
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include "peak_test.h";

void peak_test_setup()
{
	
		
}
void peak_test_send_int()
{
		uint8_t my_data[8]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};
		send_set_class_item(2,3,my_data,8);
	
};