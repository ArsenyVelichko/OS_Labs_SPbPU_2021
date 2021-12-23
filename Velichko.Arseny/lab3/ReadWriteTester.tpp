#include "Writer.h"

template<template<class> class Container, class ValueType>
ReadWriteTester<Container, ValueType>::ReadWriteTester() :
	m_container(new Container<ValueType>), m_threadPool(new ThreadPool) {}

template<template<class> class Container, class ValueType>
ReadWriteTester<Container, ValueType>::~ReadWriteTester() {
	delete m_container;
	delete m_threadPool;
}

template<template<class> class Container, class ValueType>
void ReadWriteTester<Container, ValueType>::startWriters(const std::vector<ValueType>& data,
														 const std::vector<size_t>& chunkSizes) {
	auto chunkBegin = data.begin();
	for (int i = 0; chunkBegin < data.end() && i < chunkSizes.size(); i++) {
		auto chunkEnd = std::min(chunkBegin + chunkSizes[i], data.end());
		std::vector<ValueType> chunk(chunkBegin, chunkEnd);

		auto writer = new Writer<Container, ValueType>(m_container);
		writer->setDataToWrite(chunk);
		m_threadPool->start(writer);

		chunkBegin = chunkEnd;
	}
}

template<template<class> class Container, class ValueType>
const Container<ValueType>& ReadWriteTester<Container, ValueType>::container() const {
	return *m_container;
}
