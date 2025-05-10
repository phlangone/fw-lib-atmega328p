/*
 * lcd.h
 *
 * Created: 26/04/2025 10:13:39
 * Author : Paulo H. Langone
 * LCD display 
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

/********************************************************
*       Character address on 16x2 LCD Display			*
*********************************************************/
// DDRAM ADDRESS
// DIGIT         1        2        3        4        5        6        7        8        9        10        11        12        13         14        15        16
//   1          80       81       82       83       84       85       86       87       88       89        8A        8B        8C         8D        8E        8F
//   2          C0       C1       C2       C3       C4       C5       C6       C7       C8       C9        CA        CB        CC         CD        CE        CF

/********************************************************
*                      Commands                         *
*********************************************************/
// Display
#define DISPLAY_ON                  0x0C
#define DISPLAY_OFF                 0x0A
#define DISPLAY_CLEAR               0x01

// Cursor control
#define CURSOR_ON                   0x0E
#define CURSOR_OFF                  0x0C
#define CURSOR_SHIFT_LEFT           0x10
#define CURSOR_SHIFT_RIGHT          0x14
#define CURSOR_HOME                 0x02
#define CURSOR_BLINK                0x0D
#define CURSOR_BLINK_ALTERNATE      0x0F

// Cursor shift direction at char input
#define CURSOR_LEFT_ON_INPUT        0x04
#define CURSOR_RIGHT_ON_INPUT       0x06

// Message shift direction at char input
#define TEXT_LEFT_ON_INPUT          0x07
#define TEXT_RIGHT_ON_INPUT         0x05

// Message shift direction at command
#define SHIFT_TEXT_LEFT             0x08
#define SHIFT_TEXT_RIGHT            0x1C


/************************************************************************/
/*					LCD Config Struct                                   */
/************************************************************************/
typedef struct
{
	volatile uint8_t *rs_port;
	uint8_t rs_pin;
	volatile uint8_t *enable_port;
	uint8_t enable_pin;
	volatile uint8_t *data_port;
	uint8_t d4_pin;
	uint8_t d5_pin;
	uint8_t d6_pin;
	uint8_t d7_pin;
} LCD_PinHandle;

/********************************************************
*                 Function Prototypes					*
*********************************************************/
void lcd_init(LCD_PinHandle *lcd);
void lcd_command(LCD_PinHandle *lcd, unsigned char cmd);
void lcd_print_chr_cp(LCD_PinHandle *lcd, unsigned char chr);
void lcd_print_chr(LCD_PinHandle *lcd, unsigned char chr, uint8_t row, uint8_t col);
void lcd_print_txt_cp(LCD_PinHandle *lcd, char *str);
void lcd_print_txt(LCD_PinHandle *lcd, char *str, uint8_t row, uint8_t col);
void enable(LCD_PinHandle *lcd);

/*
 * Send data/command
 */
void enable(LCD_PinHandle *lcd)
{
	_delay_us(40);
	*(lcd->enable_port) |= (1 << lcd->enable_pin);   
	_delay_us(40);
	*(lcd->enable_port) &= ~(1 << lcd->enable_pin);   
}

/*
 * Sends a command to display
 * @param LCD_PinHandle *lcd - Pointer to LCD pin mapping structure
 * @param unsigned char cmd - Command code
 */
void lcd_command(LCD_PinHandle *lcd, unsigned char cmd)
{
	_delay_us(1);
	*(lcd->rs_port) &= ~(1 << lcd->rs_pin);
	_delay_us(1);

	*(lcd->data_port) &= 0x0F;
	*(lcd->data_port) |= (cmd & 0xF0);
	enable(lcd);

	if(cmd == CURSOR_HOME)
		_delay_ms(160);
	else
		_delay_us(40);

	*(lcd->data_port) &= 0x0F;
	*(lcd->data_port) |= ((cmd << 4) & 0xF0);
	enable(lcd);
}


/*
 * Print a character at current cursor position
 * @param LCD_PinHandle *lcd - Pointer to LCD pin mapping structure
 * @param unsigned char chr - Character to be written
 */
