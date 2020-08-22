#include <emmintrin.h>

#include "mem_track.h"

#include "../defs.h"
#include "../universal/q_shared.h"
#include "../universal/assertive.h"

TempMemInfo* GetTempMemInfo(int permanent, const char* name, int type, int usageType, TempMemInfo* tempMemInfoArray, int* tempMemInfoCount, bool add_if_missing)
{
    __m128i x;
    TempMemInfo* v7; // esi
    int v8; // ebx
    TempMemInfo* v9; // edi
    TempMemInfo* v11; // eax
    TempMemInfo* v12; // edi
    TempMemInfo foundTempMemInfo; // [esp+Ch] [ebp-B0h]
    int count; // [esp+B8h] [ebp-4h]

    v7 = tempMemInfoArray;
    v8 = 0;
    count = *tempMemInfoCount;
    if (count <= 0)
    {
    LABEL_8:
        if (!add_if_missing)
            return 0;
#ifdef _DEBUG
        if (!name
            && !(unsigned char)assertive::Assert_MyHandler(
                "c:\\t6\\code\\src_noserver\\qcommon\\mem_track.cpp",
                308,
                0,
                "(name)",
                (const char*)&pBlock))
        {
            __debugbreak();
        }
        if (++ * tempMemInfoCount >= 1500
            && !(unsigned char)assertive::Assert_MyHandler(
                "c:\\t6\\code\\src_noserver\\qcommon\\mem_track.cpp",
                310,
                0,
                "(((*tempMemInfoCount) < 1500))",
                "(*tempMemInfoCount) = %i",
                *tempMemInfoCount))
        {
            __debugbreak();
        }
#endif
        v7->permanent = permanent;
        strcpy(v7->data.name, name);
        v7->data.usageType = usageType;
        v7->data.type = type;
        _mm_storel_epi64((__m128i*)v7->data.size, (__m128i)x);
        v7->data.filename = (const char*)&pBlock;
        v7->data.count = 0;
        v7->high = 0;
        v7->low = 0;
        v7->hunkSize = 0;
        v7->highExtra = 0;
    }
    else
    {
        v9 = tempMemInfoArray;
        while (1)
        {
            v7 = v9;
            if ((!name || !q_shared::I_stricmp(v9->data.name, name))
                && v9->permanent == permanent
                && (unsigned char)v9->data.usageType == usageType)
            {
                break;
            }
            ++v8;
            ++v9;
            ++v7;
            if (v8 >= count)
                goto LABEL_8;
        }
        if (!v8)
            return v9;
    }
    qmemcpy(&foundTempMemInfo, v7, sizeof(foundTempMemInfo));
    if (v8 > 0)
    {
        v11 = &tempMemInfoArray[v8];
        do
        {
            v12 = v11;
            --v8;
            --v11;
            qmemcpy(v12, v11, sizeof(TempMemInfo));
        } while (v8 > 0);
    }
    qmemcpy(tempMemInfoArray, &foundTempMemInfo, sizeof(TempMemInfo));
    return tempMemInfoArray;
}

