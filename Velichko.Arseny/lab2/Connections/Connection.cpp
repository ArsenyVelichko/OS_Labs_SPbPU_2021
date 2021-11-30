#include "Connection.h"
#include CONN_IMPL_HEADER

Connection::Connection(int id, int timeout, bool create)
	: m_id(id), m_timeout(timeout), m_pImpl(new ConnectionPrivate(this, create)) {}

const ConnectionPrivate* Connection::pImpl() const {
	return m_pImpl.get();
}

ConnectionPrivate* Connection::pImpl() {
	return m_pImpl.get();
}

ssize_t Connection::read(char* data, size_t size) {
	return pImpl()->read(data, size);
}

ssize_t Connection::write(const char* data, size_t size) {
	return pImpl()->write(data, size);
}

int Connection::id() const {
	return m_id;
}

int Connection::timeout() const {
	return m_timeout;
}

Connection::~Connection() = default;
