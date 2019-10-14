#ifndef CO2_SENSOR_H_
#define CO2_SENSOR_H_


#include <avr/pgmspace.h>

void co2_sensor_receive_callback(unsigned int co2_concentration);
void co2_sensor_error_callback();

static unsigned char CHECKSUM;
static unsigned char RESPONSE_BYTES_COUNTER;

void send_co2_concentrate_read_command() {
	CHECKSUM = 0;
	RESPONSE_BYTES_COUNTER = 0;
	UCSR0B |= (1 << UDRIE0);
	UCSR0B &= ~(1 << RXCIE0) & ~(1 << TXCIE0);
}

#define RESPONSE_LENGTH	9

static unsigned int CO2_CONCENTRATION_BUFFER = 0;
static unsigned int CO2_CONCENTRATION = 0;

void stop_receiving_co2_concentration() {
	UCSR0B &= ~(1 << RXCIE0) & ~(1 << RXEN0);
}

ISR(USART_RX_vect) {
	unsigned char data = UDR0;
	RESPONSE_BYTES_COUNTER++;
	if (RESPONSE_BYTES_COUNTER == 1) {
		if (data != 0xFF) {
			stop_receiving_co2_concentration();
		}
	} else if (RESPONSE_BYTES_COUNTER < RESPONSE_LENGTH) {
		if (RESPONSE_BYTES_COUNTER == 3) {
			CO2_CONCENTRATION_BUFFER = 0xFF * data;
		} else if (RESPONSE_BYTES_COUNTER == 4) {
			CO2_CONCENTRATION_BUFFER += data;
		}
		CHECKSUM += data;
	} else if (RESPONSE_BYTES_COUNTER == RESPONSE_LENGTH) {
		CHECKSUM = (unsigned char)(0xFF) - CHECKSUM;
		CHECKSUM += 1;
		if (CHECKSUM == data) {
			CO2_CONCENTRATION = CO2_CONCENTRATION_BUFFER;
			co2_sensor_receive_callback(CO2_CONCENTRATION);
		}
		stop_receiving_co2_concentration();
	}
}

ISR(USART_TX_vect) {
	UCSR0B &= ~(1 << TXCIE0) & ~(1 << TXEN0);
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0);
}

static const unsigned char CO2_CONCENTRATION_READ_COMMAND[] __attribute__((progmem)) = {
	0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79
};
static unsigned char CO2_CONCENTRATION_READ_COMMAND_OFFSET = 0;

ISR(USART_UDRE_vect) {
	UCSR0B |= (1 << TXEN0);
	if (CO2_CONCENTRATION_READ_COMMAND_OFFSET == sizeof(CO2_CONCENTRATION_READ_COMMAND)) {
		CO2_CONCENTRATION_READ_COMMAND_OFFSET = 0;
		UCSR0B &= ~(1 << UDRIE0);
		UCSR0B |= (1 << TXCIE0);
		return;
	}
	UDR0 = pgm_read_byte(&(CO2_CONCENTRATION_READ_COMMAND[CO2_CONCENTRATION_READ_COMMAND_OFFSET++]));
}

//#define XTAL		8000000
//#define BAUDRATE	9600
//#define BAUDDIVIDER	XTAL/(BAUDRATE*16) - 1

void co2_sensor_init() {
	// UBRR0 = XTAL / (16 * BAUDRATE) - 1;
	UBRR0 = 51;
	UCSR0C = (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
}

#endif /* CO2_SENSOR_H_ */
