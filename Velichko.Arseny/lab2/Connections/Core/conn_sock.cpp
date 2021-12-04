#include <poll.h>
#include <stdexcept>
#include <unistd.h>

#include "Logger.h"
#include "conn_sock.h"

ConnectionPrivate::ConnectionPrivate(Connection* conn)
	: m_conn(conn), m_socket(new UdpSocket) {
	const std::string& idStr = 	std::to_string(conn->id());

	UnixHostAddress sendAddr = std::string(HOST_ADDR_PREFIX) + idStr;
	UnixHostAddress receiveAddr = std::string(CLIENT_ADDR_PREFIX) + idStr;

	if (m_conn->role() == Connection::Host) {
		std::swap(sendAddr, receiveAddr);
	}
	m_socket->bind(receiveAddr);
	m_socket->setHost(sendAddr);
}

ssize_t ConnectionPrivate::read(char* data, size_t size) {
	m_socket->waitForReadyRead(m_conn->timeout());
	return m_socket->read(data, size);
}

ssize_t ConnectionPrivate::write(const char* data, size_t size) {
	return m_socket->write(data, size);
}

ConnectionPrivate::~ConnectionPrivate() {
	delete m_socket;
}

UdpSocket::UdpSocket() {
	m_socketFd = socket(PF_UNIX, SOCK_DGRAM, 0);
}

int UdpSocket::bind(const UnixHostAddress& address) {
	unlink(address.name().c_str());
	return ::bind(m_socketFd, address.rawAddress(), address.length());
}

int UdpSocket::waitForReadyRead(int timeout) {
	pollfd pollHandler = { m_socketFd, POLLIN };
	return poll(&pollHandler, 1, timeout);
}

ssize_t UdpSocket::write(const char* data, size_t size) {
	if (m_hostAddress.isNull()) {
		log_error("Host wasn't set");
		return -1;
	}

	return sendto(m_socketFd, data, size, MSG_DONTWAIT,
				  m_hostAddress.rawAddress(), m_hostAddress.length());
}

ssize_t UdpSocket::read(char* data, size_t size) {
	return recvfrom(m_socketFd, data, size, MSG_DONTWAIT, nullptr, nullptr);
}

void UdpSocket::setHost(const UnixHostAddress& address) {
	m_hostAddress = address;
}

UdpSocket::~UdpSocket() {
	unlink(m_hostAddress.name().c_str());
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
	return !static_cast<bool>(m_socketAddr);
}
