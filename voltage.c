/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *           View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
/*
avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -Wall -Os -Wl,-u,vfprintf -lprintf_flt -lm voltage.c -o voltage.elf
avr-objcopy -O ihex voltage.elf voltage.hex
avrdude -c usbasp -p m644p -U flash:w:voltage.hex
*/
#include "avr/io.h"
#include "util/delay.h"
#include "debug.h"

#define F_CPU 12000000

void init_adc(void)
{
	// | (1 << ADATE) 
	// | (1 << ADLAR) high bits reading
	ADMUX |= (1 << REFS0); // setting the multiplexer register
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t read_adc(void)
{
	// You might need to set the ADLAR to 1 to read the high bits.
	// ADMUX |= (1 < ADLAR);
	return ADC;
}


int main(void)
{
	uint16_t result;
	double voltage;
	init_debug_uart0();
	while(1)
	{
		init_adc(); // This will restart the conversion back.
		result = read_adc();
		voltage = (result * 3.3) / 1024;
		printf("%4d : %6.5fV\n", result, voltage);
		_delay_ms(1000);
	}
}