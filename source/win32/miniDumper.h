#pragma once
#include <Windows.h>

#include "../qcommon/cmd.h"

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

bool Sys_IsMiniDumpStarted();
void Sys_StartMiniDump(bool prompt);

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
	bool enablePrivilege(const char* pszPriv, void* hToken, _TOKEN_PRIVILEGES* ptpOld);
	void InitMiniDumper(char* lpCmdLine);
	void setMiniDumpFileName(miniDumper*, const char* tag);
	int unhandledExceptionHandler(_EXCEPTION_POINTERS* pExceptionInfo);
	int writeMiniDump(miniDumper*, _EXCEPTION_POINTERS* pExceptionInfo, const char* tag, _MINIDUMP_TYPE type, bool prompt, bool upload);
private:
	static LPCRITICAL_SECTION s_pCriticalSection;
};

miniDumper g_miniDumper;
bool g_miniDumperStarted;
static miniDumper* s_pMiniDumper;
cmd_function_s SetDumpDir_f_VAR;

 