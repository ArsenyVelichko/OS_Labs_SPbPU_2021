#include "TimeUtils.h"

timespec TimeUtils::msecToTimespec(int msec) {
	timespec timeSpec = {};
	timeSpec.tv_sec = msec / 1'000; //msec to sec conversion
	timeSpec.tv_nsec = msec % 1'000 * 1'000'000; //msec to nsec conversion
	return timeSpec;
}