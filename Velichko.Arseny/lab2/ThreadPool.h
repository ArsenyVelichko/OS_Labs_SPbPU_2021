#pragma once

#include <pthread.h>
#include <atomic>
#include <list>

#include "Runnable.h"

class ThreadPool {
public:
	ThreadPool();
    ~ThreadPool();

	void start(Runnable* runnable);
	void join();

private:
	static void* onStartThread(void* arg);

	void removeThread(pthread_t thread);

	struct RunnableInfo {
		ThreadPool* pool;
		Runnable* runnable;
	};

	pthread_mutex_t m_mutex;
	std::list<pthread_t> m_threads;
};
