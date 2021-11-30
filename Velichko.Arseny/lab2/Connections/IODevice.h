#pragma once

#include <cstddef>

class IODevice {
public:
	virtual ssize_t read(char* data, size_t size) = 0;
	virtual ssize_t write(const char* data, size_t size) = 0;

	virtual int waitForReadyRead(int timeout) { return 0; };
	virtual int waitForReadyWrite(int timeout) { return 0; };
};