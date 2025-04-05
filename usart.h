/*
 * usart.h
 *
 * Created: 29/03/2025 09:45:37
 * Author : Paulo H. Langone
 *
 * Library for ATMega328P USART usage based on pooling method
 * Just assynchronous communication by now
 * F_CPU = 16 MHz
 */

#ifndef USART_H
#define USART_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define ASYNCHRONOUS_MODE   (0 << UMSEL00)
#define SYNCHRONOUS_MODE    (1 << UMSEL00)

#define PARITY_NONE         (0 << UPM00)
#define PARITY_EVEN         (2 << UPM00)
#define PARITY_ODD          (3 << UPM00)

#define STOP_BIT_1          (0 << USBS0)
#define STOP_BIT_2          (1 << USBS0)

#define DATA_BIT_5          (0 << UCSZ00)
#define DATA_BIT_6          (1 << UCSZ00)
#define DATA_BIT_7          (2 << UCSZ00)
#define DATA_BIT_8          (3 << UCSZ00)

#define CLK_POS_EDGE        (0 << UCPOL0)
#define CLK_NEG_EDGE        (1 << UCPOL0)

#define SERIAL_5N1  (ASYNCHRONOUS_MODE | PARITY_NONE | STOP_BIT_1 | DATA_BIT_5 | CLK_NEG_EDGE)
#define SERIAL_5N2  (ASYNCHRONOUS_MODE | PARITY_NONE | STOP_BIT_2 | DATA_BIT_5 | CLK_NEG_EDGE)
#define SERIAL_5E1  (ASYNCHRONOUS_MODE | PARITY_EVEN | STOP_BIT_1 | DATA_BIT_5 | CLK_NEG_EDGE)
#define SERIAL_5E2  (ASYNCHRONOUS_MODE | PARITY_EVEN | STOP_BIT_2 | DATA_BIT_5 | CLK_NEG_EDGE)
#define SERIAL_5O1  (ASYNCHRONOUS_MODE | PARITY_ODD  | STOP_BIT_1 | DATA_BIT_5 | CLK_NEG_EDGE)
#define SERIAL_5O2  (ASYNCHRONOUS_MODE | PARITY_ODD  | STOP_BIT_2 | DATA_BIT_5 | CLK_NEG_EDGE)

#define SERIAL_6N1  (ASYNCHRONOUS_MODE | PARITY_NONE | STOP_BIT_1 | DATA_BIT_6 | CLK_NEG_EDGE)
#define SERIAL_6N2  (ASYNCHRONOUS_MODE | PARITY_NONE | STOP_BIT_2 | DATA_BIT_6 | CLK_NEG_EDGE)
#define SERIAL_6E1  (ASYNCHRONOUS_MODE | PARITY_EVEN | STOP_BIT_1 | DATA_BIT_6 | CLK_NEG_EDGE)
#define SERIAL_6E2  (ASYNCHRONOUS_MODE | PARITY_EVEN | STOP_BIT_2 | DATA_BIT_6 | CLK_NEG_EDGE)
#define SERIAL_6O1  (ASYNCHRONOUS_MODE | PARITY_ODD  | STOP_BIT_1 | DATA_BIT_6 | CLK_NEG_EDGE)
#define SERIAL_6O2  (ASYNCHRONOUS_MODE | PARITY_ODD  | STOP_BIT_2 | DATA_BIT_6 | CLK_NEG_EDGE)

