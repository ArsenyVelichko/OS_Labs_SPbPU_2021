#include <iostream>
#include <Logger.h>
#include "HostPlayer.h"
#include "GameEngine.h"
#include "ByteArray.h"

HostPlayer::HostPlayer(int id, const SharedControlBlock& controlBlock) :
	Player(id, 10000, true), m_controlBlock(controlBlock) {}

void HostPlayer::run() {

	while (true) {
		try {
			int gameValue = m_controlBlock->takeGameValue();
			log_info("Read started");
			if (gameValue == EndGameValue) {
				writeAnswer(EndGameValue);
				break;
			}

			int clientValue = readClientValue();
			log_info(std::to_string(clientValue));
			updateStatus(gameValue, clientValue);

			Status newStatus = updateStatus(gameValue, clientValue);
			writeAnswer(newStatus);
			if (newStatus == Status_Alive) {
				log_info("Player survived");
				m_controlBlock->playerSurvived();
			}

		} catch (std::exception& e) {
			log_error(e.what());
			break;
		}
	}

	m_controlBlock->playerLeft();
}

int HostPlayer::readClientValue() {
	size_t size = sizeof(int);
	char data[size];

	if (conn()->read(data, size) != size) {
		throw std::runtime_error("Invalid read");
	}
	return ByteArray(data, size).toInt();
}

void HostPlayer::writeAnswer(char answer) {
	size_t size = 1;

	if (conn()->write(&answer, size) != size) {
		throw std::runtime_error("Invalid write");
	}
}

Player::Status HostPlayer::updateStatus(int gameValue, int clientValue) {
	Status currStatus = status();
	Status newStatus = currStatus;

	int diff = abs(gameValue - clientValue);

	if (currStatus == Status_Alive && !hideCondition(diff)) {
		newStatus = Status_Dead;

	} else if (currStatus == Status_Dead && resurrectCondition(diff)) {
		newStatus = Status_Alive;
	}

	setStatus(newStatus);
	return newStatus;
}

bool HostPlayer::hideCondition(int diff) const {
	return diff <= 70 / m_controlBlock->playersCount();
}

bool HostPlayer::resurrectCondition(int diff) const {
	return diff <= 20 / m_controlBlock->playersCount();
}
