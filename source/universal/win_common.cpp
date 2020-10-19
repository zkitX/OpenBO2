#include "win_common.h"
#include "assertive.h"

#include <universal/com_memory.h>

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

void Sys_ListFilteredFiles(HunkUser* user, const char* baseDir, const char* subDirs, const char* filter, char** list, int* numFiles)
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
    char** result; // eax
    char* v6; // eax
    _finddata64i32_t pfd; // [esp+0h] [ebp-10248h]
    int v8; // [esp+12Ch] [ebp-1011Ch]
    char** v9; // [esp+130h] [ebp-10118h]
    int hFile; // [esp+134h] [ebp-10114h]
    char* list[16384]; // [esp+138h] [ebp-10110h]
    int numfilesa; // [esp+10138h] [ebp-110h]
    HunkUser* user; // [esp+1013Ch] [ebp-10Ch]
    char dest; // [esp+10140h] [ebp-108h]
    int i; // [esp+10244h] [ebp-4h]

    if (filter)
    {
        user = Hunk_UserCreate(0x20000, 0, 0, 0, "Sys_ListFiles", 3);
        numfilesa = 0;
        Sys_ListFilteredFiles(user, directory, &scratch, filter, list, &numfilesa);
        list[numfilesa] = 0;
        *numfiles = numfilesa;
        if (numfilesa)
        {
            v9 = (char**)Hunk_UserAlloc(user, 4 * numfilesa + 8, 4, 0);
            *v9 = (char*)user;
            ++v9;
            for (i = 0; i < numfilesa; ++i)
                v9[i] = list[i];
            v9[i] = 0;
            result = v9;
        }
        else
        {
            Hunk_UserDestroy(user);
            result = 0;
        }
    }
    else
    {
        if (!extension)
            extension = &scratch;
        if (*extension != 47 || extension[1])
        {
            v8 = 16;
        }
        else
        {
            extension = &scratch;
            v8 = 0;
        }
        if (*extension)
            Com_sprintf(&dest, 256, "%s\\*.%s", directory, extension);
        else
            Com_sprintf(&dest, 256, "%s\\*", directory);
        numfilesa = 0;
        hFile = _findfirst64i32(&dest, &pfd);
        if (hFile == -1)
        {
            *numfiles = 0;
            result = 0;
        }
        else
        {
            user = Hunk_UserCreate(0x20000, 0, 0, 0, "Sys_ListFiles", 3);
            do
            {
                if ((!wantsubs && v8 != (pfd.attrib & 0x10) || wantsubs && pfd.attrib & 0x10)
                    && (!(pfd.attrib & 0x10) || I_stricmp(pfd.name, ".")
                        && I_stricmp(pfd.name, "..")
                        && I_stricmp(pfd.name, "CVS"))
                    && (!*extension || HasFileExtension(pfd.name, extension)))
                {
                    v6 = Hunk_CopyString(user, pfd.name);
                    list[numfilesa++] = v6;
                    if (numfilesa == 0x3FFF)
                        break;
                }
            } while (_findnext64i32(hFile, &pfd) != -1);
            list[numfilesa] = 0;
            _findclose(hFile);
            *numfiles = numfilesa;
            if (numfilesa)
            {
                v9 = (char**)Hunk_UserAlloc(user, 4 * numfilesa + 8, 4, 0);
                *v9 = (char*)user;
                ++v9;
                for (i = 0; i < numfilesa; ++i)
                    v9[i] = list[i];
                v9[i] = 0;
                result = v9;
            }
            else
            {
                Hunk_UserDestroy(user);
                result = 0;
            }
        }
    }
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
