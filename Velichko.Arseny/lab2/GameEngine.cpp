#include <iostream>

#include "GameEngine.h"
#include "MutexLocker.h"

GameEngine::GameEngine() : m_gameTimer(new Timer) {
	pthread_mutex_init(&m_mutex, nullptr);
	pthread_cond_init(&m_canMakeTurn, nullptr);

	auto makeTurnCallback = [this] () {
		makeTurn();
	};
	m_gameTimer->callOnTimeout(makeTurnCallback);
}

void GameEngine::run() {
	m_gameTimer->start(1000);

	for (int i = 0; i < 5; i++) {
		MutexLocker locker(&m_mutex);
		pthread_cond_wait(&m_canMakeTurn, &m_mutex);
		std::cout << i << std::endl;
	}
}

void GameEngine::makeTurn() {
	MutexLocker locker(&m_mutex);
	pthread_cond_signal(&m_canMakeTurn);
}

GameEngine::~GameEngine() {
	delete m_gameTimer;
}
