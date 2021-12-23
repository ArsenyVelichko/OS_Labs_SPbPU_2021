#include <iostream>
#include <numeric>

#include "Logger.h"

#include "FineGrainedSet.h"
#include "ReadWriteTester.h"

void testSuite() {
	ReadWriteTester<FineGrainedSet, int> tester;

	size_t size = 200;
	std::vector<int> data(size);
	std::iota(data.begin(), data.end(), 0);

	tester.setData(data);
	tester.startWriters(2);
	tester.startReaders(5);
	tester.waitForTestsEnd();

	if (tester.container().empty()) {
		log_info("Test completed");
	}
}

int main(void) {
	Logger::create();

	testSuite();

	return 0;
}