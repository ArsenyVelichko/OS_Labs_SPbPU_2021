#include <iostream>

#include "GameEngine.h"
#include "MutexLocker.h"
#include "ThreadPool.h"
#include "Player.h"

GameEngine::GameEngine() :
	m_gameTimer(new Timer),
	m_controlBlock(new GameControlBlock),
	m_randGenerator(new RandGenerator(0, 100)) {

	auto makeTurnCallback = [this] () {
		makeTurn();
	};
	m_gameTimer->callOnTimeout(makeTurnCallback);
}

void GameEngine::run() {
	auto threadPool = ThreadPool::instance();
	auto clientListener = new ClientListener(SIGUSR1, 5000);

	threadPool->start(clientListener);
	//clientListener->waitForClient();

	int turnsWithoutAlive = 0;
	int playerId = 0;

	m_gameTimer->start(500);
	while (turnsWithoutAlive < 2) {
		//while (!clientListener->empty()) {
			threadPool->start(new Player(playerId, m_controlBlock));
			playerId++;
		//	clientListener->accept(0);
		//}

		m_controlBlock->setGameValue(m_randGenerator->generate());
		waitTurn();

		if (m_controlBlock->aliveCount() == 0) {
			turnsWithoutAlive++;
		} else {
			turnsWithoutAlive = 0;
		}
	}

	m_controlBlock->setGameValue(END_GAME_VALUE);
	clientListener->cancel();
}

void GameEngine::makeTurn() {
	MutexLocker locker(&m_mutex);
	pthread_cond_signal(&m_canMakeTurn);
}

GameEngine::~GameEngine() {
	delete m_gameTimer;
	delete m_randGenerator;
}

void GameEngine::waitTurn() {
	MutexLocker locker(&m_mutex);
	pthread_cond_wait(&m_canMakeTurn, &m_mutex);
}
