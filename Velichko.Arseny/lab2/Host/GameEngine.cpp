#include <iostream>

#include "Logger.h"
#include "GameEngine.h"
#include "MultiThreading/MutexLocker.h"
#include "HostPlayer.h"

GameEngine::GameEngine() :
	m_gameTimer(new Timer),
	m_randGenerator(new RandGenerator(0, 100)),
	m_controlBlock(new GameControlBlock),
	m_gameThreadPool(new ThreadPool) {

	auto siSet = SignalUtils::createSiSet({ SIGUSR1 });
	m_signalListener = new SignalListener(siSet, 5000);
	m_signalListener->setAutoDelete(false);

	ThreadPool::setBlockMask(siSet);

	auto makeTurnCallback = [this] () {
		makeTurn();
	};
	m_gameTimer->callOnTimeout(makeTurnCallback);
}

void GameEngine::exec() {
	m_gameThreadPool->start(m_signalListener);
	m_signalListener->waitForSignal();

	int turnsWithoutAlive = 0;
	m_playerId = 0;
	m_gameTimer->start(1000);

	while (turnsWithoutAlive < 2) {
		processSignals();
		log_info("Turn started");
		m_controlBlock->waitAllPlayers();

		int newGameValue = m_randGenerator->generate();
		m_controlBlock->setGameValue(newGameValue);
		waitTurn();

		if (m_controlBlock->aliveCount() == 0) {
			turnsWithoutAlive++;
		} else {
			turnsWithoutAlive = 0;
		}
	}

	m_controlBlock->setGameValue(EndGameValue);
	m_signalListener->cancel();
}

void GameEngine::processSignals() {
	while (!m_signalListener->empty()) {
		auto siInfo = m_signalListener->accept();
		int clientPid = siInfo->si_value.sival_int;

		sigval answerVal = {};
		answerVal.sival_int = addPlayer();
		sigqueue(clientPid, SIGUSR1, answerVal);
	}
}

void GameEngine::makeTurn() {
	MutexLocker locker(&m_mutex);
	pthread_cond_signal(&m_canMakeTurn);
}

GameEngine::~GameEngine() {
	delete m_gameTimer;
	delete m_randGenerator;

	m_gameThreadPool->join();
	delete m_gameThreadPool;
	delete m_signalListener;
}

void GameEngine::waitTurn() {
	MutexLocker locker(&m_mutex);
	pthread_cond_wait(&m_canMakeTurn, &m_mutex);
}

int GameEngine::addPlayer() {
	m_gameThreadPool->start(new HostPlayer(m_playerId, m_controlBlock));
	m_controlBlock->playerJoined();
	return m_playerId++;
}

