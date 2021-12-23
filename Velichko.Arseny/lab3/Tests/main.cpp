#include <iostream>
#include <numeric>

#include "Logger.h"

#include "FineGrainedSet.h"
#include "ReadWriteTester.h"

void writersTestSuite() {
	ReadWriteTester<FineGrainedSet, int> tester;

	size_t size = 800;
	std::vector<int> data(size);
	std::iota(data.begin(), data.end(), 0);

	tester.setData(data);
	tester.startWriters(5);
	tester.waitForTestsEnd();

	const auto& container = tester.container();
	for (int val : data) {
		if (!container.contains(val)) {
			log_error("Value " + std::to_string(val) + "wasn't written");
		}
	}
}

void readersTestSuite() {
	ReadWriteTester<FineGrainedSet, int> tester;

	size_t size = 800;
	std::vector<int> data(size);
	std::iota(data.begin(), data.end(), 0);

	tester.setData(data);
	tester.forwardData();
	tester.startReaders(5);
	tester.waitForTestsEnd();

	if (tester.container().empty()) {
		log_info("Test completed");
	}
}

void commonTestSuite() {
	ReadWriteTester<FineGrainedSet, int> tester;

	size_t size = 800;
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

	writersTestSuite();
	readersTestSuite();
	commonTestSuite();

	return 0;
}