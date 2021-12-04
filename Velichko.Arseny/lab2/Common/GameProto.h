#pragma once

namespace GameProto {

	enum PlayerStatus {
		Status_Dead,
		Status_Alive,
	};

	union MessageData {
		int clientValue;
		PlayerStatus playerStatus;
	};

	enum State {
		GameFinished = 0x1,
		ClientFinished = 0x2,
	};

	struct Message {
		MessageData data;
		uint8_t state;
	};
}