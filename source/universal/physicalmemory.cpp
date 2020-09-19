#include "physicalmemory.h"

#include "assertive.h"
#include "../win32/win_main.h"
#include <tommath.h>

#ifdef T6MP
#define T6AllocSize 0x20000000u
#elif T6SP
#define T6AllocSize 0x21000000u
#elif T6ZM
#define T6AllocSize 0x20000000u
#endif

void PMem_InitPhysicalMemory(PhysicalMemory* pmem, unsigned int memorySize, const char* name, void* memory)
{
#ifdef _DEBUG
    if (!pmem
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(pmem)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (!memory
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(memory)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
#endif
    memset((char*)pmem, 0, 0x52Cu);
    pmem->buf = (char*)memory;
    pmem->name = name;
    pmem->prim[1].pos = memorySize;
    pmem->size = memorySize;
}

void PMem_Init()
{
    LPVOID memory;

    if (g_physicalMemoryInit
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(g_physicalMemoryInit == 0)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (!g_physicalMemoryInit)
    {
        g_physicalMemoryInit = 1;
        memory = VirtualAlloc(0, T6AllocSize, 0x1000u, 4u);
        PMem_InitPhysicalMemory(&g_mem, T6AllocSize, "main", memory);
    }
}

void PMem_BeginAllocInPrim(PhysicalMemoryPrim* prim, const char* name, EMemTrack memTrack)
{
    PhysicalMemoryAllocation* allocEntry; // ST14_4

    if (prim->allocName
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "!prim->allocName"))
    {
        __debugbreak();
    }
    if (prim->allocListCount >= 0x20
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "prim->allocListCount < MAX_PHYSICAL_ALLOCATIONS"))
    {
        __debugbreak();
    }
    prim->allocName = name;
    prim->memTrack = memTrack;
    allocEntry = &prim->allocList[prim->allocListCount++];
    allocEntry->name = name;
    allocEntry->pos = prim->pos;
}

void PMem_BeginAlloc(const char* name, unsigned int allocType, EMemTrack memTrack)
{
    g_alloc_type = allocType;
    if (allocType >= 2
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(allocType) < (unsigned)(PHYS_ALLOC_COUNT)",
            "allocType doesn't index PHYS_ALLOC_COUNT\n\t%i not in [0, %i)",
            allocType,
            2))
    {
        __debugbreak();
    }
    PMem_BeginAllocInPrim(&g_mem.prim[allocType], name, memTrack);
}

void PMem_EndAllocInPrim(PhysicalMemoryPrim* prim, const char* name, unsigned int allocType)
{
    if (I_stricmp(prim->allocName, name)
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "I_stricmp( prim->allocName, name ) == 0"))
    {
        __debugbreak();
    }
    prim->allocName = 0;
    g_alloc_type = allocType != 1;
}

void PMem_EndAlloc(const char* name, unsigned int allocType)
{
    if (allocType >= 2
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(allocType) < (unsigned)(PHYS_ALLOC_COUNT)",
            "allocType doesn't index PHYS_ALLOC_COUNT\n\t%i not in [0, %i)",
            allocType,
            2))
    {
        __debugbreak();
    }
    PMem_EndAllocInPrim(&g_mem.prim[allocType], name, allocType);
}

void PMem_FreeIndex(PhysicalMemoryPrim* prim, unsigned int allocIndex, int location)
{
    PhysicalMemoryAllocation* allocEntry; // [esp+8h] [ebp-8h]
    const char* name; // [esp+Ch] [ebp-4h]

    if (prim->allocName
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "!prim->allocName"))
    {
        __debugbreak();
    }
    allocEntry = &prim->allocList[allocIndex];
    name = allocEntry->name;
    if (!allocEntry->name
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "name"))
    {
        __debugbreak();
    }
    allocEntry->name = 0;
    track_flush_physical_alloc(name, prim->memTrack);
    if (allocIndex == prim->allocListCount - 1)
    {
        track_physical_alloc(-abs((int)prim->pos - (int)prim->allocList[allocIndex].pos), name, prim->memTrack, location);
        do
        {
            prim->pos = allocEntry->pos;
            if (!prim->allocListCount
                && !(unsigned __int8)Assert_MyHandler(
                    __FILE__,
                    __LINE__,
                    0,
                    "%s",
                    "prim->allocListCount"))
            {
                __debugbreak();
            }
            if (!--prim->allocListCount)
                break;
            allocEntry = (PhysicalMemoryAllocation*)(&prim->allocListCount + 2 * prim->allocListCount);
        } while (!allocEntry->name);
    }
    else
    {
        track_PrintInfo();
        Com_Error(ERR_FATAL, "free does not match allocation");
    }
}

