#pragma once

#include <vector>

#include "Connection.h"
#include "IOStream.h"

class SharedBuffer : public IOStream {
public:
	SharedBuffer(int key, size_t size, int flags);
	~SharedBuffer();

	void scheduleDestroy();

	ssize_t read(char* data, size_t size) override;
	ssize_t write(const char* data, size_t size) override;

	int waitForReadyRead(int timeout) override;

private:
	struct Header {
		pthread_cond_t cond;
		pthread_mutex_t mutex;
		size_t pos;
		pid_t writerPid;
	};

	static void initSharedMutex(pthread_mutex_t* mutex);
	static void initSharedCond(pthread_cond_t* cond);

	size_t loadSize() const;

	Header* m_header;
	char* m_data;
	size_t m_dataSize;
	int m_memId;
};

class ConnectionPrivate {
public:
	ConnectionPrivate(Connection* conn);
	~ConnectionPrivate();

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	static constexpr char KeySeed = 'G';
	static constexpr size_t DefaultBufferSize = 128;

	Connection* m_conn;
	SharedBuffer* m_sharedBuffer;
};
