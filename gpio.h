/*
 * gpio.h
 *
 * Created: 12/04/2025 10:06:30
 * Author : Paulo H. Langone 
 * Library for ATMega328P-based Arduino boards GPIO usage
 * F_CPU = 16 MHz
 */ 

#ifndef GPIO_H
#define GPIO_H

#define F_CPU 16000000UL

#include <avr/io.h>

#define INPUT			0
#define INPUT_PULLUP	1
#define OUTPUT			2

#define HIGH			1
#define LOW				0

/*
 *	Function Prototypes
 */
void gpio_mode(uint8_t pin_num, uint8_t pin_mode);
void gpio_write(uint8_t pin_num, uint8_t pin_level);
uint8_t gpio_read(uint8_t pin_num);

/*
 *	@brief Configure pin mode
 *  @param uint8_t pin_num arduino pin number
 *  @param uint8_t pin_mode selected pin mode (see macros)
 */
void gpio_mode(uint8_t pin_num, uint8_t pin_mode)
{
	if(pin_mode == INPUT)
	{
		if(pin_num <= 7)
		{
			DDRD &= ~(1 << pin_num);
			PORTD &= ~(1 << pin_num);
		}
		else if(pin_num <= 13)
		{
			DDRB &= ~(1 << (pin_num - 8));
			PORTB &= ~(1 << (pin_num - 8));			
		}
	}
	
	else if(pin_mode == INPUT_PULLUP)
	{
		if(pin_num <= 7)
		{
			DDRD &= ~(1 << pin_num);
			PORTD |= (1 << pin_num);
		}
		else if(pin_num <= 13)
		{
			DDRB &= ~(1 << (pin_num - 8));
			PORTB |= (1 << (pin_num - 8));
		}
	}	
	
	else if(pin_mode == OUTPUT)
	{
		if(pin_num <= 7)
		{
			DDRD |= (1 << pin_num);
		}
		else if(pin_num <= 13)
		{
			DDRB |= (1 << (pin_num - 8));
		}
	}
}

/*
 *	@brief Write a logic level in a pin
 *  @param uint8_t pin_num arduino pin number
 *  @param uint8_t pin_level level (see macros) to be written in the selected pin
 */
void gpio_write(uint8_t pin_num, uint8_t pin_level)
{
	if(pin_level == HIGH)
	{
		if(pin_num <= 7)
		{
			PORTD |= (1 << pin_num);
		}
		else if(pin_num <= 13)
		{
			PORTB |= (1 << (pin_num - 8));
		}
	}
	
	else if(pin_level == LOW)
	{
		if(pin_num <= 7)
		{
			PORTD &= ~(1 << pin_num);
		}
		else if(pin_num <= 13)
		{
			PORTB &= ~(1 << (pin_num - 8));
		}
	}
}

/*
 *	@brief Read the logic level in a pin
 *  @param uint8_t pin_num arduino pin number
 *  @return uint8_t logic level read in selected pin
 */
uint8_t gpio_read(uint8_t pin_num)
{
	uint8_t read = 0;
	
	if(pin_num <= 7)
	{
		read = (PIND & (1 << pin_num));
	}
	
	else if(pin_num <= 13)
	{
		read = (PINB & (1 << (pin_num - 8)));
	}
	return read;
}

#endif // GPIO_H