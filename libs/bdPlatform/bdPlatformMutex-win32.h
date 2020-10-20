#pragma once
class bdPlatformMutex
{
public:
	HANDLE createMutex();
	void unlock(void** handle);
	void destroy(void** handle);
	void lock(void** handle);
};
