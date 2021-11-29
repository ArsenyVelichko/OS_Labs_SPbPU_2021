#pragma once

#include "Player.h"
#include "RandGenerator.h"
#include "../Connections/Connection.h"

class ClientPlayer : public Player {
public:
	explicit ClientPlayer(int id);

	void run() override;

private:
	void writeClientValue(int value);
	void updateStatus(Status newStatus);
	char readAnswer();

	int m_clientValue;
	RandGenerator* m_randGenerator;
};
