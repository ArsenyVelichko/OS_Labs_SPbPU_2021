#pragma once

class Runnable {
public:
	virtual void run() = 0;
	virtual ~Runnable() = default;

	bool autoDelete() const;
	void setAutoDelete(bool autoDelete);

private:
	bool m_autoDelete = true;
};
