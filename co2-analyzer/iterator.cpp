#include "iterator.h"

Iterator::Iterator(unsigned int * buffer, unsigned char len, unsigned char offset):
	buffer(buffer), len(len), i(len), offset(offset)
{}

bool Iterator::has_next() {
	return i != 0;
}

unsigned int Iterator::next() {
	offset %= len;
	unsigned int value = buffer[offset];
	offset += 1;
	i -= 1;
	return value;
}
