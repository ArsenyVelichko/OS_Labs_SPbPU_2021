#include "Serialization.h"

void Serialization::serialize(char* buf, int value) {
	auto intBuf = reinterpret_cast<int*>(buf);
	*intBuf = value;
}

int Serialization::parse(const char* buf) {
	auto intBuf = reinterpret_cast<const int*>(buf);
	return *intBuf;
}
