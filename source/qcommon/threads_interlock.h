#pragma once

#include <universal/assertive.h>
#include <win32/win_net.h>

struct FastCriticalSection
{
	volatile int readCount;
	volatile int writeCount;
};

inline void Sys_LockWrite(FastCriticalSection* critSect) {
	while (1) {
		if (critSect->readCount)
			NET_Sleep(0);
		if (_InterlockedIncrement((volatile long*)&critSect->writeCount) == 1 && !critSect->readCount)
			break;
		_InterlockedExchangeAdd((volatile long*)&critSect->writeCount, 0xFFFFFFFF);
		NET_Sleep(0);
	}
}

inline void Sys_UnlockWrite(FastCriticalSection* critSect) {
	if (critSect->writeCount <= 0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(critSect->writeCount > 0)",
			nullptr))
	{
		__debugbreak();
	}
	_InterlockedExchangeAdd((volatile long*)&critSect->writeCount, 0xFFFFFFFF);
}