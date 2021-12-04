#include <numeric>
#include <sys/shm.h>
#include <cstring>

#include "conn_seg.h"

#include "MultiThreading/MutexLocker.h"

ConnectionPrivate::ConnectionPrivate(Connection* conn, bool create) : m_conn(conn) {

}

ssize_t ConnectionPrivate::read(char* data, size_t size) {
	return 0;
}

ssize_t ConnectionPrivate::write(const char* data, size_t size) {
	return 0;
}

SharedBuffer::SharedBuffer(size_t size, int flags) {

	pthread_mutexattr_t mutexAttr;
	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_setpshared(&mutexAttr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(mutex(), &mutexAttr);

	pthread_condattr_t condAttr;
	pthread_condattr_init(&condAttr);
	pthread_condattr_setpshared(&condAttr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(cond(), &condAttr);
	pthread_condattr_destroy(&condAttr);

	setDataChanged(false);
}

ssize_t SharedBuffer::read(char* data, size_t size) {
	MutexLocker locker(mutex());
	memcpy(data, dataBegin(), size);
	setDataChanged(true);
	return size;
}

ssize_t SharedBuffer::write(const char* data, size_t size) {
	MutexLocker locker(mutex());
	memcpy(dataBegin(), data, size);
	setDataChanged(true);
	pthread_cond_signal(cond());
	return size;
}

int SharedBuffer::waitForReadyRead(int timeout) {
	MutexLocker locker(mutex());
	if (!dataChanged()) {
		pthread_cond_wait(cond(), mutex());
	}
	return 1;
}

SharedBuffer::Header* SharedBuffer::syncData() const {
	return reinterpret_cast<Header*>(m_ptr);
}

pthread_mutex_t* SharedBuffer::mutex() const {
	return &syncData()->mutex;
}

pthread_cond_t* SharedBuffer::cond() const {
	return &syncData()->cond;
}

void SharedBuffer::setDataChanged(bool flag) {
	syncData()->dataChanged = flag;
}

bool SharedBuffer::dataChanged() const {
	return syncData()->dataChanged;
}

char* SharedBuffer::dataBegin() const {
	return m_ptr + sizeof(Header);
}