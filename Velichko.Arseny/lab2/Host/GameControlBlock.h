#pragma once

#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <atomic>
#include <map>

class GameControlBlock {
public:
	void playerJoined();
	void playerSurvived();
	void playerLeft();

	uint32_t aliveCount() const;
	uint32_t playersCount() const;

	void setGameValue(int value);
	int takeGameValue();

	void waitAllPlayers();

private:
	void checkForAllReady();

	pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t m_gameValueFetched = PTHREAD_COND_INITIALIZER;
	pthread_cond_t m_allPlayersReady = PTHREAD_COND_INITIALIZER;

	int m_gameValue;
	std::atomic_uint32_t m_playersCount = 0;
	std::atomic_uint32_t m_playersReady = 0;
	std::atomic_uint32_t m_aliveCount = 0;
};

using SharedControlBlock = std::shared_ptr<GameControlBlock>;
