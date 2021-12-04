#pragma once

#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>
#include <memory>

#include "Connection.h"
#include "IODevice.h"

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

class UdpSocket : public IODevice {
public:
	UdpSocket();

	void setHost(const UnixHostAddress& address);

	int bind(const UnixHostAddress& address);
	int waitForReadyRead(int timeout) override;

	ssize_t write(const char* data, size_t size) override;
	ssize_t read(char* data, size_t size) override;

private:
	int m_socketFd;
	UnixHostAddress m_hostAddress;
};

class ConnectionPrivate {
public:
	ConnectionPrivate(Connection* conn, bool create);
	~ConnectionPrivate();

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	static constexpr std::string_view HOST_ADDR_PREFIX = "sock_host_";
	static constexpr std::string_view CLIENT_ADDR_PREFIX = "sock_client_";

	Connection* m_conn;
	UdpSocket* m_socket;
};
