#ifndef MOVING_AVERAGE_H_
#define MOVING_AVERAGE_H_


#include "queue.h"

class MovingAverage {
public:
	MovingAverage(unsigned int length);
	unsigned int get(unsigned int value);
private:
	Queue queue;
	int sum;
};

#endif /* MOVING_AVERAGE_H_ */
