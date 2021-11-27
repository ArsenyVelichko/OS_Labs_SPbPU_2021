#pragma once

#include "Runnable.h"
#include "Timer.h"
#include "ClientListener.h"

class GameEngine : public Runnable {
public:
	GameEngine();
	~GameEngine() override;

	void run() override;

private:
	void makeTurn();
	void waitTurn();

	Timer* m_gameTimer;

	pthread_mutex_t m_mutex;
	pthread_cond_t m_canMakeTurn;
};
