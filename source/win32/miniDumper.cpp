#include "miniDumper.h"
#include "win_main.h"

bool miniDumper::enablePrivilege(const char* pszPriv, void* hToken, _TOKEN_PRIVILEGES* ptpOld)
{
    bool result; // eax
    unsigned int cbOld; // [esp+8h] [ebp-18h]
    _TOKEN_PRIVILEGES tp; // [esp+Ch] [ebp-14h]

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = 2;
    result = 0;
    if (LookupPrivilegeValueA(0, pszPriv, (PLUID)tp.Privileges))
    {
        cbOld = 16;
        if (AdjustTokenPrivileges(hToken, 0, &tp, 0x10u, ptpOld, (PDWORD)&cbOld))
        {
            if (GetLastError() != 1300)
                result = 1;
        }
    }
    return result;
}

void miniDumper::InitMiniDumper(char* lpCmdLine)
{
    if (I_stristr(lpCmdLine, "autodump"))
    {
        Sys_StartMiniDump(0);
    }
    else if (I_stristr(lpCmdLine, "minidump") || !I_stristr(lpCmdLine, "nodump"))
    {
        Sys_StartMiniDump(1);
    }
    else
    {
        SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)PrivateUnhandledExceptionFilter);
    }
}

void miniDumper::setMiniDumpFileName(miniDumper*, const char* tag)
{
}

void Sys_StartMiniDump(bool prompt)
{
    _RTL_CRITICAL_SECTION* v1; // eax

    if (!(_S1_19 & 1))
    {
        _S1_19 |= 1u;
        s_pMiniDumper = &g_miniDumper;
        g_miniDumper.m_bPromptUserForMiniDump = prompt;
        SetUnhandledExceptionFilter(g_miniDumper.unhandledExceptionHandler);
        v1 = (_RTL_CRITICAL_SECTION*)operator new(0x18u);
        g_miniDumper.s_pCriticalSection = v1;
        if (v1)
            InitializeCriticalSection(v1);
        if (g_miniDumper.s_pCriticalSection)
        {
            DeleteCriticalSection(g_miniDumper.s_pCriticalSection);
            operator delete(g_miniDumper.s_pCriticalSection);
        }
    }
    g_miniDumperStarted = 1;
    Cmd_AddCommandInternal("dumpdir", SetDumpDir_f, &SetDumpDir_f_VAR);
}

int miniDumper::unhandledExceptionHandler(_EXCEPTION_POINTERS* pExceptionInfo)
{
    int v2; // esi
    HANDLE v3; // eax

    if (!s_pMiniDumper)
        return 0;
    SV_SysLog_ForceFlush();
    v2 = miniDumper::writeMiniDump(
        s_pMiniDumper,
        pExceptionInfo,
        "crash",
        MiniDumpWithDataSegs,
        s_pMiniDumper->m_bPromptUserForMiniDump,
        1);
    v3 = GetCurrentProcess();
    TerminateProcess(v3, 0);
    return v2;
}

