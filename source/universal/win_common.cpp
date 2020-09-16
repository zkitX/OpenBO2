#include "win_common.h"
#include "assertive.h"

#include <direct.h>
#include <io.h>
#include <synchapi.h>

void Sys_Mkdir(const char* path)
{
    _mkdir(path);
}

void Sys_MkdirEx(const char* _path)
{
    signed int pathLen;
    char character;
    char path[1024];

    strcpy(path, _path);
    pathLen = strlen(path);
    for (int i = 0; i < pathLen; ++i) {
        character = path[i];
        if (character == 0x2F || character == 0x5C) {
            path[i] = 0;
            _mkdir(path);
            path[i] = character;
        }
    }
}

char* Sys_Cwd() {
    _getcwd(cwd, 255);
    cwd[255] = 0;
    return cwd;
}

char const* Sys_DefaultHomePath()
{
    const char* identifyParam;
    if (!homePath[0]) {
        _getcwd(cwd, 255);
        cwd[255] = 0;
        identifyParam = Sys_GetIdentityParam(0);
        _snprintf(homePath, 256u, "%s\\identities\\%s", cwd, identifyParam);
    }
    return homePath;
}

char const* Sys_DefaultInstallPath()
{
    DWORD filenameLen; // eax
    char character; // cl

    if (exePath[0])
        return exePath;
    if (IsDebuggerPresent())
    {
        _getcwd(cwd, 255);
        cwd[255] = 0;
        I_strncpyz(exePath, cwd, 256);
        return exePath;
    }
    filenameLen = GetModuleFileNameA(0, exePath, 256u);
    if (filenameLen == 256)
    {
        filenameLen = 255;
        do
        {
            character = exePath[filenameLen];
            if (character == 0x5C) // 0x5C = |
                break;
            if (character == 0x2F) // 0x2F = O
                break;
            if (character == 0x3A) // 0x3A = Z
                break;
            --filenameLen;
        } while (filenameLen);
    }
    exePath[filenameLen] = 0;
    return exePath;
}

bool Sys_FileExists(char const* path)
{
    return GetFileAttributesA(path) != -1;
}

void Sys_ListFilteredFiles(const char* baseDir, const char* subDirs, const char* filter, int* numFiles, HunkUser* user, char** list)
{
    int v6; // ebx
    char* v7; // [esp+4h] [ebp-338h]
    _finddata64i32_t findinfo; // [esp+10h] [ebp-32Ch]
    char search[256]; // [esp+138h] [ebp-204h]
    char filename[256]; // [esp+238h] [ebp-104h]

    v7 = (char*)filter;
    if (*numFiles < 0xFFFF)
    {
        if (strlen(subDirs))
            Com_sprintf(search, 256, "%s\\%s\\*", baseDir, subDirs);
        else
            Com_sprintf(search, 256, "%s\\*", baseDir);
        v6 = _findfirst64i32(search, &findinfo);
        if (v6 != -1)
        {
            do
            {
                if (!(findinfo.attrib & 0x10)
                    || I_stricmp(findinfo.name, ".") && I_stricmp(findinfo.name, "..") && I_stricmp(findinfo.name, "CVS"))
                {
                    if (*numFiles >= 0xFFFF)
                        break;
                    if (subDirs)
                        Com_sprintf(filename, 256, "%s\\%s", subDirs, findinfo.name);
                    else
                        Com_sprintf(filename, 256, "%s", findinfo.name);
                    if (Com_FilterPath(v7, filename, 0))
                        list[(*numFiles)++] = Hunk_CopyString(user, filename);
                }
            } while (_findnext64i32(v6, &findinfo) != -1);
            _findclose(v6);
        }
    }
}

