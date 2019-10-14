#include "math.h"


unsigned int Math::max(Iterator it) {
	unsigned int result = 0;
	if (!it.has_next()) {
		return result;
	}
	while (it.has_next()) {
		unsigned int value = it.next();
		if (value > result) {
			result = value;
		}
	}
	return result;
}

unsigned int Math::min(Iterator it) {
	unsigned int result = 0xFFFF;
	if (!it.has_next()) {
		return result;
	}
	while (it.has_next()) {
		unsigned int value = it.next();
		if (value < result) {
			result = value;
		}
	}
	return result;
}
