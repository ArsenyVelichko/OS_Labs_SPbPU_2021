#include <stdexcept>

#include "ClientPlayer.h"
#include "Serialization.h"
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
			updateStatus(static_cast<Status>(answer));

		} catch (std::exception& e) {
			log_error(e.what());
			break;
		}
	}
}

void ClientPlayer::writeClientValue(int value) {
	size_t size = sizeof(int);
	char data[size];

	Serialization::serialize(data, value);
	if (conn()->write(data, size) < size) {
		throw std::runtime_error("Invalid write");
	}
}

void ClientPlayer::updateStatus(Status newStatus) {
	static std::vector<int> valueUpperBound = { 50, 100 };

	if (status() != newStatus) {
		m_randGenerator->setRange(0, valueUpperBound[newStatus]);
		setStatus(newStatus);
	}
}

char ClientPlayer::readAnswer() {
	size_t size = 1;
	char answer;

	if (conn()->read(&answer, size) < size) {
		throw std::runtime_error("Invalid read");
	}
	log_info("Status: " + std::to_string(static_cast<Status>(answer)));
	return answer;
}