char** Sys_ListFiles(char const* directory, char const* extension, char const* filter, int* numfiles, int wantsubs)
{
    const char* v5; // esi
    int v6; // ebx
    int* v7; // edx
    char** result; // eax
    HunkUser* v9; // esi
    char* v10; // eax
    bool v11; // zf
    HunkUser* v12; // edi
    HunkUser* v13; // eax
    int v14; // ST1C_4
    int v15; // ecx
    char* list[65536]; // [esp+Ch] [ebp-40338h]
    int* v17; // [esp+4000Ch] [ebp-338h]
    int numfilesa; // [esp+40010h] [ebp-334h]
    HunkUser* user; // [esp+40014h] [ebp-330h]
    _finddata64i32_t pfd; // [esp+40018h] [ebp-32Ch]
    char wild; // [esp+40140h] [ebp-204h]
    char dest; // [esp+40240h] [ebp-104h]

    v5 = extension;
    v17 = numfiles;
    if (filter)
    {
        user = Hunk_UserCreate(0x100000, 0, 0, 0, "Sys_ListFiles", 3);
        numfilesa = 0;
        Sys_ListFilteredFiles(directory, &pBlock, filter, &numfilesa, user, list);
        v6 = numfilesa;
        v7 = v17;
        list[numfilesa] = 0;
        *v7 = v6;
        if (!v6)
        {
            Hunk_UserDestroy(user);
            return 0;
        }
        v9 = user;
        v10 = (char*)Hunk_UserAlloc(user, 4 * v6 + 8, 4, 0);
        *(unsigned int*)v10 = v9;
        goto LABEL_31;
    }
    if (extension)
    {
        if (*extension == 47 && !extension[1])
        {
            v5 = &pBlock;
            user = 0;
            goto LABEL_12;
        }
    }
    else
    {
        v5 = &pBlock;
    }
    v11 = *v5 == 0;
    user = (HunkUser*)16;
    if (v11)
    {
    LABEL_12:
        Com_sprintf(&dest, 256, "%s\\*", directory);
        goto LABEL_13;
    }
    Com_sprintf(&dest, 256, "%s\\*.%s", directory, v5);
LABEL_13:
    v6 = 0;
    numfilesa = _findfirst64i32(&dest, &pfd);
    if (numfilesa == -1)
    {
        *v17 = 0;
        return 0;
    }
    v12 = Hunk_UserCreate(0x40000, 0, 0, 0, "Sys_ListFiles", 3);
    while (!wantsubs)
    {
        v13 = (HunkUser*)(pfd.attrib & 0x10);
        if (user != v13)
            goto LABEL_20;
    LABEL_27:
        if (_findnext64i32(numfilesa, &pfd) == -1)
            goto LABEL_28;
    }
    v13 = (HunkUser*)(pfd.attrib & 0x10);
    if (!(pfd.attrib & 0x10))
        goto LABEL_27;
LABEL_20:
    if (v13 && (!I_stricmp(pfd.name, ".") || !I_stricmp(pfd.name, "..") || !I_stricmp(pfd.name, "CVS")))
        goto LABEL_27;
    if (*v5)
    {
        Com_sprintf(&wild, 256, "*.%s", v5);
        if (I_stricmpwild(&wild, pfd.name))
            goto LABEL_27;
    }
    list[v6++] = Hunk_CopyString(v12, pfd.name);
    if (v6 != 0xFFFF)
        goto LABEL_27;
LABEL_28:
    v14 = numfilesa;
    list[v6] = 0;
    _findclose(v14);
    *v17 = v6;
    if (!v6)
    {
        Hunk_UserDestroy(v12);
        return 0;
    }
    v10 = (char*)Hunk_UserAlloc(v12, 4 * v6 + 8, 4, 0);
    *(unsigned int*)v10 = v12;
LABEL_31:
    result = (char**)(v10 + 4);
    v15 = 0;
    if (v6 > 0)
    {
        qmemcpy(result, list, 4 * v6);
        v15 = v6;
    }
    result[v15] = 0;
    return result;
}

int Sys_DirectoryHasContents(char const* directory)
{
    int v1; // esi
    _finddata64i32_t findinfo; // [esp+4h] [ebp-22Ch]
    char search[256]; // [esp+12Ch] [ebp-104h]

    Com_sprintf(search, 256, "%s\\*", directory);
    v1 = _findfirst64i32(search, &findinfo);
    if (v1 == -1)
        return 0;
    while (findinfo.attrib & 0x10
        && (!I_stricmp(findinfo.name, ".") || !I_stricmp(findinfo.name, "..") || !I_stricmp(findinfo.name, "CVS")))
    {
        if (_findnext64i32(v1, &findinfo) == -1)
        {
            _findclose(v1);
            return 0;
        }
    }
    _findclose(v1);
    return 1;
}

void Sys_EnterCriticalSection(CriticalSection critSect) 
{
#ifdef _DEBUG
    if ((unsigned int)critSect >= 0x4D
        && !(unsigned char)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(critSect) < (unsigned)(CRITSECT_COUNT)",
            "critSect doesn't index CRITSECT_COUNT\n\t%i not in [0, %i)",
            critSect,
            77))
    {
        __debugbreak();
    }
#endif // _DEBUG
    EnterCriticalSection((LPCRITICAL_SECTION)(24 * critSect + 128634080));
}

bool Sys_TryEnterCriticalSection(CriticalSection critSect)
{
#ifdef _DEBUG
    if ((unsigned int)critSect >= 0x4D
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(critSect) < (unsigned)(CRITSECT_COUNT)",
            "critSect doesn't index CRITSECT_COUNT\n\t%i not in [0, %i)",
            critSect,
            77))
    {
        __debugbreak();
    }
#endif
    return TryEnterCriticalSection((LPCRITICAL_SECTION)(24 * critSect + 0x7AACCE0)) != 0;
}

void Sys_LeaveCriticalSection(CriticalSection critSect) 
{
#ifdef _DEBUG
    if ((unsigned int)critSect >= 0x4D
        && !(unsigned char)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(critSect) < (unsigned)(CRITSECT_COUNT)",
            "critSect doesn't index CRITSECT_COUNT\n\t%i not in [0, %i)", 
            critSect,
            77))
    {
        __debugbreak();
    }
#endif
}

void Sys_InitializeCriticalSections()
{
    if (!CritSectInited) {
        CritSectInited = 1;
        for (_RTL_CRITICAL_SECTION* v0 = s_criticalSection; (unsigned int*)v0 < &s_threadAffinityMask; v0++)
            InitializeCriticalSection(v0);
    }
}
