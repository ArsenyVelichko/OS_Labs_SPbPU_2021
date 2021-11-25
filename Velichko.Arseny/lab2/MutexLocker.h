#pragma once

#include <pthread.h>

class MutexLocker {
public:
	explicit MutexLocker(pthread_mutex_t* mutex);
	~MutexLocker();

private:
	pthread_mutex_t* m_mutex;
};
