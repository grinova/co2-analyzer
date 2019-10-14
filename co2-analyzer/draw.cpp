#include "draw.h"

#include <stdlib.h>
#include "math.h"
#include "reader.h"

void Draw::init() {
	display_init();
}

void Draw::clear() {
	display_clear();
}

void Draw::fill() {
	display_fill(0xFF);
}

Draw::Draw(unsigned char chart_length, unsigned int co2_concentration, unsigned char battery):
	co2_concentration(co2_concentration), co2sensor_connected(false), battery(battery), history(Queue(chart_length))
{}

void Draw::set_co2_concentration(unsigned int co2) {
	co2sensor_connected = true;
	co2_concentration = co2;
	history.add(co2);
}

void Draw::draw() {
	if (co2sensor_connected) {
		display_set_cursor(0, 0);
		print_co2_concentration();
	}
	display_set_cursor(11, 0);
	draw_battery();
	if (co2sensor_connected) {
		draw_chart();
	} else {
		draw_sensor_did_not_connect();
	}
}

void Draw::print_co2_concentration() {
	display_send_char(CHAR_C);
	display_send_char(CHAR_O);
	display_send_char(CHAR_LOW_TWO);
	display_send_char(CHAR_COLON);
	display_send_space();
	display_print_number(co2_concentration);
	display_send_char(CHAR_LOW_P);
	display_send_char(CHAR_LOW_P);
	display_send_char(CHAR_LOW_M);
	display_send_space();
	display_send_space();
	display_send_space();
}

void Draw::draw_chart() {
	unsigned char len = history.length();
	unsigned char line_buffer_index = DISPLAY_X_RESOLUTION - len;
	unsigned int min = Math::min(history.interator());
	unsigned int max = Math::max(history.interator());
	char min_str[10];
	char max_str[10];
	itoa(min, min_str, 10);
	itoa(max, max_str, 10);
	Reader min_reader(min_str);
	Reader max_reader(max_str);
	for (unsigned char row = 4; row >= 1; row--) {
		unsigned int low = 8 * (4 - row);
		unsigned int high = low + 8;
		display_set_cursor(0, row + 1);
		Iterator it = history.interator();
		for (int i = 0; i < DISPLAY_X_RESOLUTION; i++) {
			unsigned char level = 0;
			unsigned char digit_byte = 0;
			if (i != 0) {
				if (row == 4) {			// bottom line
					if (min_reader.has_next()) {
						digit_byte = min_reader.next();
					}
				} else if (row == 1) {	// top line
					if (max_reader.has_next()) {
						digit_byte = max_reader.next();
						digit_byte >>= 1;
					}
				}
			}
			if (i >= line_buffer_index) {
				if (it.has_next()) {
					unsigned int value = it.next();
					unsigned int normal = 32 * (value - min) / (max - min);
					if (normal >= high) {
						level = 0xFF;
					} else if (normal <= low) {
						level = 0x00;
					} else {
						normal -= low;
						level = 0;
						while (normal--) {
							level >>= 1;
							level |= 0x80;
						}
					}
				}
			}
			display_send(LCD_D, level ^ digit_byte);
		}
	}
}

void Draw::draw_battery() {
	display_send(LCD_D, 0);
	display_send(LCD_D, 0);
	display_send_char(CHAR_BATTERY + battery);
}

static const char error_message1[] __attribute__((progmem)) = "co";
static const char error_message2[] __attribute__((progmem)) = " sensor";
static const char error_message3[] __attribute__((progmem)) = "not connected";

void Draw::draw_sensor_did_not_connect() {
	//display_set_cursor(0, 0);
	//for (unsigned char ch = 1; ch; ch <<= 1) {
		//display_send(LCD_D, ch);
	//}
	//display_set_cursor(1, 0);
	//display_send_space();
	display_set_cursor(2, 2);
	display_send(LCD_D, 0);
	display_send(LCD_D, 0);
	display_send_prg_string(error_message1);
	//display_send_char(CHAR_C);
	//display_send_char(CHAR_O);
	display_send_char(CHAR_LOW_TWO);
	display_send_prg_string(error_message2);
	display_set_cursor(1, 3);
	display_send(LCD_D, 0);
	display_send(LCD_D, 0);
	display_send_prg_string(error_message3);
}
