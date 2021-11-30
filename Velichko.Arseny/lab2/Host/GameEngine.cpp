#include <iostream>

#include "Logger.h"
#include "GameEngine.h"
#include "MutexLocker.h"
#include "ThreadPool.h"
#include "HostPlayer.h"

GameEngine::GameEngine() :
	m_gameTimer(new Timer),
	m_randGenerator(new RandGenerator(0, 100)),
	m_clientListener(new ClientListener(SIGUSR1, 5000)),
	m_controlBlock(new GameControlBlock),
	m_gameThreadPool(new ThreadPool) {

	sigset_t blockMask;
	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGUSR1);

	m_gameThreadPool->setBlockMask(&blockMask);
	m_clientListener->setAutoDelete(false);

	auto makeTurnCallback = [this] () {
		makeTurn();
	};

	m_gameTimer->callOnTimeout(makeTurnCallback);
}

void GameEngine::exec() {
	m_gameThreadPool->start(m_clientListener);
	m_clientListener->waitForClient();
	log_info("Client arrived");
	int turnsWithoutAlive = 0;
	m_playerId = 0;
	m_gameTimer->start(1000);

	while (turnsWithoutAlive < 2) {
		log_info("Turn started");
		acceptClients();
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

	m_controlBlock->setGameValue(Player::EndGameValue);
	m_clientListener->cancel();
}

void GameEngine::acceptClients() {
	while (!m_clientListener->empty()) {
		m_gameThreadPool->start(new HostPlayer(m_playerId, m_controlBlock));

		m_controlBlock->playerJoined();
		m_clientListener->accept(m_playerId);
		m_playerId++;
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
	delete m_clientListener;
}

void GameEngine::waitTurn() {
	MutexLocker locker(&m_mutex);
	pthread_cond_wait(&m_canMakeTurn, &m_mutex);
}

