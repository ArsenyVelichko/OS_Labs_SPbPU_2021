#pragma once

#include <atomic>

#include "Connection.h"
#include "MultiThreading/Runnable.h"
#include "GameProto.h"

using GameProto::PlayerStatus;

class Player : public Runnable {
public:
	Player(int id, int timeout, Connection::Role connRole);
	~Player() override;

	int id() const;
	PlayerStatus status() const;

protected:
	void writeMessage(const GameProto::Message& msg);
	void readMessage(GameProto::Message& msg);

	void setStatus(PlayerStatus status);

private:
	Connection* m_conn;
	PlayerStatus m_status = GameProto::Status_Alive;
};
