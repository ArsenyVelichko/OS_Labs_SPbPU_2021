#pragma once

#include <string>

#include "Connection.h"
#include "IOStream.h"

class File : public IOStream {
public:
	void open(const std::string& path, int flags);
	bool isOpened() const;
	void close();
	~File();

	std::string path() const;

	ssize_t read(char *data, size_t size) override;
	ssize_t write(const char *data, size_t size) override;

	int waitForReadyRead(int timeout) override;

private:
	std::string m_path;
	int m_fd;
	bool m_isOpened = false;
};

class ConnectionPrivate {
public:
	ConnectionPrivate(Connection* conn);
	~ConnectionPrivate();

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	static constexpr std::string_view HOST_FIFO_PREFIX = "fifo_host_";
	static constexpr std::string_view CLIENT_FIFO_PREFIX = "fifo_client_";

	Connection* m_conn;
	File m_inFifo;
	File m_outFifo;
};
