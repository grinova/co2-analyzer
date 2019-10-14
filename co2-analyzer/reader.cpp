#include "reader.h"
#include "display.h"

Reader::Reader(const char * src): buffer(src), offset(0) {}

bool Reader::has_next() {
	return *buffer != 0;
}

unsigned char Reader::next() {
	char symbol = *buffer;
	if (symbol == 0) {
		return 0;
	}
	unsigned char byte = display_get_table_byte(
		SMALL_DIGITS + symbol - '0', offset++
	);
	if (byte == 0 || offset > DISPLAY_SYMBOL_WIDTH) {
		buffer++;
		offset = 0;
	}
	return byte;
}
