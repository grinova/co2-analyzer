#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "queue.h"
#include "co2-sensor.h"
#include "draw.h"
#include "moving-average.h"


#define ADC_MOVING_AVERAGE_LENGTH	32

void timer_init() {
	OCR1A = 0x9894;	// 5 sec
	OCR1B = 0x0F42;	// 0.5 sec
	TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10) | (1 << 3);
	TIMSK1 = (1 << OCIE1B);
}

void adc_init() {
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADSC) | (1 << ADATE) |
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX = (1 << ADLAR) | (0 << REFS1) | (1 << REFS0);
}

static Draw draw(DISPLAY_X_RESOLUTION, 0, 6);
static CO2Sensor co2sensor;

void sleep(unsigned char sleep_mode) {
	set_sleep_mode(sleep_mode);
	sleep_enable();
	sleep_cpu();
	sleep_disable();
}

enum ShouldSleep {
	None,
	AdcNoiceReduce,
	Idle
};

static ShouldSleep SLEEP_MODE = None;

void co2_sensor_receive_callback(unsigned int co2_concentration) {
	draw.set_co2_concentration(co2_concentration);
	draw.draw();
	SLEEP_MODE = Idle;
}

void co2_sensor_error_callback() {
}

ISR(USART_RX_vect) {
	/*CO2Sensor::get_instance()->*/co2sensor.receive_next_byte(UDR0);
}

//ISR(USART_TX_vect) {
	//UCSR0B &= ~(1 << TXCIE0) & ~(1 << TXEN0);
	//UCSR0B |= (1 << RXCIE0) | (1 << RXEN0);
//}

ISR(USART_UDRE_vect) {
	/*CO2Sensor::get_instance()->*/co2sensor.send_next_byte();
}

ISR(TIMER1_COMPA_vect) {
	SLEEP_MODE = None;
	sei();
	sleep(AdcNoiceReduce);
	cli();
	if (!/*CO2Sensor::get_instance()->*/co2sensor.request_concentration()) {
		draw.co2sensor_connected = false;
		draw.draw();
	}
}

static char SHOTS_COUNTER = 0;

ISR(TIMER1_COMPB_vect) {
	switch (SHOTS_COUNTER) {
		case 0: // 0.5 sec
			OCR1B = 0x1E84; // 1 sec
			Draw::init();
			Draw::fill();
			break;
		case 1: // 1 sec
			TIMSK1 |= (1 << OCIE1A);
			OCR1B = 0x2DC6; // 1.5 sec

			/*CO2Sensor::get_instance()->*/co2sensor.set_response_callback(&co2_sensor_receive_callback);
			adc_init();
			Draw::clear();
			/*CO2Sensor::get_instance()->*/co2sensor.request_concentration();
			break;
		case 2: // 1.5 sec
			TIMSK1 &= ~(1 << OCIE1B);
			OCR1B = 0;
			TCNT1 = 0;
			if (!/*CO2Sensor::get_instance()->*/co2sensor.request_concentration()) {
				draw.co2sensor_connected = false;
				draw.draw();
			}
			break;
	}
	SHOTS_COUNTER++;
}

static unsigned char battery_level_edge[] = { 0, 153, 184, 189, 194, 199 };
static MovingAverage adc_moving_average(ADC_MOVING_AVERAGE_LENGTH);

ISR(ADC_vect) {
	unsigned char value = adc_moving_average.get(ADCH);;
	for (int i = sizeof(battery_level_edge) - 1; i >= 0; i--) {
		if (value >= battery_level_edge[i]) {
			draw.battery = i;
			break;
		}
	}
}

int main() {
	timer_init();

	sei();
	while (1) {
		switch (SLEEP_MODE) {
			case None:
				break;
			case AdcNoiceReduce:
				sleep(SLEEP_MODE_ADC);
				break;
			case Idle:
				sleep(SLEEP_MODE_IDLE);
				break;
		}
	}
}
