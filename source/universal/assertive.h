#pragma once
// 
#include <Windows.h>
#include <cstdio>

#include "q_shared.h"
#include "../defs.h"

class assertive
{
public:
	// _________________________________________________________________________________
	// Structure definitions
	// _________________________________________________________________________________
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
	// _________________________________________________________________________________
	// Public variable declarations
	// _________________________________________________________________________________
	static char assertMessage[4096];
	static AddressInfo_s g_assertAddress[32];
	static char lineBuffer[4096];
	// _________________________________________________________________________________
	// Function declarations
	// _________________________________________________________________________________
	static bool Assert_MyHandler(const char* filename, int line, int type, const char* expr, const char* fmt, ...);

	static HMODULE GetModuleBase(int a1, const char* name);

	static int StackTrace_Generate(int len, char* msg);
	static AddressInfo_s* StackTrace_GetAddressInfo(int* addressCount);
	static void StackTrace_ResetAddressInfo();
	static int StackTrace_ResolveSymbols();
	static int StackTrace_Walk(int ignoreCount, void* context);

private:
	// _________________________________________________________________________________
	// Private variable declarations
	// _________________________________________________________________________________
	static int g_assertAddressCount;
	static bool g_inStackTrace;
	static char g_module[260];
	static int lastAssertType;
	static unsigned int lineBufferEndPos;
	static int lineBufferStartPos;
	static char message[1024];

	// _________________________________________________________________________________
	// Private function declarations
	// _________________________________________________________________________________
	static int Assert_BuildAssertMessageWithStack(const char* extra, int line, char* message, const char* expr, const char* filename, int type, int messageLen);
	static void LoadMapFilesForDir(const char* dir);
	static void ParseError(const char* msg);
	static bool ParseMapFile(_iobuf* fp, bool a2, unsigned int baseAddress, const char* mapName);
	static char ReadLine(_iobuf* fp);
	static char SkipLines(int lineCount, _iobuf* fp);
};
