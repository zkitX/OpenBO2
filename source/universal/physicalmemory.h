#pragma once

#include <qcommon/mem_track.h>

struct PhysicalMemoryAllocation
{
	const char* name;
	unsigned int pos;
};

struct PhysicalMemoryPrim
{
	const char* allocName;
	unsigned int allocListCount;
	unsigned int pos;
	PhysicalMemoryAllocation allocList[80];
	EMemTrack memTrack;
};

struct PhysicalMemory
{
	const char* name;
	char* buf;
	PhysicalMemoryPrim prim[2];
	unsigned int size;
};

PhysicalMemory g_mem;
bool g_physicalMemoryInit;
int g_overAllocatedSize;
__declspec(thread) int g_alloc_type;

void PMem_InitPhysicalMemory(PhysicalMemory* pmem, unsigned int memorySize, const char* name, void* memory);
void PMem_Init();
void PMem_BeginAllocInPrim(PhysicalMemoryPrim* prim, const char* name, EMemTrack memTrack);
void PMem_BeginAlloc(const char* name, unsigned int allocType, EMemTrack memTrack);
void PMem_EndAllocInPrim(PhysicalMemoryPrim* prim, const char* name, unsigned int allocType);
void PMem_EndAlloc(const char* name, unsigned int allocType);
void PMem_FreeIndex(PhysicalMemoryPrim* prim, unsigned int allocIndex, int location);
void PMem_FreeInPrim(PhysicalMemoryPrim* prim, const char* name, int location);
void PMem_Free(const char* name);
char* PMem_AllocNamed(unsigned int size, unsigned int alignment, unsigned int type, unsigned int allocType, const char* name, EMemTrack memTrack);
void* PMem_Alloc(unsigned int size, unsigned int alignment, unsigned int type, unsigned int allocType, EMemTrack memTrack, const char* file, int lineNum);