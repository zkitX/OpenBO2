#include "pch.h"

HANDLE bdPlatformMutex::createMutex()
{
	return CreateMutexA(0, 0, 0);
}

void bdPlatformMutex::unlock(void** handle)
{
	ReleaseMutex(*handle);
}

void bdPlatformMutex::destroy(void** handle)
{
	ReleaseMutex(*handle);
	CloseHandle(*handle);
}

void bdPlatformMutex::lock(void** handle)
{
	WaitForSingleObject(*handle, 0xFFFFFFFF);
}
