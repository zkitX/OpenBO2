#pragma once
#include "../defs.h"
#include "../qcommon/cmd.h"
#include <Windows.h>
#include <setjmp.h>

__declspec(thread) void** g_dwTlsIndex;
__declspec(thread) unsigned int g_currentThreadId;
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma data_seg(".CRT$XLB")
#pragma data_seg()

typedef struct _SCOPETABLE_ENTRY* PSCOPETABLE_ENTRY;

struct _EH3_EXCEPTION_REGISTRATION
{
	struct _EH3_EXCEPTION_REGISTRATION* Next;
	PVOID ExceptionHandler;
	PSCOPETABLE_ENTRY ScopeTable;
	DWORD TryLevel;
};

struct CPPEH_RECORD
{
	DWORD old_esp;
	EXCEPTION_POINTERS* exc_ptr;
	struct _EH3_EXCEPTION_REGISTRATION registration;
};

struct tagTHREADNAME_INFO
{
	unsigned int dwType;
	const char* szName;
	unsigned int dwThreadID;
	unsigned int dwFlags;
};

struct va_info_t
{
	char va_string[4][1024];
	int index;
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

va_info_t va_info[17];
CmdArgs g_cmd_args[2];
int g_com_error[16][16];
TraceThreadInfo g_traceThreadInfo[16];

void* g_threadValues[17][5];
void* threadHandle[17];
unsigned int threadId[17];
void(* threadFunc[17])(unsigned int);

const char* s_threadNames[17] = { "Main", "Backend", "Worker0", "Worker1", "Worker2", "Worker3", "Worker4", "Worker5", "Worker6", "Worker7", "TitleServer", "Database", "Sound Mix", "Sound Decode", "WebM Decode"};

int g_databaseStopServer;

HANDLE databaseCompletedEvent;
HANDLE demoStreamingReady;
HANDLE serverCompletedEvent;

unsigned int s_affinityMaskForCpu[8];
unsigned int s_affinityMaskForProcess;
unsigned int s_cpuCount;

void Com_InitThreadData(int threadContext);
void SetThreadName(unsigned int dwThreadID, const char* szThreadName);
void Sys_CreateEvent(int manualReset, int initialState, void** evt);
void Sys_CreateThread(unsigned int threadContext, void(*function)(unsigned int));
void Sys_DatabaseCompleted();
void Sys_FrontEndSleep();
int Sys_GetThreadContext();
void* Sys_GetValue(int valueIndex);
void Sys_InitDemoStreamingEvent();
void Sys_InitMainThread();
void Sys_InitWebMStreamingEvent();
bool Sys_IsMainThread();
bool Sys_IsServerThread();
bool Sys_IsRenderThread();
bool Sys_IsDatabaseThread();
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
unsigned int Sys_GetCpuCount();
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

