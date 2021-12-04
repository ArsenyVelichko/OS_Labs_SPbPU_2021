#include <stdexcept>

#include "ClientPlayer.h"
#include "GameProto.h"
#include "Logger.h"

ClientPlayer::ClientPlayer(int id) :
	Player(id, 20000, Connection::Client), m_randGenerator(new RandGenerator(0, 100)) {}

void ClientPlayer::run() {
	GameProto::Message msg = {};

	while (true) {
		try {
			int clientValue = m_randGenerator->generate();
			msg.data.clientValue = clientValue;
			writeMessage(msg);

			if (msg.state & GameProto::ClientFinished) { break; }

			readMessage(msg);
			updateStatus(msg.data.playerStatus);

			if (msg.state & GameProto::GameFinished) {
				msg.state |= GameProto::ClientFinished;
			}

		} catch (std::exception& e) {
			log_error(e.what());
			break;
		}
	}
}

void ClientPlayer::updateStatus(PlayerStatus newStatus) {
	static std::vector<int> valueUpperBound = { 50, 100 };

	log_info("New status: " + std::to_string(newStatus));

	if (status() != newStatus) {
		m_randGenerator->setRange(0, valueUpperBound[newStatus]);
		setStatus(newStatus);
	}
}