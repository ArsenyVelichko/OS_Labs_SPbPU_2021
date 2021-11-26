#pragma once

#include <queue>
#include <atomic>

#include "Singleton.h"
#include "Runnable.h"

class ClientListener : public Runnable {
public:
	//timeout in milliseconds
	ClientListener(int handshakeSignal, int timeout);

	void accept(int id);
	bool empty() const;

	void cancel();
	void run() override;

	void waitForClient();

private:
	void clientArrived();

	mutable pthread_mutex_t m_mutex;
	pthread_cond_t m_clientArrived;
	std::atomic_bool m_isCanceled;

	std::queue<int> m_waitingClients;
	int m_handshakeSignal;
	timespec m_timeout;
};
