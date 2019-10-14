#include "queue.h"

#include <stdlib.h>


Queue::Queue(unsigned char cap):
	cap(cap), len(0), offset(0)
{
	buffer = (unsigned int*)malloc(cap * sizeof(unsigned int));
	while (cap--) {
		buffer[cap] = 0;
	}
}

Queue::~Queue() {
	free(buffer);
}

void Queue::add(unsigned int value) {
	buffer[offset] = value;
	offset = (offset + 1) % cap;
	if (len < cap) {
		len += 1;
	}
}

Iterator Queue::interator() {
	return Iterator(buffer, len, offset);
}

unsigned char Queue::last() {
	return buffer[offset % cap];
}

unsigned char Queue::length() {
	return len;
}
