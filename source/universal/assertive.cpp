#include <immintrin.h>

#include "assertive.h"
#include "blackbox.h"
#include "win_common.h"
#include "../qcommon/common.h"
#include "../win32/miniDumper.h"

const char aExe[5] = ".exe";
const char aDll[5] = ".dll";

int assertive::Assert_BuildAssertMessageWithStack(const char* extra, int line, char* message, const char* expr, const char* filename, int type, int messageLen)
{
    void* v7; // esi
    int v8; // eax
    char* v9; // esi
    const char* v10; // ST28_4
    const char* v11; // eax
    int v13; // [esp+0h] [ebp-20h]
    const char* v14; // [esp+8h] [ebp-18h]
    void* v15; // [esp+Ch] [ebp-14h]
    char unknown[10]; // [esp+10h] [ebp-10h]

    v7 = (void*)filename;
    v14 = extra;
    v13 = line;
    v15 = (void*)expr;
    strcpy(unknown, "<unknown>");
    if (!filename)
        v7 = unknown;
    if (!expr)
        v15 = unknown;
    if (!GetModuleFileNameA(0, g_module, 0x104u))
    {
        _mm_storel_epi64((__m128i*)g_module, _mm_loadl_epi64((const __m128i*)"<unknown application>"));
        _mm_storel_epi64((__m128i*) & g_module[8], _mm_loadl_epi64((const __m128i*)" application>"));
        *(_DWORD*)&g_module[16] = *(_DWORD*)"tion>";
        *(_WORD*)&g_module[20] = *(_WORD*)">";
    }
    v8 = _snprintf(message, messageLen, "%s\n%s\n%s\n%s:%d\n", v15, v14, g_module, v7, v13);
    v9 = &message[v8 + StackTrace_Generate(messageLen - v8, &message[v8])];
    v10 = Com_GetBuildVersion();
    v11 = Com_GetBuildName();
    return v9 - message + _snprintf(v9, &message[messageLen] - v9, "%s %s\n", v11, v10);
}

bool assertive::Assert_MyHandler(const char* filename, int line, int type, const char* expr, const char* fmt, ...)
{
    const char* v5; // eax
    int v6; // edi
    HWND v7; // eax
    char v8; // bl
    const char* lpCaption; // [esp+0h] [ebp-4h]
    va_list ap; // [esp+20h] [ebp+1Ch]

    va_start(ap, fmt);
    Sys_EnterCriticalSection(CRITSECT_ASSERT);
    _vsnprintf(message, 0x400u, fmt, ap);
    message[1023] = 0;
    StackTrace_Walk(1, 0);
    if (!g_inStackTrace)
    {
        g_inStackTrace = 1;
        LoadMapFilesForDir((const char*)&pBlock);
        g_inStackTrace = 0;
    }
    if (isHandlingAssert)
    {
        v5 = va("Assert Expression:\n    %s\nFile:    %s\nLine:    %d\n\n", message, filename, line);
        Com_Printf(10, v5);
        CopyMessageToClipboard(assertMessage);
        v6 = lastAssertType;
        if (AssertCallback)
            AssertCallback(assertMessage);
        if (v6)
        {
            if (v6 == 1)
                lpCaption = "SANITY CHECK FAILURE... (this text is on the clipboard)";
            else
                lpCaption = "INTERNAL ERROR";
        }
        else
        {
            lpCaption = "ASSERTION FAILURE... (this text is on the clipboard)";
        }
        ShowCursor(1);
        ShowCursor(1);
        v7 = GetActiveWindow();
        MessageBoxA(v7, assertMessage, lpCaption, 0x12011u);
        Assert_BuildAssertMessageWithStack(message, line, assertMessage, expr, filename, type, 4096);
        blackbox::BB_Alert(filename, line, "assert", message);
        if (isHandlingAssert == 1)
        {
            isHandlingAssert = 2;
            Com_Printf(10, "ASSERTBEGIN - ( Recursive assert )---------------------------------------------\n");
            Com_Printf(10, "%s", assertMessage);
            Com_Printf(10, "ASSERTEND - ( Recursive assert ) ----------------------------------------------\n\n");
        }
        // LiveSteam_Shutdown();
        exit(-1);
    }
    lastAssertType = type;
    isHandlingAssert = 1;
    FixWindowsDesktop();
    Assert_BuildAssertMessageWithStack(message, line, assertMessage, expr, filename, type, 4096);
    blackbox::BB_Alert(filename, line, "assert", message);
    Com_Printf(10, "ASSERTBEGIN -------------------------------------------------------------------\n");
    Com_Printf(10, "%s", assertMessage);
    Com_Printf(10, "ASSERTEND ---------------------------------------------------------------------\n");
    OutputDebugStringA(assertMessage);
    if (!Dvar_IsSystemActive())
    {
        if (!shouldQuitOnError)
            goto LABEL_19;
    LABEL_22:
        //LiveSteam_Shutdown();
        ExitProcess(0xFFFFFFFF);
    }
    if (Dvar_GetBool(quit_on_error) || Dvar_GetInt(r_vc_compile) == 2)
    {
        shouldQuitOnError = 1;
        goto LABEL_22;
    }
    shouldQuitOnError = 0;
LABEL_19:
    CopyMessageToClipboard(assertMessage);
    v8 = AssertNotify(type, (AssertOccurance)0);
    isHandlingAssert = 0;
    Sys_LeaveCriticalSection(CRITSECT_ASSERT);
    return v8 == 0;
}

