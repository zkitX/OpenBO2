#pragma once
#include <win32/win_configure.h>

#include <qcommon/cmd.h>

enum IdentityParam
{
	IDENTITY_ID = 0x0,
	IDENTITY_TITLE_ID = 0x1,
	IDENTITY_AUTH_ADDR = 0x2,
	IDENTITY_LSG_ADDR = 0x3,
	IDENTITY_DW_KEY = 0x4,
	IDENTITY_ENVIRONMENT = 0x5,
	IDENTITY_PLATFORM = 0x6,
	IDENTITY_PARAM_COUNT = 0x7,
};

enum sysEventType_t
{
	SE_NONE = 0x0,
	SE_KEY = 0x1,
	SE_CHAR = 0x2,
	SE_CONSOLE = 0x3,
};

struct cmdlineArg_t
{
	char key[64];
	char val[128];
	bool optional;
	IdentityParam id;
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

cmd_function_s Sys_Net_Restart_f_VAR;

char g_identityParams[1792];

cmdlineArg_t s_cmdlineArgs[7];

char theOtherExeName[256];

char errPtr[4096];

char sys_cmdline[1024];

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
sysEvent_t* Sys_GetEvent(sysEvent_t* result);
void Sys_Net_Restart_f();
void Sys_Init();
char* Sys_GetIdentityParam(IdentityParam p);
char* getArgFromString(char const *src, char const *marker, char *dest, unsigned int dstSize);
void checkdmlineValid();
#ifdef DEDICATED_SERVER
bool Sys_ReadIdentity(char const* id);
void DedicatedInit(const char* cmdline);
#endif
void InitMiniDumper(char *lpCmdLine);
char* Win_GetTheOtherExeName(const char* mode);
void Sys_CheckQuitRequest();
void Sys_Quit();
int __stdcall WinMain(HINSTANCE* hInstance, HINSTANCE* hPrevInstance, char* lpCmdLine, int nCmdShow);