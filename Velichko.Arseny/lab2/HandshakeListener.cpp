#include <csignal>

#include "HandshakeListener.h"

HandshakeListener::HandshakeListener(int handshakeSignal, int timeout)
	: m_handshakeSignal(handshakeSignal), m_timeout(timeout) {
	struct sigaction act = {};
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = onClientSignal;

	sigaction(handshakeSignal, &act, nullptr);
}

void HandshakeListener::onClientSignal(int signal, siginfo_t* sigInfo, void* context) {
	auto instance = HandshakeListener::instance();
	instance->m_waitingClients.push(sigInfo->si_value.sival_int);
}

void HandshakeListener::accept(int id) {
	int clientPid = m_waitingClients.front();
	kill(clientPid, m_handshakeSignal);
	m_waitingClients.pop();
}

bool HandshakeListener::empty() const {
	return m_waitingClients.empty();
}
