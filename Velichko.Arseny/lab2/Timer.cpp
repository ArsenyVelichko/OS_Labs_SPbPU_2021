#include <cmath>
#include <unistd.h>

#include "Timer.h"

const int Timer::TIMEOUT_SIGNAL = SIGRTMIN;

Timer::Timer() {
	sigevent timeoutEvent = {};
	timeoutEvent.sigev_notify = SIGEV_SIGNAL;
	timeoutEvent.sigev_signo = TIMEOUT_SIGNAL;
	timeoutEvent.sigev_value.sival_ptr = this;

	struct sigaction timeoutAction = {};
	timeoutAction.sa_flags = SA_SIGINFO;
	timeoutAction.sa_sigaction = onTimeout;

	sigaction(TIMEOUT_SIGNAL, &timeoutAction, nullptr);

	timer_create(CLOCK_REALTIME, &timeoutEvent, &m_id);
}

void Timer::onTimeout(int signal, siginfo_t* sigInfo, void* context) {
	auto timer = reinterpret_cast<Timer*>(sigInfo->si_value.sival_ptr);
	timer->m_callback();
}

void Timer::callOnTimeout(const Timer::Callback& callback) {
	m_callback = callback;
}

void Timer::start(int interval) {
	itimerspec timerSpec = {};
	timerSpec.it_value.tv_sec = interval / 1'000; //msec to sec conversion
	timerSpec.it_value.tv_nsec = interval % 1'000 * 1'000'000; //msec to nsec conversion

	timerSpec.it_interval.tv_sec = timerSpec.it_value.tv_sec;
	timerSpec.it_interval.tv_nsec = timerSpec.it_value.tv_nsec;

	timer_settime(m_id, 0, &timerSpec, nullptr);
}

Timer::~Timer() {
	timer_delete(m_id);
}

void Timer::stop() {
	itimerspec timerSpec = {};
	timer_settime(m_id, 0, &timerSpec, nullptr);
}
