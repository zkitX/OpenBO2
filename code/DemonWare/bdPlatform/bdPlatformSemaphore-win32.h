#pragma once
#include "pch.h"

class bdPlatformSemaphore
{
private:
public:
	static HANDLE createSemaphore(unsigned int initialCount, unsigned int maxCount);
	static void destroy(void** handle);
	static void release(void** handle);
	static bool wait(void** handle);
};
