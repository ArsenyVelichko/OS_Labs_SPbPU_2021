#include "Player.h"

Player::Player(int id, int timeout, bool create) :
	m_conn(new Connection(id, timeout, create)) {}

int Player::id() const {
	return m_conn->id();
}

Player::Status Player::status() const {
	return m_status;
}

void Player::setStatus(Player::Status status) {
	m_status = status;
}

Player::~Player() {
	delete m_conn;
}

Connection* Player::conn() const {
	return m_conn;
}
