#pragma once

#include <pthread.h>
#include <list>

#include "Runnable.h"
#include "Singleton.h"

class ThreadPool {
public:
	void start(Runnable* runnable);
	void join();

	void setBlockMask(const sigset_t* set);

private:
	static void* onStartThread(void* arg);
	void removeThread(pthread_t thread);

	struct RunnableInfo {
		ThreadPool* pool;
		Runnable* runnable;
	};

	std::list<pthread_t> m_threads;
};