HMODULE assertive::GetModuleBase(int a1, const char* name)
{
    unsigned int v2; // esi
    int v3; // ecx
    char v4; // al
    char* v5; // esi
    HMODULE result; // eax
    char moduleName[260]; // [esp+8h] [ebp-108h]

    v2 = strlen(name);
    v3 = v2 - 1;
    if ((signed int)(v2 - 1) >= 0)
    {
        while (1)
        {
            v4 = name[v3];
            if (v4 == 46 || v4 == 47 || v4 == 92)
                break;
            if (--v3 < 0)
                goto LABEL_10;
        }
        if (v3 >= 0 && name[v3] == 46)
            v2 = v3;
    }
LABEL_10:
    memcpy(moduleName, (char*)name, v2);
    v5 = &moduleName[v2];
    *(_DWORD*)v5 = *(_DWORD*)aExe;
    v5[4] = 0;
    result = GetModuleHandleA(moduleName);
    if (!result)
    {
        *(_DWORD*)v5 = *(_DWORD*)aDll;
        v5[4] = 0;
        result = GetModuleHandleA(moduleName);
    }
    return result;
}

void assertive::LoadMapFilesForDir(const char* dir)
{
    char* v1; // eax
    char* v2; // eax
    int v3; // ecx
    HMODULE v4; // edi
    char* v5; // eax
    _iobuf* v6; // esi
    int v7; // eax
    char v8; // cl
    HANDLE hFindFile; // [esp+0h] [ebp-1148h]
    _WIN32_FIND_DATAA FindFileData; // [esp+4h] [ebp-1144h]
    char string; // [esp+144h] [ebp-1004h]
    char v12[4]; // [esp+940h] [ebp-808h]
    char file[2048]; // [esp+944h] [ebp-804h]

    if (*dir)
    {
        v1 = Sys_DefaultInstallPath();
        sprintf(&string, "%s\\%s\\*.map", v1, dir);
    }
    else
    {
        v2 = Sys_DefaultInstallPath();
        sprintf(&string, "%s\\*.map", v2);
    }
    hFindFile = FindFirstFileA(&string, &FindFileData);
    if (hFindFile != (HANDLE)-1)
    {
        do
        {
            v4 = GetModuleBase(v3, FindFileData.cFileName);
            if (v4)
            {
                v5 = Sys_DefaultInstallPath();
                sprintf(file, "%s\\%s", v5, FindFileData.cFileName);
                v6 = fopen(file, "rb");
                if (v6)
                {
                    v7 = 0;
                    do
                    {
                        v8 = FindFileData.cFileName[v7];
                        file[v7++] = v8;
                    } while (v8);
                    v12[strlen(file)] = 0;
                    ParseMapFile(v6, (bool)v6, (unsigned int)v4, file);
                    fclose(v6);
                }
            }
        } while (FindNextFileA(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }
}

void assertive::ParseError(const char* msg)
{
    MessageBoxA(GetActiveWindow(), msg, ".map parse error", 0x10u);
}

bool assertive::ParseMapFile(_iobuf* fp, bool a2, unsigned int baseAddress, const char* mapName) {
    _iobuf* v4; // edi
    unsigned int v5; // esi
    char result; // al
    int v7; // edi
    AddressInfo_s* v8; // esi
    _iobuf* v9; // esi
    int v10; // eax
    int v11; // eax
    unsigned int v12; // edi
    int v13; // ebx
    char* v14; // esi
    unsigned int v15; // eax
    char v16; // cl
    char* v17; // eax
    _BYTE* v18; // eax
    int v19; // eax
    char* v20; // eax
    _iobuf* v21; // esi
    int v22; // eax
    unsigned int v23; // edi
    int v24; // ebx
    char* v25; // esi
    unsigned int v26; // eax
    char v27; // cl
    char* v28; // eax
    _BYTE* v29; // eax
    int v30; // eax
    char* v31; // eax
    _iobuf* v32; // ebx
    char* v33; // eax
    char* v34; // edi
    int v35; // eax
    int v36; // esi
    int v37; // eax
    int v38; // ecx
    int v39; // eax
    int v40; // ecx
    unsigned int v41; // edx
    unsigned int v42; // ebx
    unsigned int* v43; // esi
    unsigned int v44; // eax
    unsigned int v45; // edx
    int v46; // eax
    char* v47; // eax
    HWND v48; // eax
    const char* v49; // [esp-10h] [ebp-C74h]
    unsigned int loadAddress; // [esp+8h] [ebp-C5Ch]
    int readCount; // [esp+Ch] [ebp-C58h]
    int j; // [esp+10h] [ebp-C54h]
    int v53; // [esp+14h] [ebp-C50h]
    unsigned int offset; // [esp+18h] [ebp-C4Ch]
    unsigned int group; // [esp+1Ch] [ebp-C48h]
    unsigned int v56; // [esp+20h] [ebp-C44h]
    unsigned int address; // [esp+24h] [ebp-C40h]
    unsigned int baseEndAddress; // [esp+28h] [ebp-C3Ch]
    _iobuf* v59; // [esp+2Ch] [ebp-C38h]
    unsigned int lineGroup[4]; // [esp+30h] [ebp-C34h]
    unsigned int lineNumber[4]; // [esp+40h] [ebp-C24h]
    unsigned int lineOffset[4]; // [esp+50h] [ebp-C14h]
    char function[2048]; // [esp+60h] [ebp-C04h]
    char filenameBuffer[1024]; // [esp+860h] [ebp-404h]

    v4 = fp;
    v59 = fp;
    lineBufferEndPos = 0;
    lineBufferStartPos = 0;
    do
    {
        if (!ReadLine(v4))
            return 0;
    } while (sscanf(lineBuffer, " Preferred load address is %x\r\n", &loadAddress) != 1);
    if (!SkipLines(2, v4))
        return 0;
    v5 = 0;
    if (ReadLine(v4))
    {
        while (lineBuffer[0])
        {
            if (sscanf(lineBuffer, "%x:%x %xH %s %s", &group, &offset, &address, function, filenameBuffer) != 5)
            {
                v49 = "Unknown line format in the segments section";
            LABEL_112:
                v48 = GetActiveWindow();
                MessageBoxA(v48, v49, ".map parse error", 0x10u);
                return 0;
            }
            if (group == 1 && v5 < address + offset + baseAddress + 4096)
                v5 = address + offset + baseAddress + 4096;
            result = ReadLine(v4);
            if (!result)
                return result;
        }
        v7 = 0;
        baseEndAddress = v5;
        if (g_assertAddressCount > 0)
        {
            v8 = g_assertAddress;
            do
            {
                if (v8->address >= baseAddress && v8->address < baseEndAddress)
                    I_strncpyz(v8->moduleName, mapName, 64);
                ++v7;
                ++v8;
            } while (v7 < g_assertAddressCount);
        }
        v9 = v59;
        while (ReadLine(v9))
        {
            strstr(lineBuffer, "Publics by Value");
            if (v10)
            {
                if (SkipLines(1, v9) && ReadLine(v9))
                {
                    while (lineBuffer[0])
                    {
                        if (sscanf(lineBuffer, "%x:%x %s %x", &group, &offset, function, &address) != 4)
                        {
                            v49 = "Unknown line format in the public symbols section";
                            goto LABEL_112;
                        }
                        strrchr(lineBuffer, 32);
                        if (!v11 || sscanf((const char*)(v11 + 1), "%s", filenameBuffer) != 1)
                        {
                            v49 = "Couldn't parse file name in the public symbols section";
                            goto LABEL_112;
                        }
                        v12 = address;
                        v13 = 0;
                        if (g_assertAddressCount > 0)
                        {
                            v14 = g_assertAddress[0].bestFunction;
                            do
                            {
                                v15 = *((_DWORD*)v14 - 17);
                                if (v15 >= baseAddress && v15 < baseEndAddress && v12 <= v15 && (!*v14 || *((_DWORD*)v14 + 32) < v12))
                                {
                                    v16 = function[0];
                                    *((_DWORD*)v14 + 32) = v12;
                                    v17 = function;
                                    if (v16 == 95 || v16 == 63)
                                        v17 = &function[1];
                                    I_strncpyz(v14, v17, 64);
                                    strchr(v14, 64);
                                    if (v18)
                                        *v18 = 0;
                                    strrchr(filenameBuffer, 92);
                                    if (v19)
                                        v20 = (char*)(v19 + 1);
                                    else
                                        v20 = filenameBuffer;
                                    I_strncpyz(v14 + 64, v20, 64);
                                }
                                ++v13;
                                v14 += 272;
                            } while (v13 < g_assertAddressCount);
                            v9 = v59;
                        }
                        result = ReadLine(v9);
                        if (!result)
                            return result;
                    }
                    if (SkipLines(2, v9) && ReadLine(v9))
                    {
                        if (!strcmp(lineBuffer, " Static symbols\r"))
                        {
                            v21 = v59;
                            if (!SkipLines(1, v59))
                                return 0;
                            if (ReadLine(v21))
                            {
                                while (lineBuffer[0])
                                {
                                    if (sscanf(lineBuffer, "%x:%x %s %x", &group, &offset, function, &address) != 4)
                                    {
                                        ParseError("Unknown line format in the static symbols section");
                                        return 0;
                                    }
                                    strrchr(lineBuffer, 32);
                                    if (!v22 || sscanf((const char*)(v22 + 1), "%s", filenameBuffer) != 1)
                                    {
                                        ParseError("Couldn't parse file name in the static symbols section");
                                        return 0;
                                    }
                                    v23 = address;
                                    v24 = 0;
                                    if (g_assertAddressCount > 0)
                                    {
                                        v25 = g_assertAddress[0].bestFunction;
                                        do
                                        {
                                            v26 = *((_DWORD*)v25 - 17);
                                            if (v26 >= baseAddress
                                                && v26 < baseEndAddress
                                                && v23 <= v26
                                                && (!*v25 || *((_DWORD*)v25 + 32) < v23))
                                            {
                                                v27 = function[0];
                                                *((_DWORD*)v25 + 32) = v23;
                                                v28 = function;
                                                if (v27 == 95 || v27 == 63)
                                                    v28 = &function[1];
                                                I_strncpyz(v25, v28, 64);
                                                strchr(v25, 64);
                                                if (v29)
                                                    *v29 = 0;
                                                strrchr(filenameBuffer, 92);
                                                if (v30)
                                                    v31 = (char*)(v30 + 1);
                                                else
                                                    v31 = filenameBuffer;
                                                I_strncpyz(v25 + 64, v31, 64);
                                            }
                                            ++v24;
                                            v25 += 272;
                                        } while (v24 < g_assertAddressCount);
                                    }
                                    if (!ReadLine(v59))
                                        break;
                                }
                            }
                        }
                        if (!ReadLine(v59))
                            return 1;
                        while (1)
                        {
                            v32 = v59;
                            if (strncmp(lineBuffer, "Line numbers for ", 0x11u))
                            {
                                v49 = "Expected line number section";
                                goto LABEL_112;
                            }
                            strchr(lineBuffer, 40);
                            v34 = v33;
                            if (!v33)
                            {
                                v49 = "Couldn't find '(' for the name of the source file in line number section";
                                goto LABEL_112;
                            }
                            strchr(v33, 41);
                            if (!v35)
                                break;
                            v36 = v35 - (_DWORD)v34;
                            strncpy(filenameBuffer, v34 + 1, v35 - (_DWORD)v34 - 1);
                            function[v36 + 2047] = 0;
                            if (!SkipLines(1, v32))
                                return 0;
                            if (ReadLine(v32))
                            {
                                while (lineBuffer[0])
                                {
                                    v37 = sscanf(
                                        lineBuffer,
                                        "%i %x:%x %i %x:%x %i %x:%x %i %x:%x\r\n",
                                        lineNumber,
                                        lineGroup,
                                        lineOffset,
                                        &lineNumber[1],
                                        &lineGroup[1],
                                        &lineOffset[1],
                                        &lineNumber[2],
                                        &lineGroup[2],
                                        &lineOffset[2],
                                        &lineNumber[3],
                                        &lineGroup[3],
                                        &lineOffset[3]);
                                    v38 = v37;
                                    v39 = v37 / 3;
                                    readCount = v38;
                                    if (v38 != 3 * v39 || v39 <= 0)
                                    {
                                        v49 = "unknown line format in the line number section";
                                        goto LABEL_112;
                                    }
                                    if (v38 > 0)
                                    {
                                        v40 = g_assertAddressCount;
                                        v41 = baseAddress;
                                        v53 = 0;
                                        v56 = 0;
                                        do
                                        {
                                            v42 = lineOffset[v56 / 4] + v41 + 4096;
                                            j = 0;
                                            if (v40 > 0)
                                            {
                                                v43 = &g_assertAddress[0].bestLineAddress;
                                                do
                                                {
                                                    v44 = *(v43 - 66);
                                                    if (v44 >= v41 && v44 < baseEndAddress && v42 <= v44 && (!*((_BYTE*)v43 - 64) || *v43 < v42))
                                                    {
                                                        v45 = lineNumber[v56 / 4];
                                                        *v43 = v42;
                                                        v43[1] = v45;
                                                        strrchr(filenameBuffer, 92);
                                                        if (v46)
                                                            v47 = (char*)(v46 + 1);
                                                        else
                                                            v47 = filenameBuffer;
                                                        I_strncpyz((char*)v43 - 64, v47, 64);
                                                        v40 = g_assertAddressCount;
                                                        v41 = baseAddress;
                                                    }
                                                    v43 += 68;
                                                    ++j;
                                                } while (j < v40);
                                            }
                                            v56 += 4;
                                            v53 += 3;
                                        } while (v53 < readCount);
                                    }
                                    if (!ReadLine(v59))
                                        break;
                                }
                            }
                            if (!ReadLine(v59))
                                return 1;
                        }
                        v49 = "Couldn't find ')' for the name of the source file in line number section";
                        goto LABEL_112;
                    }
                }
                return 0;
            }
        }
    }
    return 0;
}

int assertive::StackTrace_Generate(int len, char* msg)
{
    char* v3; // ecx
    char* v4; // ebx
    char* v5; // esi
    char* v6; // edi
    char* v7; // esi
    int v8; // eax
    int j; // [esp+0h] [ebp-4h]

    if (g_inStackTrace)
        return 0;
    v3 = msg;
    v4 = &msg[len];
    g_inStackTrace = 1;
    v5 = msg;
    j = 0;
    if (g_assertAddressCount > 0)
    {
        v6 = g_assertAddress[0].moduleName;
        do
        {
            if (*v6)
            {
                v7 = &v5[_snprintf(v5, v4 - v5, "%s:    ", v6)];
                if (v6[196])
                {
                    v8 = _snprintf(v7, v4 - v7, "%s        ...%s, line %i", v6 + 64, v6 + 196, *((_DWORD*)v6 + 66));
                }
                else if (v6[64])
                {
                    v8 = _snprintf(v7, v4 - v7, "%s        ...%s, address %x", v6 + 64, v6 + 128, *((_DWORD*)v6 - 1));
                }
                else
                {
                    v8 = _snprintf(v7, v4 - v7, "%s, address %x", v6 + 64, *((_DWORD*)v6 - 1));
                }
                v5 = &v7[v8 + _snprintf(&v7[v8], v4 - &v7[v8], "\n")];
            }
            v6 += 272;
            ++j;
        } while (j < g_assertAddressCount);
        v3 = msg;
    }
    g_inStackTrace = 0;
    return v5 - v3;
}

assertive::AddressInfo_s* assertive::StackTrace_GetAddressInfo(int* addressCount)
{
    if (addressCount)
        *addressCount = g_assertAddressCount;
    return g_assertAddress;
}

void assertive::StackTrace_ResetAddressInfo()
{
    g_assertAddressCount = 0;
}

int assertive::StackTrace_ResolveSymbols()
{
    if (!g_inStackTrace)
    {
        g_inStackTrace = 1;
        LoadMapFilesForDir((const char*)&pBlock);
        g_inStackTrace = 0;
    }
    return 0;
}

int assertive::StackTrace_Walk(int ignoreCount, void* context)
{
    int result; // eax
    int i; // edx
    int* v4; // ecx
    int v5; // eax
    int* reg_ebp; // [esp+4h] [ebp-4h]
    int savedregs; // [esp+8h] [ebp+0h]

    if (g_inStackTrace)
        return 0;
    g_inStackTrace = 1;
    memset((char*)g_assertAddress, 0, 0x2200u);
    g_assertAddressCount = 0;
    if (context)
        reg_ebp = (int*)context;
    else
        reg_ebp = &savedregs;
    for (i = 0; i < ignoreCount + 32; ++i)
    {
        v4 = reg_ebp;
        reg_ebp = (int*)*reg_ebp;
        if (i >= ignoreCount)
        {
            v5 = g_assertAddressCount + 1;
            g_assertAddress[g_assertAddressCount].address = v4[1] - 5;
            g_assertAddressCount = v5;
            if (!reg_ebp)
                break;
        }
    }
    result = g_assertAddressCount;
    g_inStackTrace = 0;
    return result;
}

char assertive::ReadLine(_iobuf* fp)
{
    int v1; // eax
    signed int v2; // esi
    signed int v3; // edi
    int v4; // eax
    bool v6; // zf

    while (1)
    {
        lineBufferEndPos -= lineBufferStartPos;
        if ((lineBufferEndPos & 0x80000000) != 0
            && !(unsigned __int8)Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(lineBufferEndPos >= 0)",
                (const char*)&pBlock))
        {
            __debugbreak();
        }
        memmove(lineBuffer, &lineBuffer[lineBufferStartPos], lineBufferEndPos);
        v1 = 0;
        lineBufferStartPos = 0;
        if ((signed int)lineBufferEndPos > 0)
        {
            while (lineBuffer[v1] != 10)
            {
                if (++v1 >= (signed int)lineBufferEndPos)
                    goto LABEL_9;
            }
            if (v1 + 1 != lineBufferEndPos)
                break;
        }
    LABEL_9:
        v2 = 4095 - lineBufferEndPos;
        v3 = fread(&lineBuffer[lineBufferEndPos], 1u, 4095 - lineBufferEndPos, fp);
        if (v3 > v2
            && !(unsigned __int8)Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(bytesRead <= readSize)",
                (const char*)&pBlock))
        {
            __debugbreak();
        }
        v4 = v3 + lineBufferEndPos;
        lineBufferEndPos = v4;
        lineBuffer[v4] = 0;
        if (!v3)
        {
            if (!v4)
                return 0;
            goto LABEL_18;
        }
    }
    v6 = *(_BYTE*)(v1 + 127084113) == 13;
    lineBuffer[v1] = 0;
    if (v6)
    {
        lineBufferStartPos = v1 + 2;
        return 1;
    }
    v4 = v1 + 1;
LABEL_18:
    lineBufferStartPos = v4;
    return 1;
}

