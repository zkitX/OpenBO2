#include "com_memory.h"

#include "assertive.h"
#include <qcommon/mem_track.h>
#include <qcommon/threads.h>
#include <win32/win_main.h>

#include <universal/dvar.h>

#include <xanim/xmodel.h>
#include <xanim/xanim.h>

LPVOID Z_VirtualReserve(int size)
{
    LPVOID v1; // esi

#ifdef _DEBUG
    if (size <= 0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((size > 0))",
            "(size) = %i",
            size))
    {
        __debugbreak();
    }
#endif
    v1 = VirtualAlloc(0, size, (size > 0x20000 ? 0 : 0x100000) | 0x2000, 4u);
#ifdef _DEBUG
    if (!v1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(buf)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
#endif
    return v1;
}

void Z_VirtualCommitInternal(void* ptr, int size)
{
#ifdef _DEBUG
    if (size < 0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((size >= 0))",
            "(size) = %i",
            size))
    {
        __debugbreak();
    }
#endif
    if (!VirtualAlloc(ptr, size, (size > 0x20000 ? 0 : 0x100000) | 0x1000, 4u))
        Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
}

void Com_AllMemInfo_f()
{
    //track_PrintAllInfo();
}

bool Z_TryVirtualCommitInternal(void* ptr, int size)
{
#ifdef _DEBUG
    if (size < 0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((size >= 0))",
            "(size) = %i",
            size))
    {
        __debugbreak();
    }
#endif
    return VirtualAlloc(ptr, size, (size > 0x20000 ? 0 : 0x100000) | 0x1000, 4u) != 0;
}

void Com_TouchMemory()
{
    int end; // ST20_4
    int sum; // [esp+4h] [ebp-10h]
    int start; // [esp+8h] [ebp-Ch]
    int i; // [esp+10h] [ebp-4h]
    int ia; // [esp+10h] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Sys_IsRenderThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread() || Sys_IsRenderThread()"))
    {
        __debugbreak();
    }
    start = Sys_Milliseconds();
    sum = 0;
    for (i = 0; i < hunk_low.permanent >> 2; i += 64)
        sum += *(unsigned int*)&s_hunkData[4 * i];
    for (ia = (s_hunkTotal - hunk_high.permanent) >> 2; ia < hunk_high.permanent >> 2; ia += 64)
        sum += *(unsigned int*)&s_hunkData[4 * ia];
    end = Sys_Milliseconds();
    Com_Printf(16, "Com_TouchMemory: %i msec. Using sum: %d\n", end - start, sum);
}

char Hunk_CheckTempMemoryClear()
{
    char result; // al

    if (!Sys_IsMainThread()
        && !Sys_IsRenderThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread() || Sys_IsRenderThread()"))
    {
        __debugbreak();
    }
    if (!s_hunkData
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "s_hunkData"))
    {
        __debugbreak();
    }
    result = hunk_low.temp;
    if (hunk_low.temp != hunk_low.permanent)
    {
        result = Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "hunk_low.temp == hunk_low.permanent");
        if (!result)
            __debugbreak();
    }
    return result;
}

char Hunk_CheckTempMemoryHighClear()
{
    char result; // al

    if (!Sys_IsMainThread()
        && !Sys_IsRenderThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread() || Sys_IsRenderThread()"))
    {
        __debugbreak();
    }
    if (!s_hunkData
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "s_hunkData"))
    {
        __debugbreak();
    }
    result = hunk_high.temp;
    if (hunk_high.temp != hunk_high.permanent)
    {
        result = Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "hunk_high.temp == hunk_high.permanent");
        if (!result)
            __debugbreak();
    }
    return result;
}

void* Hunk_FindDataForFileInternal(int type, const char* name, int hash)
{
    fileData_s* searchFileData;

    for (searchFileData = com_fileDataHashTable[hash]; searchFileData; searchFileData = searchFileData->next)
    {
        if ((unsigned __int8)searchFileData->type == type && !I_stricmp(searchFileData->name, name))
            return searchFileData->data;
    }
    return 0;
}

void* Hunk_FindDataForFile(int type, const char* name)
{
    int hash; // ST0C_4

    hash = FS_HashFileName(name, 1024);
    return Hunk_FindDataForFileInternal(type, name, hash);
}

bool Hunk_DataOnHunk(void* data)
{
    bool result; // eax

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(Sys_IsMainThread())",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (!s_hunkData
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(s_hunkData)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (data >= s_hunkData)
        result = data < &s_hunkData[s_hunkTotal];
    else
        result = 0;
    return result;
}

