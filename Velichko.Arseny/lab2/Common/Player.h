#pragma once

#include <atomic>

#include "Runnable.h"
#include "../Connections/Connection.h"

class Player : public Runnable {
public:
	Player(int id, int timeout, bool create);
	~Player() override;

	enum Status : char {
		Status_Dead,
		Status_Alive,
	};

	//Value to inform player, that game has been ended
	static constexpr int EndGameValue = -1;

	int id() const;
	Status status() const;

protected:
	Connection* conn() const;
	void setStatus(Status status);

private:
	Connection* m_conn;
	Status m_status = Status_Alive;
};