void PMem_FreeInPrim(PhysicalMemoryPrim* prim, const char* name, int location)
{
    unsigned int allocIndex; // [esp+0h] [ebp-8h]

    for (allocIndex = 0; allocIndex < prim->allocListCount; ++allocIndex)
    {
        if (prim->allocList[allocIndex].name == name)
        {
            PMem_FreeIndex(prim, allocIndex, location);
            return;
        }
    }
}

void PMem_Free(const char* name)
{
    int i; // [esp+4h] [ebp-4h]

    for (i = 0; i < 2; ++i)
    {
        if (i)
            Com_Printf(16, "PMem_Free( %s, %s )\n", name, "High");
        else
            Com_Printf(16, "PMem_Free( %s, %s )\n", name, "Low");
        PMem_FreeInPrim(&g_mem.prim[i], name, 0);
    }
}

char* PMem_AllocNamed(unsigned int size, unsigned int alignment, unsigned int type, unsigned int allocType, const char* name, EMemTrack memTrack)
{
    int alignedSize; // ST24_4
    int v7; // ST24_4
    char* result; // ST20_4
    unsigned int highPos; // [esp+8h] [ebp-20h]
    signed int location; // [esp+18h] [ebp-10h]
    unsigned int lowPos; // [esp+1Ch] [ebp-Ch]
    unsigned int allocedSize; // [esp+20h] [ebp-8h]
    unsigned int alignmenta; // [esp+34h] [ebp+Ch]

    location = 0;
    PMem_Init();
    if ((type & 0x80000000) != 0)
        location = 1;
    if (!size
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "size"))
    {
        __debugbreak();
    }
    if (!alignment
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "alignment"))
    {
        __debugbreak();
    }
    alignmenta = alignment - 1;
    if (allocType)
    {
        if (allocType != 1
            && !(unsigned __int8)Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "%s",
                "allocType == PHYS_ALLOC_HIGH"))
        {
            __debugbreak();
        }
        v7 = size + (~alignmenta & (alignmenta + g_mem.prim[allocType].pos)) - g_mem.prim[allocType].pos;
        lowPos = ~alignmenta & (g_mem.prim[allocType].pos - size);
        g_overAllocatedSize = g_mem.prim[0].pos - lowPos;
        if (g_overAllocatedSize > 0)
        {
            Com_Printf(16, "requested size:  %d  over allocation:  %d\n", size, g_overAllocatedSize);
            Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
        }
        allocedSize = g_mem.prim[allocType].pos - lowPos;
        g_mem.prim[allocType].pos = lowPos;
    }
    else
    {
        lowPos = ~alignmenta & (alignmenta + g_mem.prim[allocType].pos);
        highPos = size + lowPos;
        alignedSize = size + lowPos - g_mem.prim[allocType].pos;
        g_overAllocatedSize = size + lowPos - g_mem.prim[1].pos;
        if (g_overAllocatedSize > 0)
        {
            Com_PrintError(
                16,
                "Need %i more bytes of '%s' physical ram for alloc to succeed\n",
                g_overAllocatedSize,
                g_mem.name);
            Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
        }
        allocedSize = highPos - g_mem.prim[allocType].pos;
        g_mem.prim[allocType].pos = highPos;
    }
    result = &g_mem.buf[lowPos];
    track_physical_alloc(allocedSize, name, memTrack, location);
    return result;
}

void* PMem_Alloc(unsigned int size, unsigned int alignment, unsigned int type, unsigned int allocType, EMemTrack memTrack, const char* file, int lineNum)
{
    char allocIdBuf[256]; // [esp+0h] [ebp-108h]

    sprintf(allocIdBuf, "%s::%s %d", g_mem.prim[allocType].allocName, file, lineNum);
    return PMem_AllocNamed(size, alignment, type, allocType, allocIdBuf, memTrack);
}
