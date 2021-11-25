#include <csignal>
#include <iostream>

#include "HandshakeListener.h"
#include "MutexLocker.h"
#include "TimeUtils.h"

HandshakeListener::HandshakeListener(int handshakeSignal, int timeout) :
	m_isCanceled(false),
	m_handshakeSignal(handshakeSignal),
    m_timeout(TimeUtils::msecToTimespec(timeout)) {

	pthread_mutex_init(&m_mutex, nullptr);

	sigset_t lockMask = signalSet();
	sigprocmask(SIG_BLOCK, &lockMask, nullptr);
}

void HandshakeListener::accept(int id) {
	MutexLocker locker(&m_mutex);
	int clientPid = m_waitingClients.front();
	kill(clientPid, m_handshakeSignal);
	m_waitingClients.pop();
}

bool HandshakeListener::empty() const {
	MutexLocker locker(&m_mutex);
	return m_waitingClients.empty();
}

void HandshakeListener::run() {
	sigset_t lockMask = signalSet();

	while (!m_isCanceled) {
		siginfo_t sigInfo;
		if (sigtimedwait(&lockMask, &sigInfo, &m_timeout) == -1) {
			perror("");
			break;
		}
		std::cout << "Success" << std::endl;
	}
}

void HandshakeListener::cancel() {
	m_isCanceled = true;
}

sigset_t HandshakeListener::signalSet() const {
	sigset_t lockMask;
	sigemptyset(&lockMask);
	sigaddset(&lockMask, m_handshakeSignal);
	return lockMask;
}
