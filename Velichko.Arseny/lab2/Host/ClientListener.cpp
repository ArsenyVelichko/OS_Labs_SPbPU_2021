#include <csignal>

#include "Logger.h"
#include "ClientListener.h"
#include "MutexLocker.h"
#include "TimeUtils.h"

ClientListener::ClientListener(int handshakeSignal, int timeout) :
	m_isCanceled(false),
	m_handshakeSignal(handshakeSignal),
    m_timeout(TimeUtils::msecToTimespec(timeout)) {}

void ClientListener::accept(int id) {
	MutexLocker locker(&m_mutex);
	int clientPid = m_waitingClients.front();

	sigval val = {};
	val.sival_int = id;
	sigqueue(clientPid, SIGUSR1, val);

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
		log_info("Listening");
		siginfo_t sigInfo;
		if (sigtimedwait(&blockMask, &sigInfo, &m_timeout) == -1) {
			continue;
		}

		appendClient(sigInfo.si_value.sival_int);
	}
}

void ClientListener::cancel() {
	m_isCanceled = true;
}

void ClientListener::waitForClient() {
	MutexLocker locker(&m_mutex);
	log_info("Waiting");
	pthread_cond_wait(&m_clientArrived, &m_mutex);
}

void ClientListener::appendClient(int pid) {
	MutexLocker locker(&m_mutex);
	m_waitingClients.push(pid);
	pthread_cond_signal(&m_clientArrived);
}