char assertive::SkipLines(int lineCount, _iobuf* fp)
{
    unsigned int v2; // eax
    signed int v3; // eax
    int v4; // ecx
    signed int v5; // esi
    signed int v6; // edi
    bool v8; // zf
    int i; // [esp+Ch] [ebp-4h]

    i = 0;
    if (lineCount <= 0)
        return 1;
    v2 = lineBufferEndPos;
    while (1)
    {
        do
        {
            v3 = v2 - lineBufferStartPos;
            lineBufferEndPos = v3;
            if (v3 < 0)
            {
                if (!(unsigned __int8)Assert_MyHandler(
                    __FILE__,
                    __LINE__,
                    0,
                    "(lineBufferEndPos >= 0)",
                    (const char*)&pBlock))
                    __debugbreak();
                v3 = lineBufferEndPos;
            }
            memmove(lineBuffer, &lineBuffer[lineBufferStartPos], v3);
            v2 = lineBufferEndPos;
            v4 = 0;
            lineBufferStartPos = 0;
            if ((signed int)lineBufferEndPos > 0)
            {
                while (lineBuffer[v4] != 10)
                {
                    if (++v4 >= (signed int)lineBufferEndPos)
                        goto LABEL_12;
                }
                if (v4 + 1 != lineBufferEndPos)
                {
                    v8 = *(_BYTE*)(v4 + 127084113) == 13;
                    lineBuffer[v4] = 0;
                    if (v8)
                        lineBufferStartPos = v4 + 2;
                    else
                        lineBufferStartPos = v4 + 1;
                    goto LABEL_18;
                }
            }
        LABEL_12:
            v5 = 4095 - lineBufferEndPos;
            v6 = fread(&lineBuffer[lineBufferEndPos], 1u, 4095 - lineBufferEndPos, fp);
            if (v6 > v5
                && !(unsigned __int8)Assert_MyHandler(
                    __FILE__,
                    __LINE__,
                    0,
                    "(bytesRead <= readSize)",
                    (const char*)&pBlock))
            {
                __debugbreak();
            }
            v2 = v6 + lineBufferEndPos;
            lineBufferEndPos = v2;
            lineBuffer[v2] = 0;
        } while (v6);
        if (!v2)
            return 0;
        lineBufferStartPos = v2;
    LABEL_18:
        if (++i >= lineCount)
            return 1;
    }
}