void track_z_alloc(int size, const char* name, int type, void* pos, int project, int overhead)
{
    TempMemInfo* v6; // eax
    int v7; // ecx
    int v8; // eax
    mem_track_node_s* v9; // esi
    mem_track_node_s* v10; // eax

    if (size < 0
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(size>=0)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    Sys_EnterCriticalSection(CRITSECT_MEMTRACK);
    g_staticsMemTrack[0].size[0] += overhead;
    g_info.typeTotal[0][0] += overhead;
    g_info.total[0] += overhead;
    if (type != 55)
    {
        v6 = GetTempMemInfo(0, name, type, 2, g_mallocMemInfoArray, &g_mallocMemInfoCount, 1);
        v6->data.size[0] += size;
        v7 = v6->data.size[0];
        ++v6->data.count;
        if (v6->high < v7)
            v6->high = v7;
        if (v6->hunkSize < 0)
        {
            v6->hunkSize = 0;
            v6->highExtra = v7;
        }
        v8 = size + g_malloc_mem_size;
        g_malloc_mem_size = v8;
        if (g_malloc_mem_high < v8)
            g_malloc_mem_high = v8;
    }
    v9 = (mem_track_node_s*)((char*)pos - 164);
    if (!*name
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(name[0])",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    strcpy(v9->data.name, name);
    v10 = g_ZMallocMemTrackList;
    v9->data.filename = (const char*)&pBlock;
    v9->data.size[0] = size;
    v9->data.size[1] = 0;
    v9->data.type = type;
    v9->data.usageType = 2;
    v9->next = v10;
    v9->prev = 0;
    v9->project = project;
    if (v10)
        v10->prev = v9;
    g_info.typeTotal[type][0] += size;
    g_info.total[0] += size;
    g_ZMallocMemTrackList = (mem_track_node_s*)((char*)pos - 164);
    switch (type)
    {
    case 55:
        break;
    case 54:
        g_info.nonSwapTotal += size;
        break;
    }
    switch (type)
    {
    case 55:
        break;
    case 54:
        g_info.nonSwapMinSpecTotal += size;
        break;
    }
    Sys_LeaveCriticalSection(CRITSECT_MEMTRACK);
}

void track_z_free(int type, void* pos, int overhead)
{
    int v3; // eax
    mem_track_node_s* v4; // edi
    int v5; // ebx
    TempMemInfo* v6; // eax
    int v7; // ecx
    int v8; // eax
    mem_track_node_s* v9; // edi
    mem_track_node_s* v10; // eax

    Sys_EnterCriticalSection(CRITSECT_MEMTRACK);
    v3 = (unsigned char)*((char*)pos - 20);
    v4 = (mem_track_node_s*)((char*)pos - 164);
    #ifdef _DEBUG
    if (v3 != type
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(node->data.type) == (type)",
            "node->data.type == type\n\t%i, %i",
            v3,
            type))
    {
        __debugbreak();
    }
    #endif // _DEBUG
    g_staticsMemTrack[0].size[0] -= overhead;
    g_info.typeTotal[0][0] -= overhead;
    g_info.total[0] -= overhead;
    if (type != 55)
    {
        v5 = v4->data.size[0];
        g_malloc_mem_size -= v5;
        if (g_malloc_mem_size < 0)
            g_malloc_mem_size = 0;
        v6 = GetTempMemInfo(0, v4->data.name, type, 2, g_mallocMemInfoArray, &g_mallocMemInfoCount, 0);
        if (v6)
        {
            v6->data.size[0] -= v5;
            v7 = v6->data.size[0];
            --v6->data.count;
            if (v6->low > v7)
                v6->low = v7;
        }
    }
    v8 = -v4->data.size[0];
    g_info.typeTotal[type][0] -= v4->data.size[0];
    g_info.total[0] += v8;
    switch (type)
    {
    case 55:
        break;
    case 54:
        g_info.nonSwapTotal += v8;
        break;
    }
    switch (type)
    {
    case 55:
        break;
    case 54:
        g_info.nonSwapMinSpecTotal += v8;
        break;
    }
    if (v4 == g_ZMallocMemTrackList)
    {
        v9 = v4->next;
        g_ZMallocMemTrackList = v9;
        if (v9)
        {
            v9->prev = 0;
            Sys_LeaveCriticalSection(CRITSECT_MEMTRACK);
            return;
        }
    }
    else
    {
        #ifdef _DEBUG
        if (!v4->prev
            && !(unsigned char)assertive::Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(node->prev)",
                (const char*)&pBlock))
        {
            __debugbreak();
        }
        #endif
        v4->prev->next = v4->next;
        v10 = v4->next;
        if (v10)
            v10->prev = v4->prev;
    }
    Sys_LeaveCriticalSection(CRITSECT_MEMTRACK);
}