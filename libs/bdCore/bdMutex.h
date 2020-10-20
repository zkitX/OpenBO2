#pragma once
class bdMutex
{
private:
	void* m_handle;
public:
	bdMutex();
	~bdMutex();
	void unlock();
	void lock();
};