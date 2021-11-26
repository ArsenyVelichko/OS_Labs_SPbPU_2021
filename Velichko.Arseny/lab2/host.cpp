#include <csignal>
#include <unistd.h>
#include <iostream>

#include "ThreadPool.h"
#include "GameEngine.h"

int main(void) {
//	int socketHandler = socket(PF_UNIX, SOCK_STREAM, 0);
//
//	struct sockaddr_un servAddr;
//
//	servAddr.sun_family = AF_UNIX;
//	strcpy(servAddr.sun_path, SOCKET_NAME);
//	socklen_t addrLen = sizeof(servAddr.sun_family) + strlen(servAddr.sun_path);
//	unlink(SOCKET_NAME);
//
//	if (bind(socketHandler,
//			 reinterpret_cast<const sockaddr*>(&servAddr),
//			 addrLen) < 0) {
//		perror("Bind error");
//		return 2;
//	}
//
//	listen(socketHandler, 1);
//
//	struct sockaddr_un clientAddr;
//	socklen_t clientAddrSize = sizeof(clientAddr);
//	int clientSocket = accept(socketHandler,
//							  reinterpret_cast<sockaddr*>(&clientAddr),
//							  &clientAddrSize);
//
//	char buf[1024];
//	read(clientSocket, buf, sizeof(buf));
//
//	std::cout << buf << std::endl;

//	const char* name = "pipe";
//
//	int fd = open(name, O_RDONLY | O_NONBLOCK);
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

//	sigset_t set;
//	sigemptyset(&set);
//	sigaddset(&set, SIGUSR1);
//	siginfo_t sigInfo;
//	timespec timeout = { 30 };
//	sigprocmask( SIG_BLOCK, &set, NULL );
//
//	if(sigtimedwait(&set, &sigInfo, &timeout) != SIGUSR1) {
//		perror("Sigwait error");
//	}

	ThreadPool::create();

	sigset_t blockMask;
	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGUSR1);

	ThreadPool::instance()->setBlockMask(&blockMask);

	auto* gameEngine = new GameEngine();
	ThreadPool::instance()->start(gameEngine);

	return 0;
}