char* Hunk_SetDataForFile(int type, const char* name, void* data, void* (*alloc)(int))
{
    const char* v4; // ebx
    const char* v5; // edi
    fileData_s* v6; // edi
    char* result; // eax
    int hash; // [esp+Ch] [ebp+Ch]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(Sys_IsMainThread())",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    v4 = name;
    v5 = name;
    hash = FS_HashFileName(name, 1024);
    if (Hunk_FindDataForFileInternal(type, v5, hash)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(!Hunk_FindDataForFileInternal( type, name, hash ))",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    v6 = (fileData_s*)alloc(strlen(v4) + 10);
    if (!Hunk_DataOnHunk(v6)
        && !Assert_MyHandler(
            "c:\\t6\\code\\src_noserver\\universal\\com_memory.cpp",
            814,
            0,
            "(Hunk_DataOnHunk( fileData ))",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    v6->data = data;
    v6->type = type;
    if (type != (unsigned __int8)type
        && !Assert_MyHandler(
            "c:\\t6\\code\\src_noserver\\universal\\com_memory.cpp",
            819,
            0,
            "(type == fileData->type)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    result = v6->name;
    strcpy(v6->name, v4);
    v6->next = com_fileDataHashTable[hash];
    com_fileDataHashTable[hash] = v6;
    return result;
}

void Hunk_AddData(int type, void* data, void* (*alloc)(int))
{
    void* v3; // eax
    fileData_s* fileData; // [esp+0h] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    v3 = (void*)((int(__stdcall*)(signed int))alloc)(9);
    if (!Hunk_DataOnHunk(v3)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Hunk_DataOnHunk( fileData )"))
    {
        __debugbreak();
    }
    fileData->data = data;
    fileData->type = type;
    if (type != (unsigned __int8)fileData->type
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "type == fileData->type"))
    {
        __debugbreak();
    }
    fileData->next = com_hunkData;
    com_hunkData = fileData;
}

void Hunk_ClearDataFor(fileData_s** pFileData, char* low, char* high)
{
    char v3; // [esp+0h] [ebp-Ch]
    XAnim_s* data; // [esp+4h] [ebp-8h]
    fileData_s* fileData; // [esp+8h] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    while (*pFileData)
    {
        fileData = *pFileData;
        if (*pFileData >= (fileData_s*)low && fileData < (fileData_s*)high)
        {
            *pFileData = fileData->next;
            data = (XAnim_s*)fileData->data;
            v3 = fileData->type;
            switch (v3)
            {
            case 2:
                XAnimFreeList(data);
                break;
            case 4:
                XModelPartsFree((XModelPartsLoad*)data);
                break;
            case 6:
                XAnimFree((XAnimParts*)data);
                break;
            }
        }
        else
        {
            pFileData = &fileData->next;
        }
    }
}

void Hunk_ClearData()
{
    char* low; // [esp+0h] [ebp-10h]
    unsigned int hash; // [esp+4h] [ebp-Ch]
    char* high; // [esp+Ch] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    low = &s_hunkData[hunk_low.permanent];
    high = &s_hunkData[s_hunkTotal - hunk_high.permanent];
    for (hash = 0; hash < 0x400; ++hash)
        Hunk_ClearDataFor((fileData_s**)(&com_hunkData[hash]), low, high);
    Hunk_ClearDataFor(&com_hunkData, low, high);
}

void Hunk_OverrideDataForFile(int type, const char* name, void* data)
{
    fileData_s* searchFileData; // [esp+4h] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    for (searchFileData = com_fileDataHashTable[FS_HashFileName(name, 1024)];
        searchFileData;
        searchFileData = searchFileData->next)
    {
        if ((unsigned __int8)searchFileData->type == type && !I_stricmp(searchFileData->name, name))
        {
            searchFileData->data = data;
            return;
        }
    }
    if (!Assert_MyHandler(
        __FILE__,
        __LINE__,
        0,
        "Hunk_OverrideDataForFile: could not find data"))
        __debugbreak();
}

void DB_EnumXAssets_LoadObj(XAssetType type, void(* func)(XAssetHeader, void*), void* inData)
{
    unsigned int hash; // [esp+4h] [ebp-Ch]

    switch (type)
    {
    case 5:
        for (hash = 0; hash < 0x400; ++hash)
            DB_EnumXAssetsFor(com_fileDataHashTable[hash], 5, func, inData);
        break;
    case 6:
        R_EnumMaterials(func, inData);
        break;
    case 7:
        R_EnumTechniqueSets(func, inData);
        break;
    case 8:
        R_EnumImages(func, inData);
        break;
    }
}

char __cdecl DB_EnumXAssetsTimeout_LoadObj(XAssetType type, void(__cdecl* func)(XAssetHeader, void*), void* inData)
{
    DB_EnumXAssets_LoadObj(type, func, inData);
    return 1;
}

char DB_EnumXAssetsTimeout_FastFile(XAssetType type, void(* func)(XAssetHeader, void*), void* inData, bool includeOverride, int msec)
{
    // TODO
    return 0;
}

void Hunk_AddAsset(XAssetHeader header, void* data)
{
    if (!data && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "data"))
        __debugbreak();
    if (*(unsigned int*)data >= *((unsigned int*)data + 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "assetList->assetCount < assetList->maxCount"))
    {
        __debugbreak();
    }
    // TODO: Make more sense
    *(XAssetHeader*)(*((unsigned int*)data + 2) + 4 * (*(unsigned int*)data)++) = header;
}

void DB_EnumXAssets()
{
    bool v0; // zf
    void (*v1)(void); // eax

    v0 = Dvar_GetBool(useFastFile) == 0;
    v1 = (void (*)(void))DB_EnumXAssets_FastFile;
    if (v0)
        v1 = (void (*)(void))DB_EnumXAssets_LoadObj;
    v1();
}

int DB_EnumXAssetsTimeout()
{
    bool v0; // zf
    int (*v1)(void); // eax

    v0 = Dvar_GetBool(useFastFile) == 0;
    v1 = (int (*)(void))DB_EnumXAssetsTimeout_FastFile;
    if (v0)
        v1 = (int (*)(void))DB_EnumXAssetsTimeout_LoadObj;
    return v1();
}

int Hunk_Used()
{
    if (!Sys_IsMainThread()
        && !Sys_IsRenderThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread() || Sys_IsRenderThread()"))
    {
        __debugbreak();
    }
    return hunk_high.permanent + hunk_low.permanent;
}

void Z_VirtualCommit(void* ptr, int size)
{
    Z_VirtualCommitInternal(ptr, size);
}

void Z_VirtualFree(void* ptr)
{
    VirtualFree(ptr, 0, 0x8000u);
}

void Z_VirtualDecommit(void* ptr, int size)
{
    if (size < 0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((size >= 0))",
            "(size) = %i",
            size))
    {
        __debugbreak();
    }
    VirtualFree(ptr, size, 0x4000u);
}

void Z_Free(void* ptr, int type)
{
    if (ptr)
    {
        track_z_free(type, ptr, 164);
        free((char*)ptr - 164);
    }
}

void* Z_Malloc(int size, const char* name, int type)
{
    char* v3; // esi
    void* v4; // esi

    v3 = (char*)malloc(size + 164);
    if (!v3 || (v4 = v3 + 164, track_z_alloc(size + 72, name, type, v4, 0, 164), !v4))
    {
        Com_PrintError(10, "Failed to Z_Malloc %i bytes\n", size + 164);
        Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
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
        Com_PrintError(10, "Failed to Z_Malloc %i bytes\n", size + 164);
        Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
    }
    return v3;
}

char* CopyString(const char* in)
{
    unsigned int out;

    if (!in && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "in"))
        __debugbreak();
    out = SL_GetString_(in, 0, 22);
    return SL_ConvertToString(out);
}

void FreeString(const char* str)
{
    unsigned int out;

    if (!str && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "str"))
        __debugbreak();
    out = SL_FindString(str);
    if (!out && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "out"))
        __debugbreak();
    SL_RemoveRefToString(out);
}

