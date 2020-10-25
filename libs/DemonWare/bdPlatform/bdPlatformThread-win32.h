#pragma once
#include "pch.h"

class bdPlatformThread {
private:
public:
	bool createThread(unsigned int(__stdcall* threadProc)(void*), void* args, void** handle, int __formal, unsigned int stackSize);
	bool startThread(void** handle);
	void deleteThread(void** handle);
};