#pragma once
class bdMutex
{
private:
	void* m_handle;
public:
	bdMutex();
	~bdMutex();
	static void unlock(bdMutex* instance);
	static void lock(bdMutex* instance);
};