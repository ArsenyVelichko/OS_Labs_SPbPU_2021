#pragma once

#include "Runnable.h"
#include "GameControlBlock.h"
#include "Player.h"
#include "../Connections/Connection.h"

class HostPlayer : public Player {
public:
	HostPlayer(int id, const SharedControlBlock& controlBlock);

	void run() override;

private:
	int readClientValue();
	Status updateStatus(int gameValue, int clientValue);
	void writeAnswer(char answer);

	SharedControlBlock m_controlBlock;

	bool hideCondition(int diff) const;
	bool resurrectCondition(int diff) const;
};
