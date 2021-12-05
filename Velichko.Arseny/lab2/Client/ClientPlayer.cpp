#include <stdexcept>

#include "ClientPlayer.h"
#include "GameProto.h"
#include "GameDefines.h"
#include "Logger.h"

using namespace GameDefines;

ClientPlayer::ClientPlayer(int id) :
	Player(id, ClientWaitTime, Connection::Client),
	m_randGenerator(new RandGenerator(AliveMinValue, AliveMaxValue)) {}

void ClientPlayer::run() {
	GameProto::Message msg = {};

	while (true) {
		try {
			int clientValue = m_randGenerator->generate();
			msg.data.clientValue = clientValue;
			writeMessage(msg);

			readMessage(msg);
			if (msg.state & GameProto::GameFinished) { break; }
			updateStatus(msg.data.playerStatus);

		} catch (std::exception& e) {
			log_error(e.what());
			break;
		}
	}
}

void ClientPlayer::updateStatus(PlayerStatus newStatus) {
	if (status() == newStatus) { return; }

	switch (newStatus) {
		case GameProto::Status_Alive:
			m_randGenerator->setRange(AliveMinValue, AliveMaxValue);
			break;

		case GameProto::Status_Dead:
			m_randGenerator->setRange(DeadMinValue, DeadMaxValue);
			break;

		default:
			log_wtf("Unknown player status");
			break;
	}
}