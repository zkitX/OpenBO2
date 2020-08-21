#pragma once
#include "../defs.h"
#include <Windows.h>
#include <setjmp.h>

#include "../win32/win_gamepad.h"

struct CmdArgs
{
	int nesting; //0x0
	char unk0[0x60]; //0x4
	int localClientNum[8]; //0x64
	int argc[8]; //0x84
	const char** argv[8]; //0xA4
	char textPool[0x2000]; //0xC4
	const char* argvPool[0x200]; //0x20C4
	int usedTextPool[8]; //0x28C4
	int totalUsedArgvPool; //0x28E4
	int totalUsedTextPool;//0x28E8
};

__declspec(thread) void** g_dwTlsIndex;
__declspec(thread) unsigned int g_currentThreadId;
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma data_seg(".CRT$XLB")
#pragma data_seg()

struct va_info_t
{
	char va_string[4][1024];
	int index;
};

struct TraceCheckCount
{
	unsigned __int16 global;
	unsigned __int16* partitions;
	unsigned __int16* brushes;
};

struct TraceThreadInfo
{
	short global;
	short(*partitions)[0x1ED0];
	short(*partitions2)[0x1ED0];
	char(*ptr0)[0x60];
	char(*ptr1)[0x4C];
	char(*ptr2)[0x4];
};

struct CmdArgs
{
	int nesting;
	LocalClientNum_t localClientNum[8];
	win_gamepad::ControllerIndex_t controllerIndex[8];
	itemDef_s* itemDef[8];
	int argshift[8];
	int argc[8];
	const char** argv[8];
	char textPool[8192];
	const char* argvPool[512];
	int usedTextPool[8];
	int totalUsedArgvPool;
	int totalUsedTextPool;
};

va_info_t va_info[17];
int g_com_error[16][16];
TraceThreadInfo g_traceThreadInfo[16];

void* g_threadValues[17][5];
void* threadHandle[17];
unsigned int threadId[17];

unsigned int s_affinityMaskForCpu[8];
unsigned int s_affinityMaskForProcess;
unsigned int s_cpuCount;

void Com_InitThreadData(int threadContext);
void Sys_CreateEvent(int manualReset, int initialState, void** evt);
void Sys_CreateThread(unsigned int threadContext, void(*function)(unsigned int));
void Sys_DatabaseCompleted();
void Sys_FrontEndSleep();
int Sys_GetThreadContext();
void Sys_InitDemoStreamingEvent();
void Sys_InitMainThread();
void Sys_InitWebMStreamingEvent();
void Sys_NotifyRenderer();
void Sys_ResetServerNetworkCompletedEvent();
void Sys_SetServerNetworkCompletedEvent();
void Sys_SetValue(int valueIndex, void* data);
char Sys_SpawnDatabaseThread(void(*function)(unsigned int));
char Sys_SpawnServerThread(void(*function)(unsigned int));
char Sys_SpawnStreamThread(void(*function)(unsigned int));
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
unsigned int Win_InitThreads();
class threads
{
public:
	// _________________________________________________________________________________
	// Function declarations
	// _________________________________________________________________________________

	// _________________________________________________________________________________
	// END
	// _________________________________________________________________________________
};