void lcd_print_chr_cp(LCD_PinHandle *lcd, unsigned char chr)
{
	_delay_us(1);
	*(lcd->rs_port) |= (1 << lcd->rs_pin);
	_delay_us(1);

	*(lcd->data_port) &= 0x0F;
	*(lcd->data_port) |= (chr & 0xF0);
	enable(lcd);

	*(lcd->data_port) &= 0x0F;
	*(lcd->data_port) |= ((chr << 4) & 0xF0);
	enable(lcd);
}


/*
 * Print a character at a defined position
 * @param LCD_PinHandle *lcd - Pointer to LCD pin mapping structure
 * @param unsigned char chr - Character to be written
 * @param uint8_t row - Row index (starting from 1)
 * @param uint8_t col - Column index (starting from 1)
 */
void lcd_print_chr(LCD_PinHandle *lcd, unsigned char chr, uint8_t row, uint8_t col)
{
	switch (row)
	{
	case 1:
		col += 0x7F;
		lcd_command(lcd, col);
		lcd_print_chr_cp(lcd, chr);
		break;

	case 2:
		col += 0xBF;
		lcd_command(lcd, col);
		lcd_print_chr_cp(lcd, chr);
		break;
	}
}


/*
 * Print a string at current cursor position
 * @param LCD_PinHandle *lcd - Pointer to LCD pin mapping structure
 * @param char *str - Pointer to the string to be written
 */
void lcd_print_txt_cp(LCD_PinHandle *lcd, char *str)
{
	while (*str != 0)
	{
		lcd_print_chr_cp(lcd, *str);
		str++;
	}
}


/*
 * Print a string at a defined position
 * @param LCD_PinHandle *lcd - Pointer to LCD pin mapping structure
 * @param char *str - pointer to a string
 * @param unsigned char row - row index (from 1)
 * @param unsigned char col - column index (from 1)
 */
void lcd_print_txt(LCD_PinHandle *lcd, char *str, uint8_t row, uint8_t col)
{
	switch(row)
	{
	case 1:
		col += 0x7f;
		lcd_command(lcd, col);
		lcd_print_txt_cp(lcd, str);
		break;

	case 2:
		col += 0xbf;
		lcd_command(lcd, col);
		lcd_print_txt_cp(lcd, str);
		break;
	}
}


/*
 * Initialize display (according to Figure 24 from HD44780 datasheet)
 * @param LCD_PinHandle *lcd - Pointer to LCD pin mapping structure
 */
void lcd_init(LCD_PinHandle *lcd)
{
	// Pins DDR
	*(lcd->enable_port - 1) |= (1 << lcd->enable_pin);
	*(lcd->rs_port - 1) |= (1 << lcd->rs_pin);
	*(lcd->data_port - 1) |= (1 << lcd->d4_pin) | (1 << lcd->d5_pin) | (1 << lcd->d6_pin) | (1 << lcd->d7_pin);
  
	// Initialize data bus
	*(lcd->data_port) &= ~(1 << lcd->d4_pin) & ~(1 << lcd->d5_pin) & ~(1 << lcd->d6_pin) & ~(1 << lcd->d7_pin);
  
	_delay_ms(100); // Delay after power on
  
	/* Execute Function Set, according to datasheet
	 * DL = 0 - 4 bits mode
	 * N = 1  - 2 rows
	 * F = 0  - N/A
	 */
	*(lcd->data_port) |= (1 << lcd->d4_pin) | (1 << lcd->d5_pin);
	enable(lcd);
	_delay_ms(5);
	enable(lcd);
	_delay_us(100);
	enable(lcd);
	*(lcd->data_port) &= ~(1 << lcd->d4_pin);
	enable(lcd);
	lcd_command(lcd, 0x28); // Function set at last

	// Clear display
	lcd_command(lcd, DISPLAY_CLEAR);

	// Turn display on, with cursor (and cursor blink)
	lcd_command(lcd, CURSOR_BLINK);

	// Shift right at data input
	lcd_command(lcd, CURSOR_RIGHT_ON_INPUT);
	
	_delay_ms(50);
}