int DB_GetAllXAssetOfType_LoadObj(XAssetType type, XAssetHeader* assets, int maxCount)
{
    bool v3; // zf
    void(__cdecl * v4)(XAssetType, void(__cdecl*)(XAssetHeader, void*), void*); // eax
    AssetList assetList; // [esp+0h] [ebp-Ch]

    assetList.assets = assets;
    assetList.assetCount = 0;
    assetList.maxCount = maxCount;
    v3 = Dvar_GetBool(useFastFile) == 0;
    v4 = (void(__cdecl*)(XAssetType, void(__cdecl*)(XAssetHeader, void*), void*))DB_EnumXAssets_FastFile;
    if (v3)
        v4 = DB_EnumXAssets_LoadObj;
    ((void(__cdecl*)(XAssetType, void(__cdecl*)(XAssetHeader, void*), void*, bool))v4)(
        type,
        Hunk_AddAsset,
        &assetList,
        0);
    return assetList.assetCount;
}

int DB_GetAllXAssetOfType()
{
    bool v0; // zf
    int (*v1)(void); // eax

    v0 = Dvar_GetBool(useFastFile) == 0;
    v1 = (int (*)(void))DB_GetAllXAssetOfType_FastFile;
    if (v0)
        v1 = (int (*)(void))DB_GetAllXAssetOfType_LoadObj;
    return v1();
}

