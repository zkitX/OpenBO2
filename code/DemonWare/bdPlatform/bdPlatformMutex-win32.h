#pragma once
class bdPlatformMutex
{
public:
	static HANDLE createMutex();
	static void unlock(void** handle);
	static void destroy(void** handle);
	static void lock(void** handle);
};
