#pragma once

#include <queue>
#include <atomic>

#include "Singleton.h"
#include "Runnable.h"

class HandshakeListener : public Runnable {
public:
	//timeout in milliseconds
	HandshakeListener(int handshakeSignal, int timeout);

	void accept(int id);
	bool empty() const;

	void cancel();
	void run() override;

private:
	sigset_t signalSet() const;

	mutable pthread_mutex_t m_mutex;
	std::atomic_bool m_isCanceled;

	std::queue<int> m_waitingClients;
	int m_handshakeSignal;
	timespec m_timeout;
};
