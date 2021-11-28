#pragma once

#include "Runnable.h"
#include "Timer.h"
#include "ClientListener.h"
#include "GameControlBlock.h"
#include "RandGenerator.h"

class GameEngine : public Runnable {
public:
	GameEngine();
	~GameEngine() override;

	void run() override;

	static constexpr int END_GAME_VALUE = -1;

private:
	int gameValue() const;

	void makeTurn();
	void waitTurn();

	Timer* m_gameTimer;
	SharedControlBlock m_controlBlock;
	RandGenerator* m_randGenerator;

	pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t m_canMakeTurn = PTHREAD_COND_INITIALIZER;
};
