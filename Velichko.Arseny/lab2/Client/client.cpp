#include <sys/socket.h>
#include <cstdio>

#include <iostream>
#include <sstream>
#include <csignal>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/poll.h>

#include "../Connections/Connection.h"
#include "ThreadPool.h"
#include "Logger.h"
#include "ClientPlayer.h"

int main(int argc, char** argv) {
//	const char* name = "pipe";
//	mkfifo(name, 0666);
//
//	int fd = open(name, O_RDWR | O_NONBLOCK);
//	size_t writed = write(fd, "Loh", 4);
//	pollfd pollHandler = { fd, 0 };
//	//poll(&pollHandler, 1, -1);
//
//
//	close(fd);

	Logger::create();

	if (argc != 2) {
		log_error("Wrong arguments number");
		return EXIT_FAILURE;
	}

	std::istringstream iss(argv[1]);
	int hostPid;
	iss >> hostPid;
	sigval val = {};
	val.sival_int = getpid();
	sigqueue(hostPid, SIGUSR1, val);

	sigset_t sigSet;
	sigemptyset(&sigSet);
	sigaddset(&sigSet, SIGUSR1);

	auto threadPool = new ThreadPool;
	threadPool->setBlockMask(&sigSet);

	siginfo_t sigInfo;
	timespec timeout = {};
	timeout.tv_sec = 5;
	if (sigtimedwait(&sigSet, &sigInfo, &timeout) == -1) {
		log_error("No signal");
		return EXIT_FAILURE;
	}
	int id = sigInfo.si_value.sival_int;
	log_info(std::to_string(id));

	threadPool->start(new ClientPlayer(id));
	threadPool->join();

	return 0;
}