#pragma once

#include "Runnable.h"
#include "GameControlBlock.h"
#include "../Connections/Connection.h"

class Player : public Runnable {
public:
	Player(int id, const SharedControlBlock& controlBlock);
	~Player() override;

	int id() const;
	void run() override;

private:
	SharedControlBlock m_controlBlock;
	Connection* m_conn;
	bool m_isAlive = true;
};
