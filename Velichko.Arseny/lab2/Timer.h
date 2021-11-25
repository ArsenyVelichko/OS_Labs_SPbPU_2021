#pragma once

#include <csignal>
#include <ctime>
#include <functional>

class Timer {
public:
	Timer();
	~Timer();

	using Callback = std::function<void ()>;
	void callOnTimeout(const Callback& callback);

	//interval in milliseconds
	void start(int interval);
	void stop();

private:
	static void onTimeout(int signal, siginfo_t* sigInfo, void* context);

	static const int TIMEOUT_SIGNAL;
	timer_t m_id;
	Callback m_callback;
};
