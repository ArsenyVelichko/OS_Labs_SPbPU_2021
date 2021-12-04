#pragma once

#include "Connection.h"
#include "IODevice.h"

class ConnectionPrivate {
public:
	ConnectionPrivate(Connection* conn, bool create);

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	Connection* m_conn;
};
