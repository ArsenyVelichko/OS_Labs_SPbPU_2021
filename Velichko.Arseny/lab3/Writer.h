#pragma once

#include <vector>

#include "Runnable.h"

template<template<class> class Container, class ValueType>
class Writer : public Runnable {
public:
	explicit Writer(Container<ValueType>* container);

	void setDataToWrite(const std::vector<ValueType>& data);

	void run() override;

private:
	Container<ValueType>* m_container;
	std::vector<ValueType> m_data;
	pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
};

#include "Writer.tpp"