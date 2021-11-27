#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include "../Connections/Connection.h"

int main(int argc, char** argv) {
//	int socketHandler = socket(PF_UNIX, SOCK_STREAM, 0);
//	if (socketHandler < 0) {
//		perror("Generate error");
//		return 1;
//	}
//
//	struct sockaddr_un servAddr;
//
//	servAddr.sun_family = AF_UNIX;
//	strcpy(servAddr.sun_path, SOCKET_NAME);
//	socklen_t addrLen = sizeof(servAddr.sun_family) + strlen(servAddr.sun_path);
//
//	if (connect(socketHandler,
//				reinterpret_cast<const sockaddr*>(&servAddr),
//				addrLen) < 0 ) {
//		perror("Connect error");
//		return 2;
//	}
//
//	const char data[] = "Hello from client.";
//	sleep(10);
//	write(socketHandler, data, sizeof(data));
//
//	close(socketHandler);

//	const char* name = "pipe";
//	mkfifo(name, 0666);
//
//	int fd = open(name, O_RDWR | O_NONBLOCK);
//	size_t writed = write(fd, "Loh", 4);
//	sleep(20);
//	close(fd);
	auto conn = new Connection(0, false);
	if (conn->write("Loh", 4) == -1) {
		perror("");
	}
	return 0;
}