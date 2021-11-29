#pragma once

#include <memory>
#include <cstdio>

class ConnectionPrivate;

class Connection {
public:
	Connection(int id, int timeout, bool create);
	~Connection();

	int id() const;
	int timeout() const;

	ssize_t read(char* data, size_t size);
	ssize_t write(const char* data, size_t size);

private:
	//Constant propagation
	const ConnectionPrivate* pImpl() const;
	ConnectionPrivate* pImpl();

	std::unique_ptr<ConnectionPrivate> m_pImpl;
};