#define SERIAL_7N1  (ASYNCHRONOUS_MODE | PARITY_NONE | STOP_BIT_1 | DATA_BIT_7 | CLK_NEG_EDGE)
#define SERIAL_7N2  (ASYNCHRONOUS_MODE | PARITY_NONE | STOP_BIT_2 | DATA_BIT_7 | CLK_NEG_EDGE)
#define SERIAL_7E1  (ASYNCHRONOUS_MODE | PARITY_EVEN | STOP_BIT_1 | DATA_BIT_7 | CLK_NEG_EDGE)
#define SERIAL_7E2  (ASYNCHRONOUS_MODE | PARITY_EVEN | STOP_BIT_2 | DATA_BIT_7 | CLK_NEG_EDGE)
#define SERIAL_7O1  (ASYNCHRONOUS_MODE | PARITY_ODD  | STOP_BIT_1 | DATA_BIT_7 | CLK_NEG_EDGE)
#define SERIAL_7O2  (ASYNCHRONOUS_MODE | PARITY_ODD  | STOP_BIT_2 | DATA_BIT_7 | CLK_NEG_EDGE)

#define SERIAL_8N1  (ASYNCHRONOUS_MODE | PARITY_NONE | STOP_BIT_1 | DATA_BIT_8 | CLK_NEG_EDGE)
#define SERIAL_8N2  (ASYNCHRONOUS_MODE | PARITY_NONE | STOP_BIT_2 | DATA_BIT_8 | CLK_NEG_EDGE)
#define SERIAL_8E1  (ASYNCHRONOUS_MODE | PARITY_EVEN | STOP_BIT_1 | DATA_BIT_8 | CLK_NEG_EDGE)
#define SERIAL_8E2  (ASYNCHRONOUS_MODE | PARITY_EVEN | STOP_BIT_2 | DATA_BIT_8 | CLK_NEG_EDGE)
#define SERIAL_8O1  (ASYNCHRONOUS_MODE | PARITY_ODD  | STOP_BIT_1 | DATA_BIT_8 | CLK_NEG_EDGE)
#define SERIAL_8O2  (ASYNCHRONOUS_MODE | PARITY_ODD  | STOP_BIT_2 | DATA_BIT_8 | CLK_NEG_EDGE)

/*
 * Function prototypes
 */
void usart_init(uint16_t speed, uint16_t config);
void usart_write_char(uint8_t chr);
void usart_write_str(char *str);
uint16_t usart_read_str(char* rcv_buff);

/*
 * @brief Initialize USART
 * @param uint16_t speed Communication speed
 * @param uint16_t config Frame Format
 */
void usart_init(uint16_t speed, uint16_t config)
{
    // Baud rate
	uint16_t baudrate = ((F_CPU / (16UL * speed)) - 1);
    UBRR0H = (uint8_t) (baudrate >> 8);
    UBRR0L = (uint8_t) baudrate;
    
    // Enable receiver & transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Frame format:
    UCSR0C = (config & ((1 << UMSEL00) | (3 << UPM00) | (1 << USBS0) | (1 << UCPOL0)));

    uint8_t data_bits = (config >> UCSZ00) & 0x07;
    UCSR0C |= (data_bits & 0x03) << UCSZ00;                
    UCSR0B |= ((data_bits & 0x04) >> 2) << UCSZ02;
}

/*
 * @brief Transmit a char from USART
 * @param uint8_t chr character to be written  
 */
void usart_write_char(uint8_t chr)
{
    // Wait for the transmit buffer to be empty
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Write data to transmit buffer 
    UDR0 = chr;
}

/*
 * @brief Transmit a string from USART
 * @param const char *str string to be written  
 */
void usart_write_str(char *str)
{
    // While string is not done
    while (*str != '\0')
    {
        usart_write_char(*str);
        str++;
    }
}

/**
 * @brief Read string until <CR><LF> (Carrier Return ou Line Feed)
 * @return r_bytes number of bytes read
 */
uint16_t usart_read_str(char* rcv_buff)
{
    uint16_t r_bytes = 0;
    
    for (r_bytes = 0; r_bytes < 255; r_bytes++)
    {
        while (!(UCSR0A & (1 << RXC0)));
        rcv_buff[r_bytes] = UDR0;
        if (rcv_buff[r_bytes] == '\n')
            break;    
    }
 
    return r_bytes;
}

#endif //USART_H
