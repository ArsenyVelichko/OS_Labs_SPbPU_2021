#pragma once

#include "ThreadPool.h"

template<template<class> class Container, class ValueType>
class ReadWriteTester {
public:
	ReadWriteTester();
	~ReadWriteTester();

	void startWriters(const std::vector<ValueType>& data, const std::vector<size_t>& chunkSizes);

	void waitForTestsEnd();

	const Container<ValueType>& container() const;

private:
	ThreadPool* m_threadPool;
	Container<ValueType>* m_container;
};

#include "ReadWriteTester.tpp"
