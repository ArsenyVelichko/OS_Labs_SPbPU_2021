#include <stdexcept>

#include "ClientPlayer.h"
#include "ByteArray.h"
#include "Logger.h"

ClientPlayer::ClientPlayer(int id) :
	Player(id, 10000, false), m_randGenerator(new RandGenerator(0, 100)) {}

void ClientPlayer::run() {
	while (true) {
		try {
			int clientValue = m_randGenerator->generate();
			writeClientValue(clientValue);

			char answer = readAnswer();
			if (answer == Player::EndGameValue) { break; }
			setStatus(static_cast<Status>(answer));

		} catch (std::exception& e) {
			log_error(e.what());
			break;
		}
	}
}

void ClientPlayer::writeClientValue(int value) {
	ByteArray ba = ByteArray::fromInt(value);

	if (conn()->write(ba.constData(), ba.size()) != ba.size()) {
		throw std::runtime_error("Invalid write");
	}
}

char ClientPlayer::readAnswer() {
	size_t size = 1;
	char data[size];

	if (conn()->read(data, size) != size) {
		throw std::runtime_error("Invalid read");
	}
	log_info("Status: " + std::to_string(static_cast<Status>(data[0])));
	return data[0];
}
