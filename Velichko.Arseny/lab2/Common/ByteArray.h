#pragma once

#include <cstddef>

class ByteArray {
public:
	ByteArray() = default;
	ByteArray(const char* data, size_t size);
	ByteArray(const ByteArray& other);

	~ByteArray();

	friend void swap(ByteArray& first, ByteArray& second);
	ByteArray& operator=(ByteArray other);

	static ByteArray fromInt(int value);
	int toInt(int pos = 0) const;

	const char* constData() const;
	size_t size() const;

private:
	char* m_data = nullptr;
	size_t m_size = 0;
};