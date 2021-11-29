#pragma once

#include "ThreadPool.h"
#include "Runnable.h"
#include "Timer.h"
#include "ClientListener.h"
#include "GameControlBlock.h"
#include "RandGenerator.h"

class GameEngine {
public:
	GameEngine();
	~GameEngine();

	void exec();

private:
	void acceptClients();

	void makeTurn();
	void waitTurn();

	int m_playerId;
	Timer* m_gameTimer;
	SharedControlBlock m_controlBlock;
	ThreadPool* m_gameThreadPool;

	ClientListener* m_clientListener;
	RandGenerator* m_randGenerator;

	pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t m_canMakeTurn = PTHREAD_COND_INITIALIZER;
};
