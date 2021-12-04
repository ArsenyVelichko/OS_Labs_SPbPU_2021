#include "Player.h"

Player::Player(int id, int timeout, Connection::Role connRole) :
	m_conn(new Connection(id, timeout, connRole)) {}

int Player::id() const {
	return m_conn->id();
}

PlayerStatus Player::status() const {
	return m_status;
}

void Player::setStatus(PlayerStatus status) {
	m_status = status;
}

Player::~Player() {
	delete m_conn;
}

void Player::writeMessage(const GameProto::Message& msg) {
	auto data = reinterpret_cast<const char*>(&msg);
	m_conn->write(data, sizeof(GameProto::Message));
}

void Player::readMessage(GameProto::Message& msg) {
	auto data = reinterpret_cast<char*>(&msg);
	m_conn->read(data, sizeof(GameProto::Message));
}
