#include <poll.h>
#include <stdexcept>
#include <unistd.h>

#include "conn_sock.h"

ConnectionImpl::ConnectionImpl(int id, bool create) : m_socket(new UdpSocket),
	m_sendAddr(std::string(HOST_ADDR_NAME) + std::to_string(id)) {

	UnixHostAddress receiveAddr = std::string(CLIENT_ADDR_NAME) + std::to_string(id);
	if (create) {
		swap(receiveAddr, m_sendAddr);
	}
	m_socket->bind(receiveAddr);
}

ssize_t ConnectionImpl::read(char* data, size_t size) {
	m_socket->pollDatagram(50000);
	return m_socket->readDatagram(data, size);
}

ssize_t ConnectionImpl::write(const char* data, size_t size) {
	return m_socket->writeDatagram(data, size, m_sendAddr);
}

ConnectionImpl::~ConnectionImpl() {
	delete m_socket;
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

ssize_t UdpSocket::readDatagram(char* data, size_t maxSize) {
	return recvfrom(m_socketFd, data, maxSize, MSG_DONTWAIT, nullptr, nullptr);;
}

int UdpSocket::pollDatagram(int timeout) const {
	pollfd pollHandler = { m_socketFd, POLLIN };
	return poll(&pollHandler, 1, timeout);
}

UnixHostAddress::UnixHostAddress(const std::string& name) {
	m_socketAddr = new sockaddr_un();
	m_socketAddr->sun_family = AF_UNIX;
	strcpy(m_socketAddr->sun_path, name.c_str());
}

UnixHostAddress::UnixHostAddress(const UnixHostAddress& other)
	: UnixHostAddress(other.name()) {}

UnixHostAddress::~UnixHostAddress() {
	delete m_socketAddr;
}

const sockaddr* UnixHostAddress::rawAddress() const {
	return reinterpret_cast<const sockaddr*>(m_socketAddr);
}

size_t UnixHostAddress::length() const {
	return sizeof(m_socketAddr->sun_family) + strlen(m_socketAddr->sun_path);
}

std::string UnixHostAddress::name() const {
	return m_socketAddr->sun_path;
}

UnixHostAddress& UnixHostAddress::operator=(UnixHostAddress other) {
	swap(*this, other);
	return *this;
}

void swap(UnixHostAddress& first, UnixHostAddress& second) {
	using std::swap;
	swap(first.m_socketAddr, second.m_socketAddr);
}
