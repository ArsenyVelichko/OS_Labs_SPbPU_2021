#include <iostream>
#include <numeric>

#include "Logger.h"

#include "FineGrainedSet.h"
#include "ReadWriteTester.h"

void writersTestSuite() {
	ReadWriteTester<FineGrainedSet, int> tester;

	size_t size = 200;
	std::vector<int> data(size);
	std::iota(data.begin(), data.end(), 0);

	size_t chunkNum = 10;
	std::vector<size_t> chunkSizes(chunkNum);

	size_t chunkSize = size / chunkNum;
	auto uniformStep = [chunkSize, i = 0] () mutable {
		return chunkSize * i++;
	};
	std::generate(chunkSizes.begin(), chunkSizes.end(), uniformStep);

	tester.startWriters(data, chunkSizes);

	const auto& container = tester.container();
	for (int val : data) {
		if (!container.contains(val)) {
			log_error("Value " + std::to_string(val) + " wasn't written");
		}
	}
}

int main(void) {
	Logger::create();

	writersTestSuite();

	return 0;
}