void Com_PrintStackTrace(int code, void(__cdecl* cb)(const char*))
{
    Com_Printf(10, "STACKBEGIN -------------------------------------------------------------------\n");
    assertive::StackTrace_Walk(1, 0);
    assertive::StackTrace_ResolveSymbols();
    assertive::StackTrace_Generate(0x2000, g_stackTrace);
    Com_Printf(10, "%s", g_stackTrace);
    Com_Printf(10, "STACKEND ---------------------------------------------------------------------\n");
    if (cb)
        cb(g_stackTrace);
}

void CopyMessageToClipboard(const char* msg)
{
    HWND v1; // eax
    HGLOBAL v2; // eax
    void* v3; // esi
    _BYTE* v4; // eax
    char* v5; // ecx
    int v6; // edx
    char v7; // al

    v1 = GetDesktopWindow();
    if (OpenClipboard(v1))
    {
        EmptyClipboard();
        v2 = GlobalAlloc(2u, strlen(msg) + 1);
        v3 = v2;
        if (v2)
        {
            v4 = (_BYTE*)GlobalLock(v2);
            if (v4)
            {
                v5 = (char*)msg;
                v6 = v4 - (_BYTE*)msg;
                do
                {
                    v7 = *v5;
                    v5[v6] = *v5;
                    ++v5;
                } while (v7);
                GlobalUnlock(v3);
                SetClipboardData(1u, v3);
            }
        }
        CloseClipboard();
    }
}

