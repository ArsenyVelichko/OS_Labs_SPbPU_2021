#pragma once

#include <memory>
#include <pthread.h>
#include <atomic>
#include <map>

class GameControlBlock {
public:
	void setGameValue(int value);
	int takeGameValue(int id);

	void incAliveCount();
	uint32_t aliveCount() const;

private:
	pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t m_gameValueFetched = PTHREAD_COND_INITIALIZER;

	std::map<int, bool> m_fetchedIds;

	int m_gameValue;
	std::atomic_uint32_t m_aliveCount;
};

using SharedControlBlock = std::shared_ptr<GameControlBlock>;
