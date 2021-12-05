#include <Logger.h>

#include "HostPlayer.h"
#include "GameEngine.h"

#include "GameProto.h"
#include "GameDefines.h"

using namespace GameDefines;

HostPlayer::HostPlayer(int id, const SharedControlBlock& controlBlock) :
	Player(id, HostWaitTime, Connection::Host), m_controlBlock(controlBlock) {}

void HostPlayer::run() {
	GameProto::Message msg = {};
	int gameValue;

	do {
		try {
			gameValue = m_controlBlock->waitGameValue();

			readMessage(msg);
			int clientValue = msg.data.clientValue;
			log_info(std::to_string(clientValue));

			if (gameValue == EndGameValue) {
				msg.state |= GameProto::GameFinished;
			}

			PlayerStatus newStatus = updateStatus(gameValue, clientValue);
			msg.data.playerStatus = newStatus;
			writeMessage(msg);

			if (newStatus == GameProto::Status_Alive) {
				m_controlBlock->playerSurvived();
			}

		} catch (std::exception& e) {
			log_error(e.what());
			break;
		}
	} while (gameValue != EndGameValue);

	m_controlBlock->playerLeft();
}

PlayerStatus HostPlayer::updateStatus(int gameValue, int clientValue) {
	using namespace GameProto;

	PlayerStatus currStatus = status();
	PlayerStatus newStatus = currStatus;

	int diff = abs(gameValue - clientValue);

	if (currStatus == Status_Alive && !hideCondition(diff)) {
		newStatus = Status_Dead;

	} else if (currStatus == Status_Alive && resurrectCondition(diff)) {
		newStatus = Status_Alive;
	}

	setStatus(newStatus);
	return newStatus;
}

bool HostPlayer::hideCondition(int diff) const {
	return diff <= HideCondValue / m_controlBlock->playersCount();
}

bool HostPlayer::resurrectCondition(int diff) const {
	return diff <= ResurrectCondValue / m_controlBlock->playersCount();
}
