#ifndef DISPLAY_H_
#define DISPLAY_H_


#include <avr/io.h>
#include <avr/pgmspace.h>

#define PIN_SCE   PB2
#define PIN_SDIN  PB3
#define PIN_SCLK  PB5

#define PORT_DISPLAY	PORTB
#define DDR_DISPLAY		DDRB
#define PIN_RESET		PB6
#define PIN_DC			PB7

#define LCD_C     0x00
#define LCD_D     0x01

#define DISPLAY_X_RESOLUTION	84
#define DISPLAY_Y_RESOLUTION	48

#define DISPLAY_CURSOR_WIDTH	7
#define DISPLAY_CURSOR_HEIGHT	8

#define CHAR_C			10
#define CHAR_O			11
#define CHAR_LOW_P		12
#define CHAR_LOW_M		13
#define CHAR_LOW_TWO	14
#define CHAR_COLON		15
#define CHAR_BATTERY	16
#define SMALL_DIGITS	22
#define SMALL_CHARS		32
#define CHAR_UNKNOWN	52

#define DISPLAY_SYMBOL_WIDTH	5

void display_init();
void display_clear();
void display_fill(unsigned char code);
void display_print_number(int x);
void display_send(unsigned char dc, unsigned char data);
void display_send_char(unsigned char code);
void display_send_space();
void display_send_prg_string(const char * str);
void display_set_cursor(unsigned char x, unsigned char y);
unsigned char display_get_table_byte(unsigned char code, unsigned char index);

#endif /* DISPLAY_H_ */
