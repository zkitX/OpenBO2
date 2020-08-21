#pragma once

#include <Windows.h>

miniDumper g_miniDumper;
bool g_miniDumperStarted;
unsigned int _S1_19;

// _________________________________________________________________________________
// Structure declarations
// _________________________________________________________________________________


// _________________________________________________________________________________
// Enumeration declarations
// _________________________________________________________________________________

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

enum _MINIDUMP_TYPE
{
	MiniDumpNormal = 0x0,
	MiniDumpWithDataSegs = 0x1,
	MiniDumpWithFullMemory = 0x2,
	MiniDumpWithHandleData = 0x4,
	MiniDumpFilterMemory = 0x8,
	MiniDumpScanMemory = 0x10,
	MiniDumpWithUnloadedModules = 0x20,
	MiniDumpWithIndirectlyReferencedMemory = 0x40,
	MiniDumpFilterModulePaths = 0x80,
	MiniDumpWithProcessThreadData = 0x100,
	MiniDumpWithPrivateReadWriteMemory = 0x200,
	MiniDumpWithoutOptionalData = 0x400,
	MiniDumpWithFullMemoryInfo = 0x800,
	MiniDumpWithThreadInfo = 0x1000,
	MiniDumpWithCodeSegs = 0x2000,
	MiniDumpWithoutAuxiliaryState = 0x4000,
	MiniDumpWithFullAuxiliaryState = 0x8000,
	MiniDumpWithPrivateWriteCopyMemory = 0x10000,
	MiniDumpIgnoreInaccessibleMemory = 0x20000,
	MiniDumpWithTokenInformation = 0x40000,
	MiniDumpValidTypeFlags = 0x7FFFF,
};

// _________________________________________________________________________________
// Functions declarations
// _________________________________________________________________________________

int PrivateUnhandledExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);

void Sys_Error(const char* error, ...);
char* Sys_GetIdentityParam(IdentityParam p);
void Sys_OutOfMemErrorInternal(int a1, const char* filename, int line);

// Startup function
int __stdcall WinMain(HINSTANCE__* hInstance, HINSTANCE__* hPrevInstance, char* lpCmdLine, int nCmdShow);

char* Win_GetTheOtherExeName(const char* mode);

// _________________________________________________________________________________
// END
// _________________________________________________________________________________

class miniDumper {
	struct {
		_EXCEPTION_POINTERS* m_pExceptionInfo;
		char m_szMiniDumpPath[260];
		char m_szMiniDumpDir[260];
		char m_szAppPath[260];
		char m_szAppBaseName[260];
		bool m_bPromptUserForMiniDump;
	};
public:
	void __cdecl SetDumpDir_f();
	void InitMiniDumper(char* lpCmdLine);
	void setMiniDumpFileName(miniDumper*, const char* tag);
	void Sys_StartMiniDump(bool prompt);
	int unhandledExceptionHandler(_EXCEPTION_POINTERS* pExceptionInfo);
	int writeMiniDump(miniDumper*, _EXCEPTION_POINTERS* pExceptionInfo, const char* tag, _MINIDUMP_TYPE type, bool prompt, bool upload);
private:
	static miniDumper* s_pMiniDumper;
	static LPCRITICAL_SECTION s_pCriticalSection;
};