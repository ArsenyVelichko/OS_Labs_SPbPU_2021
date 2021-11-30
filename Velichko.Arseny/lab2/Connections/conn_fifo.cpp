#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>

#include "conn_fifo.h"

ConnectionPrivate::ConnectionPrivate(Connection* conn, bool create) : m_conn(conn) {
	std::string idStr = std::to_string(conn->id());

	std::string inFifoName = std::string(CLIENT_FIFO_PREFIX) + idStr;
	std::string outFifoName = std::string(HOST_FIFO_PREFIX) + idStr;

	if (create) {
		mkfifo(inFifoName.c_str(), 0666);
		mkfifo(outFifoName.c_str(), 0666);
		std::swap(inFifoName, outFifoName);
	}

	m_inFifo.open(inFifoName, O_RDWR | O_NONBLOCK);
	m_outFifo.open(outFifoName, O_RDWR | O_NONBLOCK);
}

ssize_t ConnectionPrivate::read(char* data, size_t size) {
	m_inFifo.waitForReadyRead(m_conn->timeout());
	return m_inFifo.read(data, size);
}

ssize_t ConnectionPrivate::write(const char* data, size_t size) {
	return m_outFifo.write(data, size);
}

bool File::open(const std::string& name, int flags) {
	m_fd = ::open(name.c_str(), flags);
	return m_fd != -1;
}

File::~File() {
	close(m_fd);
}

ssize_t File::read(char* data, size_t size) {
	return ::read(m_fd, data, size);
}

ssize_t File::write(const char* data, size_t size) {
	return ::write(m_fd, data, size);
}

int File::waitForReadyRead(int timeout) {
	pollfd pollHandler = { m_fd, POLLIN };
	return poll(&pollHandler, 1, timeout);
}