void Hunk_ClearToMark(int mark)
{
    char* beginBuf; // [esp+8h] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    Hunk_CheckTempMemoryHighClear();
    beginBuf = (char*)((unsigned int)&s_hunkData[s_hunkTotal - hunk_high.temp] & 0xFFFFF000);
    hunk_high.temp = mark;
    hunk_high.permanent = mark;
    Hunk_ClearData();
    if ((char*)((unsigned int)&s_hunkData[s_hunkTotal - hunk_high.temp] & 0xFFFFF000) != beginBuf)
        Z_VirtualDecommit(
            beginBuf,
            ((unsigned int)&s_hunkData[s_hunkTotal - hunk_high.temp] & 0xFFFFF000) - (unsigned int)beginBuf);
    track_hunk_ClearToMarkHigh(mark);
}

void Hunk_ClearToMarkLow(int mark)
{
    char* endBuf; // [esp+0h] [ebp-Ch]
    char* beginBuf; // [esp+8h] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    Hunk_CheckTempMemoryClear();
    endBuf = (char*)((unsigned int)&s_hunkData[hunk_low.temp + 4095] & 0xFFFFF000);
    hunk_low.temp = mark;
    hunk_low.permanent = mark;
    Hunk_ClearData();
    beginBuf = (char*)((unsigned int)&s_hunkData[hunk_low.temp + 4095] & 0xFFFFF000);
    if (endBuf != beginBuf)
        Z_VirtualDecommit(beginBuf, endBuf - beginBuf);
    track_hunk_ClearToMarkLow(mark);
}

void Hunk_Clear()
{
    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    hunk_low.permanent = 0;
    hunk_low.temp = 0;
    hunk_high.permanent = 0;
    hunk_high.temp = 0;
    Hunk_ClearData();
    Z_VirtualDecommit(s_hunkData, s_hunkTotal);
    track_hunk_ClearToMarkLow(0);
    track_hunk_ClearToMarkHigh(0);
    track_hunk_ClearToStart();
}

char* Hunk_AllocAlign(int size, int alignment, const char* name, int type)
{
    int old_permanent; // [esp+0h] [ebp-14h]
    char* buf; // [esp+4h] [ebp-10h]
    char* endBuf; // [esp+8h] [ebp-Ch]
    int alignmenta; // [esp+20h] [ebp+Ch]

    if (!Sys_IsMainThread()
        && !Sys_IsRenderThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread() || Sys_IsRenderThread()"))
    {
        __debugbreak();
    }
    if (!s_hunkData
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "s_hunkData"))
    {
        __debugbreak();
    }
    if (alignment & (alignment - 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "!(alignment & (alignment - 1))"))
    {
        __debugbreak();
    }
    if (alignment > 4096
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "alignment <= HUNK_MAX_ALIGNMENT"))
    {
        __debugbreak();
    }
    alignmenta = alignment - 1;
    Hunk_CheckTempMemoryHighClear();
    old_permanent = hunk_high.permanent;
    endBuf = (char*)((unsigned int)&s_hunkData[s_hunkTotal - hunk_high.permanent] & 0xFFFFF000);
    hunk_high.permanent += size;
    hunk_high.permanent = ~alignmenta & (alignmenta + hunk_high.permanent);
    hunk_high.temp = hunk_high.permanent;
    if (hunk_high.permanent + hunk_low.temp > s_hunkTotal)
    {
        track_PrintAllInfo();
        Com_Error(ERR_DROP, "Hunk_AllocAlign failed on % i bytes(total % i MB, low % i MB, high % i MB)", size, s_hunkTotal / 0x100000, hunk_low.temp / 0x100000, hunk_high.temp / 0x100000);
    }
    buf = &s_hunkData[s_hunkTotal - hunk_high.permanent];
    if (alignmenta & (unsigned int)buf
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "!(((psize_int)buf) & alignment)"))
    {
        __debugbreak();
    }
    if (endBuf != (char*)((unsigned int)buf & 0xFFFFF000))
        Z_VirtualCommit((void*)((unsigned int)buf & 0xFFFFF000), (int)&endBuf[-((unsigned int)buf & 0xFFFFF000)]);
    track_hunk_alloc(hunk_high.permanent - old_permanent, hunk_high.temp, name, type);
    memset(buf, 0, size);
    return buf;
}

