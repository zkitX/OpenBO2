#pragma once
#include <stdio.h>

#include "q_shared.h"
#include "../defs.h"

enum AssertOccurance
{
	FIRST_TIME = 0x0,
	RECURSIVE = 0x1,
};

struct AddressInfo_s
{
	unsigned int address;
	char moduleName[64];
	char bestFunction[64];
	char bestFunctionFilename[64];
	unsigned int bestFunctionAddress;
	char bestLineFilename[64];
	unsigned int bestLineAddress;
	unsigned int bestLineNumber;
};

char assertMessage[4096];
AddressInfo_s g_assertAddress[32];
char lineBuffer[4096];

int g_assertAddressCount;
bool g_inStackTrace;
char g_module[260];
int isHandlingAssert;
int lastAssertType;
unsigned int lineBufferEndPos;
int lineBufferStartPos;
char message[1024];

int g_hiddenCount;
HWND__* g_hwndGame[4];
char g_stackTrace[8192];

bool shouldQuitOnError;

int HideWindowCallback(HWND__* hwnd, int lParam);
void FixWindowsDesktop();
void ParseError(const char* msg);
char ReadLine(_iobuf* fp);
char SkipLines(int lineCount, _iobuf* fp);
bool ParseMapFile(_iobuf* fp, bool a2, unsigned int baseAddress, const char* mapName);
HMODULE GetModuleBase(int a1, const char* name);
void LoadMapFilesForDir(const char* dir);
int StackTrace_ResolveSymbols();
int StackTrace_Generate(int len, char* msg);
int StackTrace_Walk(int ignoreCount, void* context);
AddressInfo_s* StackTrace_GetAddressInfo(int* addressCount);
void StackTrace_ResetAddressInfo();
void CopyMessageToClipboard(const char* msg);
char AssertNotify(int type, AssertOccurance occurance);
int Assert_BuildAssertMessageWithStack(const char* extra, int line, char* message, const char* expr, const char* filename, int type, int messageLen);
int IsDebuggerConnected();
void RefreshQuitOnErrorCondition();
bool QuitOnError();
bool Assert_MyHandler(const char* filename, int line, int type, const char* expr, const char* fmt, ...);

void(* AssertCallback)(const char*);