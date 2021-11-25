#pragma once

#include <queue>
#include "Singleton.h"

class HandshakeListener : public Singleton<HandshakeListener> {
public:
	void accept(int id);
	bool empty() const;

protected:
	//timeout in seconds
	HandshakeListener(int handshakeSignal, int timeout);

private:
	static void onClientSignal(int signal, siginfo_t* sigInfo, void* context);

	std::queue<int> m_waitingClients;
	int m_handshakeSignal;
	int m_timeout;
};
