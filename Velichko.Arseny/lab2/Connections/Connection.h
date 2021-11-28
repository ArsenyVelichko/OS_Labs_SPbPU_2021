#pragma once

#include <memory>
#include <cstdio>

class ConnectionImpl;

class Connection {
public:
	Connection(int id, bool create);
	~Connection();

	int id() const;

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	//Constant propagation
	const ConnectionImpl* pImpl() const;
	ConnectionImpl* pImpl();

	std::unique_ptr<ConnectionImpl> m_pImpl;
	int m_id;
};