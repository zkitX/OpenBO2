#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <psapi.h>

struct tlFileBuf
{
	char* Buf;
	unsigned int Size;
	unsigned int UserData;
};

struct tlSystemCallbacks
{
	bool(* ReadFile)(const char*, tlFileBuf*, unsigned int, unsigned int);
	void(* ReleaseFile)(tlFileBuf*);
	void(* CriticalError)(const char*);
	void(* Warning)(const char*);
	void(* DebugPrint)(const char*);
	void* (* MemAlloc)(unsigned int, unsigned int, unsigned int);
	void* (* MemRealloc)(void*, unsigned int, unsigned int, unsigned int);
	void(* MemFree)(void*);
};

tlSystemCallbacks tlCurSystemCallbacks;

char g_npPool[131072];
float tlPcTicksPerMS;
int tlMemAllocCounter;
int tlScratchPadRefCount;
bool tlScratchpadLocked;
void* tlScratchPadPtr;
char* tlHostPrefix;
void* tlStackBegin;
void* tlStackEnd;

inline void tlSetSystemCallbacks(const tlSystemCallbacks* Callbacks) {
	memcpy(&tlCurSystemCallbacks, Callbacks, sizeof(tlCurSystemCallbacks));
}

inline void tlSetFileServerRootPC(char const* Path) {
	strcpy(tlHostPrefix, Path);
}

inline void tlMemFree(void* Ptr) {
	--tlMemAllocCounter;
	if (tlCurSystemCallbacks.MemFree)
		tlCurSystemCallbacks.MemFree(Ptr);
	else
		_aligned_free(Ptr);
}

inline void tlReleaseFile(tlFileBuf* File) {
	char* fileBuf;

	if (tlCurSystemCallbacks.ReleaseFile)
	{
		tlCurSystemCallbacks.ReleaseFile(File);
	}
	else
	{
		fileBuf = File->Buf;
		--tlMemAllocCounter;
		if (tlCurSystemCallbacks.MemFree)
			tlCurSystemCallbacks.MemFree(fileBuf);
		else
			_aligned_free(fileBuf);
		File->Buf = 0;
		File->Size = 0;
		File->UserData = 0;
	}
}

inline LARGE_INTEGER tlPcGetTick() {
	LARGE_INTEGER li;

	QueryPerformanceCounter(&li);
	return li;
}

inline void tlPrint(char const* txt) {
	OutputDebugStringA(txt);
}

inline int tlGetVersion() {
	return 66560;
}

inline void tlStackRangeInit() {
	tlStackBegin = 0;
	tlStackEnd = 0;
}

inline bool tlFatalHandler(char const* Msg) {
	if (tlCurSystemCallbacks.CriticalError) {
		tlCurSystemCallbacks.CriticalError(Msg);
		return false;
	}
	else {
		OutputDebugStringA("TL Fatal Error: ");
		OutputDebugStringA(Msg);
		OutputDebugStringA("\n");
		return true;
	}
}

inline void tlDebugPrint(char const* txt) {
	if (tlCurSystemCallbacks.DebugPrint)
		tlCurSystemCallbacks.DebugPrint(txt);
	else
		OutputDebugStringA(txt);
}

inline void tlVPrintf(char const* Format, char* args) {
	char Work[512];

	vsprintf(Work, Format, args);
	if (tlCurSystemCallbacks.DebugPrint)
		tlCurSystemCallbacks.DebugPrint(Work);
	else
		OutputDebugStringA(Work);
}

inline void tlPrintf(char const* Format, ...) {
	char Work[512];
	va_list ap;

	va_start(ap, Format);
	vsprintf(Work, Format, ap);
	if (tlCurSystemCallbacks.DebugPrint)
		tlCurSystemCallbacks.DebugPrint(Work);
	else
		OutputDebugStringA(Work);
}

inline bool _tlAssert(char const* file, int line, char const* expr, char const* desc) {
	char Buf[256];

	_snprintf(Buf, 0x100u, "ASSERT in %s(%d):\n\"%s\" - %s", file, line, expr, desc);
	Buf[255] = 0;
	if (tlCurSystemCallbacks.CriticalError)
	{
		tlCurSystemCallbacks.CriticalError(Buf);
		return false;
	}
	else
	{
		OutputDebugStringA("TL Fatal Error: ");
		OutputDebugStringA(Buf);
		OutputDebugStringA("\n");
		return true;
	}
}

inline void tlFatal(char const* Format, ...) {
	char Work[512];
	va_list ap;

	va_start(ap, Format);
	vsprintf(Work, Format, ap);
	if (!tlCurSystemCallbacks.CriticalError)
	{
		OutputDebugStringA("TL Fatal Error: ");
		OutputDebugStringA(Work);
		OutputDebugStringA("\n");
		__debugbreak();
	}
	tlCurSystemCallbacks.CriticalError(Work);
}

inline void* tlMemAlloc(unsigned int Size, unsigned int Align, unsigned int Flags) {
	unsigned int alignChk;
	void* memPtr;

	alignChk = Align;
	if (!Align && !(Size & 0xF))
		alignChk = 0;
	++tlMemAllocCounter;
	if (tlCurSystemCallbacks.MemAlloc)
		memPtr = tlCurSystemCallbacks.MemAlloc(Size, alignChk, Flags);
	else
		memPtr = _aligned_malloc(Size, alignChk);
	if (!(Flags & 2) && !memPtr && Size)
		tlFatal("Memory allocation failed. %d bytes, %d align", Size, alignChk);
	return memPtr;
}

