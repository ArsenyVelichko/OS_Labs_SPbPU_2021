#include "MutexLocker.h"

template<template<class> class Container, class ValueType>
Writer<Container, ValueType>::Writer(Container<ValueType>* container) : m_container(container) {}

template<template<class> class Container, class ValueType>
void Writer<Container, ValueType>::setDataToWrite(const std::vector<ValueType>& data) {
	MutexLocker locker(&m_mutex);
	m_data = data;
}

template<template<class> class Container, class ValueType>
void Writer<Container, ValueType>::run() {
	MutexLocker locker(&m_mutex);
	for (const auto& val : m_data) {
		m_container->insert(val);
	}
}
