#include "jobqueue.h"


#pragma region tlAtomicMutex
void tlAtomicMutex::Lock()
{
	DWORD CurThread;
	DWORD retnThreadId;
	LONG Target;

	retnThreadId = GetCurrentThreadId();
	CurThread = retnThreadId;
	if (this->ThreadId != retnThreadId || this->ThreadId) {
		while (_InterlockedCompareExchange64((volatile signed __int64*)this->ThisPtr, retnThreadId, 0i64))
		{
			Sleep(0);
			retnThreadId = CurThread;
		}
		Target = 0;
		InterlockedExchange(&Target, 0);
		this->LockCount = 1;
	}
	else
	{
		++this->LockCount;
	}
}

tlAtomicMutex::~tlAtomicMutex()
{
	this->ThreadId = 0i64;
	this->ThisPtr = 0;
}
#pragma endregion

