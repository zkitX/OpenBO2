#pragma once
#include "pch.h"

class bdPlatformThread {
private:
public:
	static bool createThread(unsigned int(__stdcall* threadProc)(void*), void* args, void** handle, int __formal, unsigned int stackSize);
	static bool startThread(void** handle);
	static void deleteThread(void** handle);
};