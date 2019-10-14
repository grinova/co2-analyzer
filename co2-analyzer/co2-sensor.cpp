#include "co2-sensor.h"
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define RESPONSE_LENGTH	9

static const unsigned char CO2_CONCENTRATION_READ_COMMAND[] __attribute__((progmem)) = {
	0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79
};

//CO2Sensor* CO2Sensor::instance = new CO2Sensor;

//#define XTAL		8000000
//#define BAUDRATE	9600
//#define BAUDDIVIDER	XTAL/(BAUDRATE*16) - 1

CO2Sensor::CO2Sensor() {
	//connected = false;
	co2_concentration_read_command_offset = 0;
	has_request = false;
	// UBRR0 = XTAL / (16 * BAUDRATE) - 1;
	UBRR0 = 51;
	UCSR0C = (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
}

//CO2Sensor* CO2Sensor::get_instance() {
	////if (!instance) {
		////instance = new CO2Sensor();
	////}
	//return instance;
//}

unsigned int CO2Sensor::get_concentration() {
	return concentration;
}

//bool CO2Sensor::is_connected() {
	//return connected;
//}

void CO2Sensor::set_response_callback(void (*callback)(unsigned int)) {
	response_callback = callback;
}

bool CO2Sensor::request_concentration() {
	checksum = 0;
	response_bytes_counter = 0;
	UCSR0B |= (1 << UDRIE0);
	UCSR0B &= ~(1 << RXCIE0) & ~(1 << TXCIE0);
	bool result = !has_request;
	has_request = true;
	return result;
}

void CO2Sensor::receive_next_byte(unsigned char data) {
	response_bytes_counter++;
	if (response_bytes_counter == 1) {
		if (data != 0xFF) {
			stop();
		}
	} else if (response_bytes_counter < RESPONSE_LENGTH) {
		if (response_bytes_counter == 3) {
			concentration_buffer = 0xFF * data;
		} else if (response_bytes_counter == 4) {
			concentration_buffer += data;
		}
		checksum += data;
	} else if (response_bytes_counter == RESPONSE_LENGTH) {
		checksum = (unsigned char)(0xFF) - checksum;
		checksum += 1;
		if (checksum == data) {
			concentration = concentration_buffer;
			response();
		}
		stop();
	}
}

void CO2Sensor::send_next_byte() {
	UCSR0B |= (1 << TXEN0);
	if (co2_concentration_read_command_offset == sizeof(CO2_CONCENTRATION_READ_COMMAND)) {
		co2_concentration_read_command_offset = 0;
		UCSR0B &= ~(1 << UDRIE0);
		UCSR0B |= (1 << TXCIE0);
		return;
	}
	UDR0 = pgm_read_byte(&(CO2_CONCENTRATION_READ_COMMAND[co2_concentration_read_command_offset++]));
}

void CO2Sensor::response() {
	if (response_callback) {
		response_callback(concentration);
	}
}

void CO2Sensor::stop() {
	UCSR0B &= ~(1 << RXCIE0) & ~(1 << RXEN0);
	has_request = false;
}

//ISR(USART_RX_vect) {
	//CO2Sensor::get_instance()->receive_next_byte(UDR0);
//}

ISR(USART_TX_vect) {
	UCSR0B &= ~(1 << TXCIE0) & ~(1 << TXEN0);
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0);
}

//ISR(USART_UDRE_vect) {
	//CO2Sensor::get_instance()->send_next_byte();
//}
