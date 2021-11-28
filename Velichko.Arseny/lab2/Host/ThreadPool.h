#pragma once

#include <pthread.h>
#include <list>

#include "Runnable.h"
#include "Singleton.h"

class ThreadPool : public Singleton<ThreadPool> {
public:
	void start(Runnable* runnable);
	void join();

	void setBlockMask(const sigset_t* set);

protected:
	ThreadPool() = default;
	~ThreadPool();

private:
	static void* onStartThread(void* arg);

	void removeThread(pthread_t thread);

	struct RunnableInfo {
		ThreadPool* pool;
		Runnable* runnable;
	};

	pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
	std::list<pthread_t> m_threads;
};
