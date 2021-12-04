#include "Logger.h"
#include "SignalListener.h"
#include "MultiThreading/MutexLocker.h"
#include "TimeUtils.h"


SignalListener::SignalListener(const SharedSiSet& signalSet, int timeout)
	: m_timeout(TimeUtils::msecToTimespec(timeout)), m_signalSet(signalSet) {}

SharedSiInfo SignalListener::accept() {
	MutexLocker locker(&m_mutex);
	SharedSiInfo siInfo = m_waitingSignals.front();
	m_waitingSignals.pop();
	return siInfo;
}

bool SignalListener::empty() const {
	MutexLocker locker(&m_mutex);
	return m_waitingSignals.empty();
}

void SignalListener::run() {
	while (!m_isCanceled) {
		log_info("Listening");
		SharedSiInfo siInfo = std::make_shared<siginfo_t>();
		if (sigtimedwait(m_signalSet.get(), siInfo.get(), &m_timeout) == -1) {
			continue;
		}

		MutexLocker locker(&m_mutex);
		m_waitingSignals.push(siInfo);
	}
}

void SignalListener::cancel() {
	m_isCanceled = true;
}

void SignalListener::waitForClient() {
	MutexLocker locker(&m_mutex);
	log_info("Waiting");
	pthread_cond_wait(&m_clientArrived, &m_mutex);
}
