#pragma once

#include <pthread.h>
#include <vector>

#include "Runnable.h"
#include "Singleton.h"
#include "Signals/SignalsUtils.h"

using SignalUtils::SharedSiSet;

class ThreadPool {
public:
	void start(Runnable* runnable);
	void join();

	static void setBlockMask(const SharedSiSet& set);

private:
	static void* onStartThread(void* arg);
	void removeThread(pthread_t thread);

	struct RunnableInfo {
		ThreadPool* pool;
		Runnable* runnable;
	};

	pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
	std::vector<pthread_t> m_threads;
};
