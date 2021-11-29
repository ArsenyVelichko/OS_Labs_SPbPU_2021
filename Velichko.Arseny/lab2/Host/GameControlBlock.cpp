#include "GameControlBlock.h"
#include "MutexLocker.h"

void GameControlBlock::setGameValue(int value) {
	MutexLocker locker(&m_mutex);
	m_aliveCount = 0;
	m_gameValue = value;
	pthread_cond_broadcast(&m_gameValueFetched);
}

int GameControlBlock::takeGameValue() {
	MutexLocker locker(&m_mutex);
	m_playersReady++;

	if (m_playersReady == m_playersCount) {
		pthread_cond_signal(&m_allPlayersReady);
	}
	pthread_cond_wait(&m_gameValueFetched, &m_mutex);
	return m_gameValue;
}

void GameControlBlock::playerJoined() {
	m_playersCount++;
}

void GameControlBlock::playerSurvived() {
	m_aliveCount++;
}

void GameControlBlock::playerLeft() {
	MutexLocker locker(&m_mutex);
	m_playersCount--;

	if (m_playersReady == m_playersCount) {
		pthread_cond_signal(&m_allPlayersReady);
	}
}


uint32_t GameControlBlock::aliveCount() const {
	return m_aliveCount;
}

uint32_t GameControlBlock::playersCount() const {
	return m_playersCount;
}

void GameControlBlock::waitAllPlayers() {
	MutexLocker locker(&m_mutex);

	if (m_playersCount != m_playersReady) {
		pthread_cond_wait(&m_allPlayersReady, &m_mutex);
	}
	m_playersReady = 0;
}