void FixWindowsDesktop()
{
    DWORD v0; // eax
    HWND v1; // esi
    HDC v2; // edi
    int v3; // eax
    unsigned __int16* v4; // ecx
    signed int v5; // edx
    unsigned __int16 ramp[3][256]; // [esp+8h] [ebp-604h]

    ChangeDisplaySettingsA(0, 0);
    v0 = GetCurrentThreadId();
    EnumThreadWindows(v0, (WNDENUMPROC)HideWindowCallback, 0);
    v1 = GetDesktopWindow();
    v2 = GetDC(v1);
    v3 = 0;
    v4 = ramp[1];
    v5 = 256;
    do
    {
        *(v4 - 256) = v3;
        *v4 = v3;
        v4[256] = v3;
        v3 += 257;
        ++v4;
        --v5;
    } while (v5);
    SetDeviceGammaRamp(v2, ramp);
    ReleaseDC(v1, v2);
}

int HideWindowCallback(HWND__* hwnd, int lParam)
{
    LONG v2; // esi
    LONG v3; // edi
    int v4; // eax
    char caption[1024]; // [esp+8h] [ebp-404h]

    if (!GetWindowTextA(hwnd, caption, 1024) || !strcmp(caption, Com_GetBuildDisplayNameR()))
    {
        v2 = GetWindowLongA(hwnd, -16);
        v3 = GetWindowLongA(hwnd, -20);
        if (v2 & 0x10000000)
        {
            v4 = g_hiddenCount;
            g_hwndGame[g_hiddenCount] = hwnd;
            g_hiddenCount = v4 + 1;
            SetWindowLongA(hwnd, -16, v2 & 0xEFFFFFFF);
            SetWindowLongA(hwnd, -20, v3 & 0xFFFFFFF7);
        }
    }
    return 1;
}

char AssertNotify(int type, AssertOccurance occurance)
{
    const char* v2; // edi
    HWND v3; // eax

    if (AssertCallback)
        AssertCallback(assertive::assertMessage);
    if (type)
    {
        if (type == 1)
            v2 = "SANITY CHECK FAILURE... (this text is on the clipboard)";
        else
            v2 = "INTERNAL ERROR";
    }
    else
    {
        v2 = "ASSERTION FAILURE... (this text is on the clipboard)";
    }
    ShowCursor(1);
    ShowCursor(1);
    v3 = GetActiveWindow();
    if (MessageBoxA(v3, assertive::assertMessage, v2, 0x12011u) == 1 && occurance != 1)
    {
        if (Sys_IsMiniDumpStarted() && !IsDebuggerPresent())
            RaiseException(1u, 0, 0, 0);
        //LiveSteam_Shutdown();
        exit(-1);
    }
    return 1;
}