inline void* tlMemRealloc(void* Ptr, unsigned int Size, unsigned int Align, unsigned int Flags) {
	unsigned int alignChk;
	void* memPtr;

	alignChk = Align;
	if (!Align && !(Size & 0xF))
		alignChk = 0;
	if (tlCurSystemCallbacks.MemRealloc)
		memPtr = tlCurSystemCallbacks.MemRealloc(Ptr, Size, alignChk, Flags);
	else
		memPtr = _aligned_realloc(Ptr, Size, alignChk);
	if (!(Flags & 2) && !memPtr && Size)
		tlFatal("Memory reallocation failed.");
	return memPtr;
}

inline unsigned int tlGetFreeMemory() {
	unsigned int freeMemory;
	DWORD processId;
	HANDLE processHandle;
	_PROCESS_MEMORY_COUNTERS pmc;
	_MEMORYSTATUS stat;

	freeMemory = 0;
	GlobalMemoryStatus(&stat);
	freeMemory = stat.dwTotalPhys;
	GetCurrentProcess();
	processId = GetCurrentProcessId();
	processHandle = OpenProcess(0x410u, 0, processId);
	if (GetProcessMemoryInfo(processHandle, &pmc, 40))
		freeMemory -= pmc.PeakWorkingSetSize;
	CloseHandle(processHandle);
	return freeMemory;
}

inline void* tlScratchPadInit() {
	void* scratchPadPtr;

	if (tlScratchPadRefCount++)
		return tlScratchPadPtr;
	++tlMemAllocCounter;
	if (tlCurSystemCallbacks.MemAlloc)
		scratchPadPtr = tlCurSystemCallbacks.MemAlloc(0x4000u, 0x10u, 0);
	else
		scratchPadPtr = _aligned_malloc(0x4000u, 0x10u);
	if (!scratchPadPtr)
		tlFatal("Memory allocation failed. %d bytes, %d align", 0x4000, 16);
	tlScratchPadPtr = scratchPadPtr;
	return scratchPadPtr;
}

inline void tlScratchPadReset() {
	if (tlScratchPadRefCount < 1
		&& _tlAssert(__FILE__, __LINE__, "tlScratchPadRefCount >= 1", "Scratchpad reset underflow."))
	{
		__debugbreak();
	}
	if (!--tlScratchPadRefCount)
	{
		--tlMemAllocCounter;
		if (tlCurSystemCallbacks.MemFree)
			tlCurSystemCallbacks.MemFree(tlScratchPadPtr);
		else
			_aligned_free(tlScratchPadPtr);
		tlScratchPadPtr = 0;
	}
}

inline void tlWarning(char const* Format, ...) {
	char Work[512];
	va_list ap;

	va_start(ap, Format);
	vsprintf(Work, Format, ap);
	if (tlCurSystemCallbacks.Warning)
		tlCurSystemCallbacks.Warning(Work);
	else
		tlPrintf("%s", Work);
}

inline bool tlReadFile(char const* FileName, tlFileBuf* File, unsigned int Align, unsigned int Flags) {
	int fileLoc;
	char fileRead = 0;
	unsigned int fileLen;
	char* filePtr;
	char* i;
	HANDLE openedFile;
	unsigned int fileSize;
	char* allocLoc;
	unsigned int bytesRead = 0;
	unsigned int alignment;
	char Work[512] = {0};

	alignment = Align;
	if (tlCurSystemCallbacks.ReadFile)
		return tlCurSystemCallbacks.ReadFile(FileName, File, Align, Flags);
	if (!strncmp(FileName, tlHostPrefix, strlen(tlHostPrefix)))
	{
		Work[0] = 0;
	}
	else
	{
		fileLoc = 0;
		do
		{
			fileRead = tlHostPrefix[fileLoc];
			Work[fileLoc++] = fileRead;
		} while (fileRead);
	}
	fileLen = strlen(FileName) + 1;
	filePtr = (char*)&alignment + 3;
	while (*++filePtr)
		;
	memcpy(filePtr, FileName, fileLen);
	for (i = Work; *i; ++i)
	{
		if (*i == 47)
			*i = 92;
	}
	openedFile = CreateFileA(Work, 0x80000000, 1u, 0, 3u, (DWORD)&g_npPool[24776], 0);
	if (openedFile == (HANDLE)-1)
		return 0;
	fileSize = GetFileSize(openedFile, 0);
	File->Size = fileSize;
	if (!alignment && !(fileSize & 0xF))
		alignment = 0;
	++tlMemAllocCounter;
	if (tlCurSystemCallbacks.MemAlloc)
		allocLoc = (char*)tlCurSystemCallbacks.MemAlloc(fileSize, alignment, Flags);
	else
		allocLoc = (char*)_aligned_malloc(fileSize, alignment);
	if (!(Flags & 2) && !allocLoc)
	{
		if (fileSize)
			tlFatal("Memory allocation failed. %d bytes, %d align", fileSize, alignment);
	}
	fileSize = File->Size;
	File->Buf = allocLoc;
	ReadFile(openedFile, allocLoc, fileSize, (LPDWORD)&bytesRead, 0);
	CloseHandle(openedFile);
	return 1;
}