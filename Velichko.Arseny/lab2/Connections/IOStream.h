#pragma once

#include <cstddef>

class IOStream {
public:
	virtual ssize_t read(char* data, size_t size) = 0;
	virtual ssize_t write(const char* data, size_t size) = 0;

	virtual int waitForReadyRead(int timeout) = 0;
};