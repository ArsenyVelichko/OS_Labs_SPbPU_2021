#include <poll.h>
#include <stdexcept>
#include <unistd.h>

#include "conn_sock.h"

ConnectionPrivate::ConnectionPrivate(int id, int timeout, bool create)
	: m_id(id), m_timeout(timeout), m_socket(new UdpSocket) {
	const std::string& idStr = 	std::to_string(id);

	m_sendAddr = std::string(HOST_ADDR_NAME) + idStr;
	m_receiveAddr = std::string(CLIENT_ADDR_NAME) + idStr;

	if (create) {
		std::swap(m_receiveAddr, m_sendAddr);
	}
	m_socket->bind(m_receiveAddr);
}

ssize_t ConnectionPrivate::read(char* data, size_t size) {
	m_socket->pollDatagram(m_timeout);
	return m_socket->readDatagram(data, size);
}

ssize_t ConnectionPrivate::write(const char* data, size_t size) {
	return m_socket->writeDatagram(data, size, m_sendAddr);
}

ConnectionPrivate::~ConnectionPrivate() {
	delete m_socket;
}

int ConnectionPrivate::id() const {
	return m_id;
}

int ConnectionPrivate::timeout() const {
	return m_timeout;
}

UdpSocket::UdpSocket() {
	m_socketFd = socket(PF_UNIX, SOCK_DGRAM, 0);
}

int UdpSocket::bind(const UnixHostAddress& address) {
	unlink(address.name().c_str());
	return ::bind(m_socketFd, address.rawAddress(), address.length());
}

ssize_t UdpSocket::writeDatagram(const char* data, size_t size, const UnixHostAddress& address) {
	return sendto(m_socketFd, data, size, MSG_DONTWAIT, address.rawAddress(), address.length());
}

ssize_t UdpSocket::readDatagram(char* data, size_t size) {
	return recvfrom(m_socketFd, data, size, MSG_DONTWAIT, nullptr, nullptr);;
}

int UdpSocket::pollDatagram(int timeout) const {
	pollfd pollHandler = { m_socketFd, POLLIN };
	return poll(&pollHandler, 1, timeout);
}

UnixHostAddress::UnixHostAddress(const std::string& name) {
	setName(name);
}

const sockaddr* UnixHostAddress::rawAddress() const {
	return std::reinterpret_pointer_cast<sockaddr>(m_socketAddr).get();
}

size_t UnixHostAddress::length() const {
	return sizeof(m_socketAddr->sun_family) + strlen(m_socketAddr->sun_path);
}

void UnixHostAddress::setName(const std::string& name) {
	m_socketAddr.reset(new sockaddr_un());
	m_socketAddr->sun_family = AF_UNIX;
	strcpy(m_socketAddr->sun_path, name.c_str());
}

std::string UnixHostAddress::name() const {
	return m_socketAddr->sun_path;
}

bool UnixHostAddress::isNull() const {
	return static_cast<bool>(m_socketAddr);
}
