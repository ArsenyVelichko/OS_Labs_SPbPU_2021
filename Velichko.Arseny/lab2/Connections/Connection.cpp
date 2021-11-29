#include "Connection.h"
#include CONN_IMPL_HEADER

Connection::Connection(int id, int timeout, bool create)
	: m_pImpl(new ConnectionPrivate(id, timeout, create)) {}

const ConnectionPrivate* Connection::pImpl() const {
	return m_pImpl.get();
}

ConnectionPrivate* Connection::pImpl() {
	return m_pImpl.get();
}

ssize_t Connection::read(char* data, size_t size) {
	return m_pImpl->read(data, size);
}

ssize_t Connection::write(const char* data, size_t size) {
	return m_pImpl->write(data, size);
}

int Connection::id() const {
	return m_pImpl->id();
}

int Connection::timeout() const {
	return m_pImpl->timeout();
}

Connection::~Connection() = default;
