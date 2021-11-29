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
	if (pthread_create(&thread, nullptr, onStartThread, info) != 0) {
		log_error("Pthread create failed");
		return;
	}

	m_threads.push_back(thread);
}

void ThreadPool::join() {
	for (auto& thread : m_threads) {
		pthread_join(thread, nullptr);
	}
}

void ThreadPool::removeThread(pthread_t thread) {
	std::remove(m_threads.begin(), m_threads.end(), thread);
}

void ThreadPool::setBlockMask(const sigset_t* set) {
	pthread_sigmask(SIG_BLOCK, set, nullptr);
}
