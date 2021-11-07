#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

int main(void) {
	int socketHandler = socket(PF_UNIX, SOCK_STREAM, 0);

	struct sockaddr_un servAddr;

	servAddr.sun_family = AF_UNIX;
	strcpy(servAddr.sun_path, SOCKET_NAME);
	socklen_t addrLen = sizeof(servAddr.sun_family) + strlen(servAddr.sun_path);
	unlink(SOCKET_NAME);

	if (bind(socketHandler,
			 reinterpret_cast<const sockaddr*>(&servAddr),
			 addrLen) < 0) {
		perror("Bind error");
		return 2;
	}

	listen(socketHandler, 1);

	struct sockaddr_un clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	int clientSocket = accept(socketHandler,
							  reinterpret_cast<sockaddr*>(&clientAddr),
							  &clientAddrSize);

	char buf[1024];
	read(clientSocket, buf, sizeof(buf));

	std::cout << buf << std::endl;
	return 0;
}