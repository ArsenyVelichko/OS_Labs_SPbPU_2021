#include "MultiThreading/ThreadPool.h"
#include "Logger.h"
#include "ClientPlayer.h"
#include "HandshakeHelper.h"

int main(int argc, char** argv) {
	Logger::create();

	if (argc != 2) {
		log_error("Wrong arguments number");
		return EXIT_FAILURE;
	}

	int hostPid = std::stoi(argv[1]);

	HandshakeHelper helper(SIGUSR1);
	helper.sendRequest(hostPid);
	int id = helper.waitAnswer(5000);
	if (id == -1) { return EXIT_FAILURE; }

	auto threadPool = std::make_unique<ThreadPool>();
	threadPool->start(new ClientPlayer(id));
	threadPool->join();

	return 0;
}