unsigned int Hunk_AllocateTempMemoryHigh(int size, const char* name)
{
    unsigned int buf; // [esp+0h] [ebp-10h]
    char* endBuf; // [esp+4h] [ebp-Ch]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    if (!s_hunkData
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "s_hunkData"))
    {
        __debugbreak();
    }
    endBuf = (char*)((unsigned int)&s_hunkData[s_hunkTotal - hunk_high.temp] & 0xFFFFF000);
    hunk_high.temp += size;
    hunk_high.temp = (hunk_high.temp + 15) & 0xFFFFFFF0;
    if (hunk_high.temp + hunk_low.temp > s_hunkTotal)
    {
        track_PrintAllInfo();
        Com_Error(
            ERR_DROP,
            "Hunk_AllocateTempMemoryHigh: failed on %i bytes (total %i MB, low %i MB, high %i MB)",
            size,
            s_hunkTotal / 0x100000,
            hunk_low.temp / 0x100000,
            hunk_high.temp / 0x100000);
    }
    buf = (unsigned int)&s_hunkData[s_hunkTotal - hunk_high.temp];
    if (((unsigned char)s_hunkTotal + (unsigned char)s_hunkData - LOBYTE(hunk_high.temp)) & 0xF
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "!(((psize_int)buf) & 15)"))
    {
        __debugbreak();
    }
    if (endBuf != (char*)(buf & 0xFFFFF000))
        Z_VirtualCommit((void*)(buf & 0xFFFFF000), (int)&endBuf[-(buf & 0xFFFFF000)], 11);
    track_temp_high_alloc(size, hunk_high.temp + hunk_low.temp, hunk_high.permanent, name);
    return buf;
}

void Hunk_ClearTempMemoryHigh()
{
    char* beginBuf; // [esp+8h] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    beginBuf = (char*)((unsigned int)&s_hunkData[s_hunkTotal - hunk_high.temp] & 0xFFFFF000);
    hunk_high.temp = hunk_high.permanent;
    if ((char*)((unsigned int)&s_hunkData[s_hunkTotal - hunk_high.permanent] & 0xFFFFF000) != beginBuf)
        Z_VirtualDecommit(
            beginBuf,
            ((unsigned int)&s_hunkData[s_hunkTotal - hunk_high.permanent] & 0xFFFFF000) - (unsigned int)beginBuf);
    track_temp_high_clear(hunk_high.permanent);
}

char* Hunk_AllocLowAlign(int size, int alignment, const char* name, int type)
{
    int old_permanent; // [esp+0h] [ebp-14h]
    char* buf; // [esp+4h] [ebp-10h]
    char* beginBuf; // [esp+10h] [ebp-4h]
    int alignmenta; // [esp+20h] [ebp+Ch]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    if (!s_hunkData
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "s_hunkData"))
    {
        __debugbreak();
    }
    if (alignment & (alignment - 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "!(alignment & (alignment - 1))"))
    {
        __debugbreak();
    }
    if (alignment > 4096
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "alignment <= HUNK_MAX_ALIGNMENT"))
    {
        __debugbreak();
    }
    alignmenta = alignment - 1;
    Hunk_CheckTempMemoryClear();
    old_permanent = hunk_low.permanent;
    beginBuf = (char*)((unsigned int)&s_hunkData[hunk_low.permanent + 4095] & 0xFFFFF000);
    hunk_low.permanent = ~alignmenta & (alignmenta + hunk_low.permanent);
    buf = &s_hunkData[hunk_low.permanent];
    if (alignmenta & (unsigned int)&s_hunkData[hunk_low.permanent]
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "!(((psize_int)buf) & alignment)"))
    {
        __debugbreak();
    }
    hunk_low.permanent += size;
    hunk_low.temp = hunk_low.permanent;
    if (hunk_high.temp + hunk_low.permanent > s_hunkTotal)
    {
        track_PrintAllInfo();
        Com_Error(ERR_DROP, "Hunk_AllocLowAlign failed on %i bytes (total %i MB, low %i MB, high %i MB)", size, s_hunkTotal / 0x100000, hunk_low.temp / 0x100000, hunk_high.temp / 0x100000);
    }
    if ((char*)((unsigned int)&s_hunkData[hunk_low.permanent + 4095] & 0xFFFFF000) != beginBuf)
        Z_VirtualCommit(
            beginBuf,
            ((unsigned int)&s_hunkData[hunk_low.permanent + 4095] & 0xFFFFF000) - (unsigned int)beginBuf);
    track_hunk_allocLow(hunk_low.permanent - old_permanent, hunk_low.permanent, name, type);
    memset(buf, 0, size);
    return buf;
}

