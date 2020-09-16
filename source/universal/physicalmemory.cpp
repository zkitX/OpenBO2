#include "physicalmemory.h"

#include "assertive.h"
#include "../win32/win_main.h"

char* _PMem_AllocNamed(unsigned int size, unsigned int alignment, unsigned int type, unsigned int allocType, const char* name, EMemTrack memTrack)
{
    PhysicalMemoryPrim* memprim;
    unsigned int v7; // edi
    int v8; // esi
    unsigned int v9; // edi
    int v10; // eax
    unsigned int v11; // edi
    char* v12; // esi
    signed int location;
    int allocedSize;
    signed int typea;

    location = 0;
    if ((type & 0x80000000) != 0)
        location = 1;
    memprim = &g_mem.prim[allocType];
#ifdef _DEBUG
    if (!size
        && !(unsigned char)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(size)",
            &pBlock))
    {
        __debugbreak();
    }
    if (!alignment
        && !(unsigned char)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(alignment)",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    v7 = alignment - 1;
#ifdef _DEBUG
    if (allocType >= 2
        && !(unsigned char)Assert_MyHandler(
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
#endif
    typea = allocType + 0x12;
    Sys_EnterCriticalSection((CriticalSection)(allocType + 0x12));
    if (allocType) {
#ifdef _DEBUG
        if (allocType != 1
            && !(unsigned char)Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(allocType == PHYS_ALLOC_HIGH)",
                &pBlock))
        {
            __debugbreak();
        }
#endif
        v8 = ~v7 & (memprim->pos - size);
        v11 = allocType;
        g_overAllocatedSize = g_mem.prim[0].pos - v8;
        if ((signed int)(g_mem.prim[0].pos - v8) > 0)
        {
#ifdef _DEBUG
            if (allocType >= 2
                && !(unsigned char)Assert_MyHandler(
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
#endif
            Sys_LeaveCriticalSection((CriticalSection)typea);
            Com_Printf(10, "requested size:  %d  over allocation:  %d\n", size, g_overAllocatedSize);
            Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
        }
        v10 = memprim->pos - v8;
        memprim->pos = v8;
    }
    else {
        v8 = ~v7 & (v7 + memprim->pos);
        v9 = v8 + size;
        g_overAllocatedSize = v8 + size - g_mem.prim[1].pos;
        if ((signed int)(v8 + size - g_mem.prim[1].pos) > 0)
        {
            Sys_LeaveCriticalSection(CRITSECT_MEM_ALLOC0);
            Com_PrintError(
                10,
                "Need %i more bytes of '%s' physical ram for alloc to succeed\n",
                g_overAllocatedSize,
                g_mem.name);
            Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
        }
        v10 = v9 - memprim->pos;
        memprim->pos = v9;
        v11 = allocType;
    }
    v12 = &g_mem.buf[v8];
    allocedSize = v10;
#ifdef _DEBUG
    if (v11 >= 2
        && !(unsigned char)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(allocType) < (unsigned)(PHYS_ALLOC_COUNT)",
            "allocType doesn't index PHYS_ALLOC_COUNT\n\t%i not in [0, %i)",
            v11,
            2))
    {
        __debugbreak();
    }
#endif
    Sys_LeaveCriticalSection((CriticalSection)typea);
    track_physical_alloc(allocedSize, name, memTrack, location);
    return v12;
}
