#pragma once

#include <vector>

#include "Connection.h"
#include "IOStream.h"

class SharedBuffer : public IOStream {
public:
	SharedBuffer(size_t size, int flags);

	ssize_t read(char* data, size_t size) override;
	ssize_t write(const char* data, size_t size) override;

	int waitForReadyRead(int timeout) override;

private:
	struct Header {
		pthread_cond_t cond;
		pthread_mutex_t mutex;
		bool dataChanged;
	};

	Header* syncData() const;

	pthread_mutex_t* mutex() const;
	pthread_cond_t* cond() const;

	void setDataChanged(bool flag);
	bool dataChanged() const;

	char* dataBegin() const;

	int m_memId;
	char* m_ptr;
};

class ConnectionPrivate {
public:
	ConnectionPrivate(Connection* conn, bool create);

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	Connection* m_conn;
	SharedBuffer* m_memManager;
};
