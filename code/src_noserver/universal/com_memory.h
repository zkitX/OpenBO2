#pragma once

#include <Windows.h>

#include <database/db_load.h>
#include <qcommon/cmd.h>

struct MemoryFile
{
	char* buffer;
	int bufferSize;
	int bytesUsed;
	char* careerStatsBuffer;
	int cacheBufferUsed;
	int segmentIndex;
	int segmentStart;
	int nonZeroCount;
	int zeroCount;
	bool errorOnOverflow;
	bool memoryOverflow;
	bool compress_enabled;
	bool rle_enabled;
	bool is_writing;
	int cacheBufferAvail;
	char cacheBuffer[32760];
};

struct hunkHeader_t
{
	unsigned int magic;
	int size;
	const char* name;
	int dummy;
};

struct hunkUsed_t
{
	int permanent;
	int temp;
};

struct fileData_s
{
	void* data;
	fileData_s* next;
	char type;
	char name[1];
};

SIZE_T s_hunkTotal;
fileData_s* com_hunkData;
hunkUsed_t hunk_high;
hunkUsed_t hunk_low;
char* s_hunkData;
fileData_s* com_fileDataHashTable[1024];
char* s_origHunkData;
cmd_function_s Com_AllMemInfo_f_VAR;

LPVOID Z_VirtualReserve(int size);
bool Z_TryVirtualCommitInternal(void *ptr, int size);
void Z_VirtualCommitInternal(void* ptr, int size);
void Com_AllMemInfo_f();
void Com_TouchMemory();
char Hunk_CheckTempMemoryClear();
char Hunk_CheckTempMemoryHighClear();
void* Hunk_FindDataForFileInternal(int type, const char* name, int hash);
void* Hunk_FindDataForFile(int type, const char* name);
bool Hunk_DataOnHunk(void* data);
char* Hunk_SetDataForFile(int type, const char* name, void* data, void* (*alloc)(int));
void Hunk_AddData(int type, void* data, void* (* alloc)(int));
void Hunk_ClearDataFor(fileData_s** pFileData, char* low, char* high);
void Hunk_ClearData();
void Hunk_OverrideDataForFile(int type, const char* name, void* data);
void DB_EnumXAssets_LoadObj(XAssetType type, void(*func)(XAssetHeader, void*), void* inData);
char __cdecl DB_EnumXAssetsTimeout_LoadObj(XAssetType type, void(__cdecl* func)(XAssetHeader, void*), void* inData);
char DB_EnumXAssetsTimeout_FastFile(XAssetType type, void(* func)(XAssetHeader, void*), void* inData, bool includeOverride, int msec);
void Hunk_AddAsset(XAssetHeader header, void* data);
void DB_EnumXAssets();
int DB_EnumXAssetsTimeout();
int Hunk_Used();
void Z_VirtualCommit(void* ptr, int size);
void Z_VirtualFree(void* ptr);
void Z_VirtualDecommit(void* ptr, int size);
void Z_Free(void* ptr, int type);
void* Z_Malloc(int size, const char* name, int type);
char* Z_MallocGarbage(int size, const char* name, int type);
char* CopyString(const char* in);
void FreeString(const char* str);
int DB_GetAllXAssetOfType_LoadObj(XAssetType type, XAssetHeader* assets, int maxCount);
int DB_GetAllXAssetOfType();
void Hunk_ClearToMark(int mark);
void Hunk_ClearToMarkLow(int mark);
void Hunk_Clear();
char* Hunk_AllocAlign(int size, int alignment, const char* name, int type);
unsigned int Hunk_AllocateTempMemoryHigh(int size, const char* name);
void Hunk_ClearTempMemoryHigh();
char* Hunk_AllocLowAlign(int size, int alignment, const char* name, int type);
void* Hunk_AllocateTempMemory(int size, const char* name);
void Hunk_FreeTempMemory(void* buf);
void Hunk_ClearTempMemory();
void* Z_TryVirtualAllocInternal(int size, const char* name);
char* Z_TryVirtualAlloc(int size, const char* name, int type);
char* Z_VirtualAlloc(int size, const char* name, int type);
void ReplaceString(const char** str, const char* in);
void Com_InitHunkMemory();
char* Hunk_Alloc(int size, const char* name, int type);
char* Hunk_AllocLow(int size, const char* name, int type);
void Com_Memset(void* dest, const int val, int count);
