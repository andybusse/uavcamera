// IO pin functions

#include <avr/io.h>
#include "mod/base.h"
#include "mod/module.h"

#include "io_pins.h"

// defined by module.h but must be implemented locally, here
void payload_tx_enable(boolean_t want_enable)
{
	if (want_enable)
		PORTC |= PAYLOAD_TX_ENABLE;
	else
		PORTC &= ~PAYLOAD_TX_ENABLE;
}

void io_pins_setup()
{
	// 1 = output, 0 = input
	
	//DDRB = 0b00101100; // only PB2 (SPI_SS), PB3 (SPI_MOSI) and PB5 (SPI_CLK) are outputs
	DDRC |= 0b00000011; // only PC0 (PAYLOAD_TX_ENABLE), PC1 (STATUS_LED)
	DDRD |= 0b00000010; // only PD1 (PAYLOAD_TX) is an output
	
	PORTC |= 0x04; // set PAYLOAD_TX_ENABLE low, STATUS_LED off and nSPI_CS_ADC high
	DDRD |= STATUS_LED;
	DDRD |= 0x20;
}

void toggle_status_led()
{
	if (STATUS_LED_PORT & STATUS_LED)
		STATUS_LED_PORT &= ~STATUS_LED;
	else
		STATUS_LED_PORT |= STATUS_LED;
}

void status_led_on()
{
	STATUS_LED_PORT |= STATUS_LED;
}

void status_led_off()
{
	STATUS_LED_PORT &= ~STATUS_LED;
}
