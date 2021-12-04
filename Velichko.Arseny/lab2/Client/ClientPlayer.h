#pragma once

#include "Player.h"
#include "RandGenerator.h"
#include "Connection.h"

class ClientPlayer : public Player {
public:
	explicit ClientPlayer(int id);

	void run() override;

private:
	void writeClientValue(int value);
	void updateStatus(Status newStatus);
	char readAnswer();

	RandGenerator* m_randGenerator;
};
