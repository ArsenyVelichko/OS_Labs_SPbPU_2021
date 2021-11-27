#include "Connection.h"
#include CONN_IMPL_HEADER

Connection::Connection(int id, bool create)
	: m_pImpl(new ConnectionImpl(id, create)) {}

const ConnectionImpl* Connection::pImpl() const {
	return m_pImpl.get();
}

ConnectionImpl* Connection::pImpl() {
	return m_pImpl.get();
}

ssize_t Connection::read(char* data, size_t size) {
	return m_pImpl->read(data, size);
}

ssize_t Connection::write(const char* data, size_t size) {
	return m_pImpl->write(data, size);
}
