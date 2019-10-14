#include "moving-average.h"


MovingAverage::MovingAverage(unsigned int length): queue(Queue(length)), sum(0) {
}

unsigned int MovingAverage::get(unsigned int value) {
	sum -= queue.last();
	sum += value;
	queue.add(value);
	return sum / queue.length();
}
