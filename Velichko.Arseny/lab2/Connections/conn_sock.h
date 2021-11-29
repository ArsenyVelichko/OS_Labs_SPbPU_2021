#pragma once

#include <cstddef>
#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>
#include <memory>

#include "Connection.h"

class UnixHostAddress {
public:
	UnixHostAddress() = default;
	UnixHostAddress(const std::string& name);

	bool isNull() const;

	void setName(const std::string& name);

	std::string name() const;
	const sockaddr* rawAddress() const;
	size_t length() const;

private:
	std::shared_ptr<sockaddr_un> m_socketAddr;
};

class UdpSocket {
public:
	UdpSocket();

	int bind(const UnixHostAddress& address);
	int pollDatagram(int timeout) const;

	ssize_t writeDatagram(const char* data, size_t size, const UnixHostAddress& address);
	ssize_t readDatagram(char* data, size_t size);

private:
	int m_socketFd;
};

class ConnectionPrivate {
public:
	ConnectionPrivate(int id, int timeout, bool create);
	~ConnectionPrivate();

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

	int id() const;
	int timeout() const;

private:
	static constexpr std::string_view HOST_ADDR_NAME = "host";
	static constexpr std::string_view CLIENT_ADDR_NAME = "Client";

	int m_id;
	int m_timeout;

	UdpSocket* m_socket;
	UnixHostAddress m_sendAddr;
	UnixHostAddress m_receiveAddr;
};
