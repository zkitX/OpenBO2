#pragma once
class bdRunnable
{
private:
	bool m_stop;
public:
	bdRunnable();
	virtual ~bdRunnable();
	void start();
	void stop();
};

