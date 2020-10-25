#pragma once
#include "pch.h"

class bdPlatformSemaphore
{
private:
public:
	HANDLE createSemaphore(unsigned int initialCount, unsigned int maxCount);
	void destroy(void** handle);
	void release(void** handle);
	bool wait(void** handle);
};
