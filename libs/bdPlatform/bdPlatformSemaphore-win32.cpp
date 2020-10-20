#include "pch.h"

HANDLE bdPlatformSemaphore::createSemaphore(unsigned int initialCount, unsigned int maxCount)
{
	return CreateSemaphoreA(0, initialCount, maxCount, 0);
}

void bdPlatformSemaphore::destroy(void** handle)
{
	ReleaseSemaphore(*handle, 1, 0);
	CloseHandle(*handle);
}

void bdPlatformSemaphore::release(void** handle)
{
	ReleaseSemaphore(*handle, 1, 0);
}

bool bdPlatformSemaphore::wait(void** handle)
{
	return WaitForSingleObject(*handle, 0xFFFFFFFF) == 0;
}
