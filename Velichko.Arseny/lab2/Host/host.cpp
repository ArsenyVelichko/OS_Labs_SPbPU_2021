#include <csignal>
#include <unistd.h>
#include <iostream>
#include <poll.h>
#include <fcntl.h>

#include "Logger.h"
#include "ThreadPool.h"
#include "GameEngine.h"
#include "../Connections/Connection.h"

int main(void) {
//	const char* name = "pipe";
//
//	int fd = open(name, O_RDWR | O_NONBLOCK);
//	struct pollfd pollfds[] = {
//			{fd, POLLIN, 0},
//	};
//	poll(pollfds, 1, -1);
//	if (pollfds->revents & POLLIN) {
//		printf("Poll in \n");
//		char buf[20] = {};
//		read(fd, buf, sizeof(buf));
//		printf("%s", buf);
//	}
//
//	if (pollfds->revents & POLLHUP) {
//		printf("Poll hup \n");
//	}
//	close(fd);

	Logger::create();
	auto gameEngine = std::make_unique<GameEngine>();
	gameEngine->exec();
	return 0;
}