#ifndef DRAW_H_
#define DRAW_H_


#include "queue.h"
#include "display.h"

class Draw {
private:
	unsigned int co2_concentration;
public:
	static void init();
	static void clear();
	static void fill();

	bool co2sensor_connected;
	unsigned char battery;
	Draw(unsigned char chart_length, unsigned int co2_concentration, unsigned char battery);
	Queue history;
	void draw();
	void set_co2_concentration(unsigned int co2_concentration);
private:
	//unsigned int co2_concentration;
	void print_co2_concentration();
	void draw_chart();
	void draw_battery();
	void draw_sensor_did_not_connect();
};


#endif /* DRAW_H_ */
