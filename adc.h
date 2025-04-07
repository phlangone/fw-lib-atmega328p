/*
 * adc.c
 *
 * Created: 05/04/2025 11:08:43
 * Author : Paulo H. Langone
 */ 

#ifndef ADC_H
#define ADC_H

#define F_CPU 16000000UL

#include <avr/io.h>

//Function Prototypes
void adc_init();
uint16_t adc_read(uint8_t channel);

/*
 *	@brief Initialize ADC 
 */
void adc_init()
{
	// AVCC with external capacitor at AREF pin
	ADMUX |= (1 << REFS0);
	
	// ADC Enable and ADPS = 128 (111)
	ADCSRA |= ((1 << ADEN) | (7 << ADPS0));
	
	// Get first conversion and do nothing
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
}

/*
 *	@brief Performs an ADC read in a selected channel
 *  @param uint8_t channel channel to be read
 *  @return read from the selected ADC channel
 */
uint16_t adc_read(uint8_t channel)
{	
	// Wait for any possible conversion to end
	while(ADCSRA & (1 << ADSC));	
	
	// Select channel
	ADMUX |= (channel << MUX0);
	
	// Get conversion
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	
	// Return conversion
	return (ADCL | (ADCH << 8));
}

#endif // ADC_H
