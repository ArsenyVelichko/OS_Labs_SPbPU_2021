#include <algorithm>
#include <csignal>

#include "ThreadPool.h"
#include "MutexLocker.h"
#include "Logger.h"

void* ThreadPool::onStartThread(void* arg) {
	auto runnableInfo = reinterpret_cast<RunnableInfo*>(arg);
	Runnable* runnable = runnableInfo->runnable;
	ThreadPool* pool = runnableInfo->pool;

	runnable->run();
	if (runnable->autoDelete()) {
		delete runnable;
	}

	pool->removeThread(pthread_self());
	delete runnableInfo;
	return nullptr;
}

void ThreadPool::start(Runnable* runnable) {
	pthread_t thread;
	auto info = new RunnableInfo{ this, runnable };
	pthread_create(&thread, nullptr, onStartThread, info);

	MutexLocker locker(&m_mutex);
	m_threads.push_back(thread);
}

void ThreadPool::join() {
	size_t currSize;
	size_t i = 0;

	do {
		pthread_t thread;

		MutexLocker locker(&m_mutex);
		currSize = m_threads.size();
		thread = m_threads[i];
		locker.unlock();

		pthread_join(thread, nullptr);
		i++;

	} while (i < currSize);
}

void ThreadPool::removeThread(pthread_t thread) {
	MutexLocker locker(&m_mutex);
	std::remove(m_threads.begin(), m_threads.end(), thread);
}

void ThreadPool::setBlockMask(const SharedSiSet& set) {
	pthread_sigmask(SIG_BLOCK, set.get(), nullptr);
}
