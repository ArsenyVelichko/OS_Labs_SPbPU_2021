#pragma once

#include "Runnable.h"
#include "Timer.h"

class GameEngine : public Runnable {
public:
	GameEngine();
	~GameEngine();

	void run() override;

private:
	void makeTurn();

	Timer* m_gameTimer;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_canMakeTurn;
};
