#include <iostream>
#include "Player.h"
#include "GameEngine.h"

Player::Player(int id, const SharedControlBlock& controlBlock) :
	m_controlBlock(controlBlock), m_conn(new Connection(id, true)) {}

void Player::run() {
	while (true) {
		int gameValue = m_controlBlock->takeGameValue(id());
		if (gameValue == GameEngine::END_GAME_VALUE) { break; }


		std::cout << gameValue << std::endl;
	}
}

Player::~Player() {
	delete m_conn;
}

int Player::id() const {
	return m_conn->id();
}