int miniDumper::writeMiniDump(miniDumper*, _EXCEPTION_POINTERS* pExceptionInfo, const char* tag, _MINIDUMP_TYPE type, bool prompt, bool upload)
{
    miniDumper* v6; // esi
    HANDLE v7; // eax
    HANDLE v8; // eax
    int v10; // eax
    _BYTE* v11; // edi
    char* v12; // eax
    char v13; // cl
    char* v14; // eax
    char v15; // cl
    char* v16; // eax
    char v17; // cl
    HANDLE v18; // ebx
    int v19; // edi
    DWORD v20; // ST04_4
    HANDLE v21; // eax
    FARPROC v22; // esi
    DWORD v23; // eax
    DWORD v24; // eax
    DWORD v25; // [esp+4h] [ebp-1140h]
    _EXCEPTION_POINTERS* v26; // [esp+8h] [ebp-113Ch]
    int v27; // [esp+Ch] [ebp-1138h]
    _EXCEPTION_POINTERS* v28; // [esp+10h] [ebp-1134h]
    int v29; // [esp+14h] [ebp-1130h]
    HMODULE hModule; // [esp+18h] [ebp-112Ch]
    FARPROC v31; // [esp+1Ch] [ebp-1128h]
    _TOKEN_PRIVILEGES* TokenHandle; // [esp+20h] [ebp-1124h]
    char* taga; // [esp+24h] [ebp-1120h]
    LPCSTR lpText; // [esp+28h] [ebp-111Ch]
    _TOKEN_PRIVILEGES ptpOld; // [esp+2Ch] [ebp-1118h]
    char string; // [esp+3Ch] [ebp-1108h]
    char v37; // [esp+103Bh] [ebp-109h]
    char LibFileName[260]; // [esp+103Ch] [ebp-108h]

    v6 = this;
    v28 = pExceptionInfo;
    taga = (char*)tag;
    v29 = 0;
    this->m_pExceptionInfo = pExceptionInfo;
    TokenHandle = 0;
    v7 = GetCurrentThread();
    if (!OpenThreadToken(v7, 0x28u, 1, (PHANDLE)&TokenHandle))
    {
        if (GetLastError() != 1008)
            return 0;
        v8 = GetCurrentProcess();
        if (!OpenProcessToken(v8, 0x28u, (PHANDLE)&TokenHandle))
            return 0;
    }
    if (!GetModuleFileNameA(0, v6->m_szAppPath, 0x104u))
        goto LABEL_38;
    strrchr(v6->m_szAppPath, 92);
    if (v10)
    {
        v11 = (_BYTE*)(v10 + 1);
        v12 = (char*)(v10 + 1);
        do
        {
            v13 = *v12;
            v12[(_BYTE*)v6->m_szAppBaseName - v11] = *v12;
            ++v12;
        } while (v13);
        *v11 = 0;
    }
    v14 = v6->m_szAppPath;
    do
    {
        v15 = *v14;
        v14[LibFileName - v6->m_szAppPath] = *v14;
        ++v14;
    } while (v15);
    v16 = &v37;
    do
        v17 = (v16++)[1];
    while (v17);
    hModule = LoadLibraryA(LibFileName);
    if (!hModule)
        LABEL_38:
    hModule = LoadLibraryA("DBGHELP.DLL");
    lpText = 0;
    if (hModule)
    {
        v31 = GetProcAddress(hModule, "MiniDumpWriteDump");
        if (v31)
        {
            g_miniDumper.setMiniDumpFileName(v6, taga);
            ShowCursor(1);
            taga = v6->m_szMiniDumpPath;
            sprintf(
                &string,
                "There was an unexpected error:\n\nWould you like to create a mini-dump file?\n\n%s ",
                v6->m_szMiniDumpPath);
            if (!prompt || MessageBoxA(0, &string, 0, 4u) == 6)
            {
                v18 = CreateFileA(v6->m_szMiniDumpPath, 0x40000000u, 2u, 0, 2u, 0x80u, 0);
                if (v18 == (HANDLE)-1)
                {
                    v24 = GetLastError();
                    sprintf(&string, "Failed to create the mini-dump file '%s' (error %d)", v6->m_szMiniDumpPath, v24);
                    lpText = &string;
                }
                else
                {
                    v25 = GetCurrentThreadId();
                    v26 = v28;
                    v27 = 0;
                    v19 = g_miniDumper.enablePrivilege((const char*)v6, "SeDebugPrivilege", TokenHandle, &ptpOld);
                    EnterCriticalSection(g_miniDumper.s_pCriticalSection);
                    v20 = GetCurrentProcessId();
                    v21 = GetCurrentProcess();
                    v31 = (FARPROC)((int(__stdcall*)(HANDLE, DWORD, HANDLE, _MINIDUMP_TYPE, DWORD*, _DWORD, _DWORD))v31)(
                        v21,
                        v20,
                        v18,
                        type,
                        &v25,
                        0,
                        0);
                    LeaveCriticalSection(g_miniDumper.s_pCriticalSection);
                    if (v19)
                        g_miniDumper.restorePrivilege(v6, TokenHandle, &ptpOld);
                    v22 = v31;
                    if (v31)
                    {
                        lpText = 0;
                        v29 = 1;
                    }
                    else
                    {
                        v23 = GetLastError();
                        sprintf(&string, "Failed to save the mini-dump file to '%s' (error %d)", taga, v23);
                        lpText = &string;
                    }
                    CloseHandle(v18);
                }
            }
        }
        else
        {
            lpText = "Call to GetProcAddress failed to find MiniDumpWriteDump. The DBGHELP.DLL is possibly outdated.";
        }
        FreeLibrary(hModule);
        if (!lpText)
            return v29;
    }
    else
    {
        lpText = "Call to LoadLibrary failed to find DBGHELP.DLL.";
    }
    if (prompt)
        MessageBoxA(0, lpText, 0, 0);
    return v29;
}

void SetDumpDir_f()
{
    const char* dirArg; // esi

    dirArg = Cmd_Argv(1);
    Sys_MkdirEx(dirArg);
    strncpy(s_pMiniDumper->m_szMiniDumpDir, dirArg, 0x104u);
}

bool Sys_IsMiniDumpStarted()
{
    return g_miniDumperStarted;
}