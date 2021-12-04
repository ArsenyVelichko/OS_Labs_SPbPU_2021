#include "Logger.h"
#include "GameEngine.h"

int main(void) {
	Logger::create();
	auto gameEngine = std::make_unique<GameEngine>();
	gameEngine->exec();
	return 0;
}