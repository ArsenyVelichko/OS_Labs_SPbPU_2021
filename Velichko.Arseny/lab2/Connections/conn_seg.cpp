#include "conn_seg.h"

ConnectionPrivate::ConnectionPrivate(Connection* conn, bool create) : m_conn(conn) {}

ssize_t ConnectionPrivate::read(char* data, size_t size) {
	return 0;
}

ssize_t ConnectionPrivate::write(const char* data, size_t size) {
	return 0;
}