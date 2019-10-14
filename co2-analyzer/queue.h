#ifndef QUEUE_H_
#define QUEUE_H_

#include "iterator.h"

class Queue {
public:
	Queue(unsigned char cap);
	~Queue();
	void add(unsigned int value);
	Iterator interator();
	unsigned char last();
	unsigned char length();

private:
	unsigned char cap;
	unsigned char len;
	unsigned int* buffer;
	unsigned char offset;
};

#endif /* QUEUE_H_ */
