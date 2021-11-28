#include "GameControlBlock.h"
#include "MutexLocker.h"

void GameControlBlock::setGameValue(int value) {
	MutexLocker locker(&m_mutex);
	m_gameValue = value;

	for (auto& pair : m_fetchedIds) {
		pair.second = true;
	}
	pthread_cond_broadcast(&m_gameValueFetched);
}

int GameControlBlock::takeGameValue(int id) {
	MutexLocker locker(&m_mutex);

	if (m_fetchedIds.find(id) == m_fetchedIds.end()) {
		m_fetchedIds[id] = true;
	}

	if (!m_fetchedIds[id]) {
		pthread_cond_wait(&m_gameValueFetched, &m_mutex);
	}
	m_fetchedIds[id] = false;
	return m_gameValue;
}

void GameControlBlock::incAliveCount() {
	m_aliveCount++;
}

uint32_t GameControlBlock::aliveCount() const {
	return m_aliveCount;
}