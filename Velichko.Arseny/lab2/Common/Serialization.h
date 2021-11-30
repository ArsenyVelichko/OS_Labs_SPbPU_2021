#pragma once

#include <cstddef>

namespace Serialization {
	void serialize(char* buf, int value);
	int parse(const char* buf);
}