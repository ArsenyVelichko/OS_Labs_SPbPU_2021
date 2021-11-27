#pragma once

#include <cstddef>
#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>

class ConnectionImpl {
public:
	ConnectionImpl(int id, bool create);
	~ConnectionImpl();

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	class UnixHostAddress {
	public:
		UnixHostAddress(const std::string& name);
		~UnixHostAddress();

		std::string name() const;
		const sockaddr* rawAddress() const;
		size_t length() const;

	private:
		sockaddr_un* m_socketAddr;
	};

	class UdpSocket {
	public:
		explicit UdpSocket();

		int bind(const UnixHostAddress& address);
		int pollDatagram(int timeout) const;

		ssize_t writeDatagram(const char* data, size_t size, const UnixHostAddress& address);
		ssize_t readDatagram(char* data, size_t maxSize);

	private:
		int m_socketFd;
	};

	static constexpr std::string_view HOST_ADDR_NAME = "host";
	static constexpr std::string_view CLIENT_ADDR_NAME = "client";

	UdpSocket* m_socket;
	UnixHostAddress m_sendAddr;

	int m_id;
};
