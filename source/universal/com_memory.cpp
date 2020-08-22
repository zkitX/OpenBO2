#include "com_memory.h"

#include "assertive.h"
#include "../qcommon/mem_track.h"
#include "../win32/win_main.h"

void Com_Memset(void* dest, const int val, int count)
{
    memset((char*)dest, val, count);
}

void* Z_Malloc(int size, const char* name, int type)
{
    char* v3; // esi
    void* v4; // esi

    v3 = (char*)malloc(size + 164);
    if (!v3 || (v4 = v3 + 164, track_z_alloc(size + 72, name, type, v4, 0, 164), !v4))
    {
        Com_PrintError(10, "Failed to Z_Malloc %i bytes\n", size + 164);
        Sys_OutOfMemErrorInternal((int)v3, __FILE__, __LINE__);
    }
    Com_Memset(v4, 0, size);
    return v4;
}

char* Z_MallocGarbage(int size, const char* name, int type)
{
    char* v3; // esi

    v3 = (char*)malloc(size + 164);
    if (!v3 || (v3 += 164, track_z_alloc(size + 72, name, type, v3, 0, 164), !v3))
    {
        Com_PrintError((int)v3, 10, "Failed to Z_Malloc %i bytes\n", size + 164);
        Sys_OutOfMemErrorInternal((int)v3, __FILE__, __LINE__);
    }
    return v3;
}