void* Hunk_AllocateTempMemory(int size, const char* name)
{
    hunkHeader_t* hdr; // [esp+0h] [ebp-18h]
    char* buf; // [esp+4h] [ebp-14h]
    int bufa; // [esp+4h] [ebp-14h]
    int prev_temp; // [esp+8h] [ebp-10h]
    char* beginBuf; // [esp+14h] [ebp-4h]
    int sizea; // [esp+20h] [ebp+8h]

    if (!Sys_IsMainThread()
        && !Sys_IsRenderThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread() || Sys_IsRenderThread()"))
    {
        __debugbreak();
    }
    if (!s_hunkData)
        return Z_Malloc(size, name, 11);
    if (!s_hunkData
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "s_hunkData"))
    {
        __debugbreak();
    }
    sizea = size + 16;
    prev_temp = hunk_low.temp;
    beginBuf = (char*)((unsigned int)&s_hunkData[hunk_low.temp + 4095] & 0xFFFFF000);
    hunk_low.temp = (hunk_low.temp + 15) & 0xFFFFFFF0;
    buf = &s_hunkData[hunk_low.temp];
    hunk_low.temp += sizea;
    if (hunk_high.temp + hunk_low.temp > s_hunkTotal)
    {
        track_PrintAllInfo();
        Com_Error(
            ERR_DROP,
            "Hunk_AllocateTempMemory: failed on %i bytes (total %i MB, low %i MB, high %i MB), needs %i more hunk bytes",
            sizea,
            s_hunkTotal / 0x100000,
            hunk_low.temp / 0x100000,
            hunk_high.temp / 0x100000,
            hunk_high.temp + hunk_low.temp - s_hunkTotal);
    }
    hdr = (hunkHeader_t*)buf;
    bufa = (int)(buf + 16);
    if (bufa & 0xF
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "!(((psize_int)buf) & 15)"))
    {
        __debugbreak();
    }
    if ((char*)((unsigned int)&s_hunkData[hunk_low.temp + 4095] & 0xFFFFF000) != beginBuf)
        Z_VirtualCommit(beginBuf, ((unsigned int)&s_hunkData[hunk_low.temp + 4095] & 0xFFFFF000) - (unsigned int)beginBuf);
    hdr->magic = 0x89537892;
    hdr->size = hunk_low.temp - prev_temp;
    track_temp_alloc(hdr->size, hunk_high.temp + hunk_low.temp, hunk_low.permanent, name);
    hdr->name = name;
    return (void*)bufa;
}

void Hunk_FreeTempMemory(void* buf)
{
    hunkHeader_t* hdr; // [esp+0h] [ebp-10h]
    char* endBuf; // [esp+4h] [ebp-Ch]
    char* beginBuf; // [esp+Ch] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Sys_IsRenderThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread() || Sys_IsRenderThread()"))
    {
        __debugbreak();
    }
    if (s_hunkData)
    {
        if (!s_hunkData
            && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "s_hunkData"))
        {
            __debugbreak();
        }
        if (!buf && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "buf"))
            __debugbreak();
        hdr = (hunkHeader_t*)((char*)buf - 16);
        if (*((unsigned int*)buf - 4) != 0x89537892)
            Com_Error(ERR_FATAL, "Hunk_FreeTempMemory: bad magic");
        hdr->magic = 0x89537893;
        if (hdr != (hunkHeader_t*)&s_hunkData[(hunk_low.temp - hdr->size + 15) & 0xFFFFFFF0]
            && !Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "%s",
                "hdr == (void *)( s_hunkData + ((hunk_low.temp - hdr->size + 15) & ~15) )"))
        {
            __debugbreak();
        }
        endBuf = (char*)((unsigned int)&s_hunkData[hunk_low.temp + 4095] & 0xFFFFF000);
        hunk_low.temp -= hdr->size;
        track_temp_free(hdr->size, hunk_low.permanent, hdr->name);
        beginBuf = (char*)((unsigned int)&s_hunkData[hunk_low.temp + 4095] & 0xFFFFF000);
        if (endBuf != beginBuf)
            Z_VirtualDecommit(beginBuf, endBuf - beginBuf);
    }
    else
    {
        Z_Free(buf, 11);
    }
}

