#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>

#include "conn_fifo.h"

ConnectionPrivate::ConnectionPrivate(Connection* conn) : m_conn(conn) {
	std::string idStr = std::to_string(conn->id());

	std::string inFifoName = std::string(CLIENT_FIFO_PREFIX) + idStr;
	std::string outFifoName = std::string(HOST_FIFO_PREFIX) + idStr;

	if (m_conn->role() == Connection::Host) {
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

void File::open(const std::string& path, int flags) {
	m_path = path;
	m_fd = ::open(path.c_str(), flags);
	m_isOpened = true;
}

bool File::isOpened() const {
	return m_isOpened;
}

void File::close() {
	if (m_isOpened) {
		m_path = "";
		::close(m_fd);
	}
}

File::~File() {
	close();
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

std::string File::path() const {
	return m_path;
}

ConnectionPrivate::~ConnectionPrivate() {
	if (m_conn->role() == Connection::Host) {
		unlink(m_inFifo.path().c_str());
		unlink(m_inFifo.path().c_str());
	}
}