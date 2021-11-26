#include <csignal>

#include "ClientListener.h"
#include "MutexLocker.h"
#include "TimeUtils.h"

ClientListener::ClientListener(int handshakeSignal, int timeout) :
	m_isCanceled(false),
	m_handshakeSignal(handshakeSignal),
    m_timeout(TimeUtils::msecToTimespec(timeout)) {

	pthread_mutex_init(&m_mutex, nullptr);
}

void ClientListener::accept(int id) {
	MutexLocker locker(&m_mutex);
	int clientPid = m_waitingClients.front();
	kill(clientPid, m_handshakeSignal);
	m_waitingClients.pop();
}

bool ClientListener::empty() const {
	MutexLocker locker(&m_mutex);
	return m_waitingClients.empty();
}

void ClientListener::run() {
	sigset_t blockMask;
	sigemptyset(&blockMask);
	sigaddset(&blockMask, m_handshakeSignal);

	while (!m_isCanceled) {
		siginfo_t sigInfo;
		if (sigtimedwait(&blockMask, &sigInfo, &m_timeout) == -1) {
			continue;
		}

		clientArrived();
	}
}

void ClientListener::cancel() {
	m_isCanceled = true;
}

void ClientListener::waitForClient() {
	MutexLocker locker(&m_mutex);
	pthread_cond_wait(&m_clientArrived, &m_mutex);
}

void ClientListener::clientArrived() {
	MutexLocker locker(&m_mutex);
	pthread_cond_signal(&m_clientArrived);
}