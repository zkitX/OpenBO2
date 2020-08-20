#pragma once
#include <Windows.h>
class threads
{
public:
	// _________________________________________________________________________________
	// Function declarations
	// _________________________________________________________________________________
	void Sys_CreateEvent(int manualReset, int initialState, void** evt);
	void Sys_CreateThread(unsigned int threadContext, void(*function)(unsigned int));
	void Sys_DatabaseCompleted();
	void Sys_FrontEndSleep();
	int Sys_GetThreadContext();
	void Sys_InitDemoStreamingEvent();
	void Sys_InitWebMStreamingEvent();
	void Sys_NotifyRenderer();
	void Sys_ResetServerNetworkCompletedEvent();
	void Sys_SetServerNetworkCompletedEvent();
	char Sys_SpawnDatabaseThread(void(* function)(unsigned int));
	char Sys_SpawnServerThread(void(* function)(unsigned int));
	char Sys_SpawnStreamThread(void(* function)(unsigned int));
	void Sys_StreamSleep();
	unsigned int Sys_ThreadMain(void* parameter);
	void Sys_WaitAllowServerNetworkLoop();
	void Sys_WaitForDemoStreamingEvent();
	bool Sys_WaitForDemoStreamingEventTimeout(unsigned int msec);
	bool Sys_WaitForGumpFlush(int timeout);
	bool Sys_WaitForGumpLoad(int timeout);
	void Sys_WaitForSingleObject(void** event);
	bool Sys_WaitForSingleObjectTimeout(void** event, unsigned int msec);
	bool Sys_WaitServer(int timeout);
	void Sys_WaitServerNetworkCompleted();
	void Sys_WaitStartDatabase();
	bool Sys_WaitStartServer(int timeout);
	// _________________________________________________________________________________
	// END
	// _________________________________________________________________________________
};

