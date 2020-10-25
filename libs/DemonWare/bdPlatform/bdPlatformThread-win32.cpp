#include <pch.h>

bool bdPlatformThread::createThread(unsigned int(__stdcall* threadProc)(void*), void* args, void** handle, int __formal, unsigned int stackSize)
{
	HANDLE newThread;

	newThread = CreateThread(0, stackSize, (LPTHREAD_START_ROUTINE)threadProc, args, 4u, 0);
	*handle = newThread;
	return newThread != 0;

}

bool bdPlatformThread::startThread(void** handle)
{
	return ResumeThread(*handle) != -1;
}

void bdPlatformThread::deleteThread(void** handle)
{
	CloseHandle(*handle);
}