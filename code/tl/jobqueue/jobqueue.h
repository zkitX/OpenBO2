#pragma once
#include <Windows.h>
#include <tl_system.h>

class tlAtomicMutex {
public:
	unsigned __int64 ThreadId;
	int LockCount;
	tlAtomicMutex* ThisPtr;

	void Lock();
	~tlAtomicMutex();
};