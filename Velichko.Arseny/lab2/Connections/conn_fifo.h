#pragma once

#include <string>

#include "Connection.h"
#include "IODevice.h"

class File : public IODevice {
public:
	bool open(const std::string& name, int flags);
	~File();

	ssize_t read(char *data, size_t size) override;
	ssize_t write(const char *data, size_t size) override;

	int waitForReadyRead(int timeout) override;

private:
	int m_fd = -1;
};

class ConnectionPrivate {
public:
	ConnectionPrivate(Connection* conn, bool create);

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	static constexpr std::string_view HOST_FIFO_PREFIX = "fifo_host_";
	static constexpr std::string_view CLIENT_FIFO_PREFIX = "fifo_client_";

	Connection* m_conn;
	File m_inFifo;
	File m_outFifo;
};
