#include <cstring>
#include <algorithm>

#include "ByteArray.h"

ByteArray::ByteArray(const char* data, size_t size)
	: m_data(new char[size]), m_size(size) {
	memcpy(m_data, data, size);
}

ByteArray::ByteArray(const ByteArray& other) : ByteArray(other.m_data, other.m_size) {}

ByteArray::~ByteArray() {
	delete[] m_data;
}

void swap(ByteArray& first, ByteArray& second) {
	std::swap(first.m_size, second.m_size);
	std::swap(first.m_data, second.m_data);
}

ByteArray& ByteArray::operator=(ByteArray other) {
	swap(*this, other);
	return *this;
}

ByteArray ByteArray::fromInt(int value) {
	auto data = reinterpret_cast<const char*>(&value);
	return ByteArray(data, sizeof(int));
}

int ByteArray::toInt(int pos) const {
	return *reinterpret_cast<int*>(m_data + pos);
}

const char* ByteArray::constData() const {
	return m_data;
}

size_t ByteArray::size() const {
	return m_size;
}

