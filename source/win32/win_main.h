#pragma once
#include <win32/win_configure.h>

enum sysEventType_t
{
	SE_NONE = 0x0,
	SE_KEY = 0x1,
	SE_CHAR = 0x2,
	SE_CONSOLE = 0x3,
};

struct sysEvent_t
{
	int evTime;
	sysEventType_t evType;
	int evValue;
	int evValue2;
	int evPtrLength;
	void* evPtr;
};

SysInfo sys_info;

char sys_exitCmdLine[1024];

bool g_quitRequested;

int enable_OutputDebugString;

sysEvent_t eventQue[256];
int eventHead;
int eventTail;

void Sys_FindInfo();
void Sys_OutOfMemErrorInternal(const char* filename, int line);
void Sys_QuitAndStartProcess(const char* exeName, const char* parameters);
void Sys_SpawnQuitProcess();
void Sys_Error(const char* error, ...);
void Sys_Print(const char* msg);
char* Sys_GetClipboardData();
void Sys_FreeClipboardData(char* text);
void Sys_QueEvent(int time, sysEventType_t type, int value, int value2, int ptrLength, void* ptr);
void Sys_ShutdownEvents();
sysEvent_t* Win_GetEvent(sysEvent_t* result);
void Sys_LoadingKeepAlive();