void Hunk_ClearTempMemory()
{
    char* endBuf; // [esp+0h] [ebp-Ch]
    char* beginBuf; // [esp+8h] [ebp-4h]

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "%s",
            "Sys_IsMainThread()"))
    {
        __debugbreak();
    }
    if (!s_hunkData
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "s_hunkData"))
    {
        __debugbreak();
    }
    endBuf = (char*)((unsigned int)&s_hunkData[hunk_low.temp + 4095] & 0xFFFFF000);
    hunk_low.temp = hunk_low.permanent;
    beginBuf = (char*)((unsigned int)&s_hunkData[hunk_low.permanent + 4095] & 0xFFFFF000);
    if (endBuf != beginBuf)
        Z_VirtualDecommit(beginBuf, endBuf - beginBuf);
    track_temp_clear(hunk_low.permanent);
}

void* Z_TryVirtualAllocInternal(int size, const char* name)
{
    void* ptr; // [esp+0h] [ebp-4h]

    ptr = Z_VirtualReserve(size);
    if (ptr)
    {
        if (Z_TryVirtualCommitInternal(ptr, size))
            return ptr;
        Z_VirtualFree(ptr);
    }
    return 0;
}

char* Z_TryVirtualAlloc(int size, const char* name, int type)
{
    char* buf; // [esp+0h] [ebp-4h]

    buf = (char*)Z_TryVirtualAllocInternal(size, name);
    if (buf)
        track_z_commit((size + 4095) & 0xFFFFF000, type);
    return buf;
}

char* Z_VirtualAlloc(int size, const char* name, int type)
{
    char* buf; // [esp+0h] [ebp-4h]

    buf = Z_TryVirtualAlloc(size, name, type);
    if (!buf)
        Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
    return buf;
}

void ReplaceString(const char** str, const char* in)
{
    char* newStr; // [esp+0h] [ebp-4h]

    if (!str && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "str"))
        __debugbreak();
    if (!in && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "in"))
        __debugbreak();
    newStr = CopyString(in);
    if (*str)
        FreeString(*str);
    *str = newStr;
}

void Com_InitHunkMemory()
{
    char* v0; // eax

    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(Sys_IsMainThread())",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (s_hunkData
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(!s_hunkData)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (FS_LoadStack())
        Com_Error(ERR_FATAL, "Hunk initialization failed. File system load stack not zero");
    if (!Dvar_GetBool(useFastFile))
        s_hunkTotal = 0xA000000;
    if (Dvar_GetBool(useFastFile))
    {
        s_hunkTotal = 0x300000;
    }
    v0 = (char*)Z_VirtualReserve(s_hunkTotal);
    s_hunkData = v0;
    if (!v0)
        Sys_OutOfMemErrorInternal(__FILE__, __LINE__);
    s_origHunkData = v0;
    track_set_hunk_size(s_hunkTotal);
    track_physical_alloc(s_hunkTotal, "hunk", 1, 0);
    Hunk_Clear();
    Cmd_AddCommandInternal("meminfo", Com_AllMemInfo_f, &Com_AllMemInfo_f_VAR);
}

char* Hunk_Alloc(int size, const char* name, int type)
{
    if (!Sys_IsMainThread()
        && !Sys_IsRenderThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(Sys_IsMainThread() || Sys_IsRenderThread())",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    return Hunk_AllocAlign(size, 32, name, type);
}

char* Hunk_AllocLow(int size, const char* name, int type)
{
    if (!Sys_IsMainThread()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(Sys_IsMainThread())",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    return Hunk_AllocLowAlign(size, 32, name, type);
}

void Com_Memset(void* dest, const int val, int count)
{
    memset((char*)dest, val, count);
}
