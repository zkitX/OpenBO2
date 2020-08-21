#include "win_main.h"
#include "win_localize.h"

#include "../qcommon/cmd.h"
#include "../qcommon/common.h"
#include "../universal/blackbox.h"

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

void miniDumper::Sys_StartMiniDump(bool prompt)
{
    _RTL_CRITICAL_SECTION* v1; // eax

    if (!(_S1_19 & 1))
    {
        _S1_19 |= 1u;
        miniDumper::s_pMiniDumper = &g_miniDumper;
        g_miniDumper.m_bPromptUserForMiniDump = prompt;
        SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)miniDumper::unhandledExceptionHandler);
        v1 = (_RTL_CRITICAL_SECTION*)operator new(0x18u);
        miniDumper::s_pCriticalSection = v1;
        if (v1)
            InitializeCriticalSection(v1);
        if (miniDumper::s_pCriticalSection)
        {
            DeleteCriticalSection(miniDumper::s_pCriticalSection);
            operator delete(miniDumper::s_pCriticalSection);
        }
    }
    g_miniDumperStarted = 1;
    Cmd_AddCommandInternal("dumpdir", SetDumpDir_f, &SetDumpDir_f_VAR);
}

int miniDumper::unhandledExceptionHandler(_EXCEPTION_POINTERS* pExceptionInfo)
{
    int v2; // esi
    HANDLE v3; // eax

    if (!miniDumper::s_pMiniDumper)
        return 0;
    SV_SysLog_ForceFlush();
    v2 = miniDumper::writeMiniDump(
        miniDumper::s_pMiniDumper,
        pExceptionInfo,
        "crash",
        MiniDumpWithDataSegs,
        miniDumper::s_pMiniDumper->m_bPromptUserForMiniDump,
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
    void* TokenHandle; // [esp+20h] [ebp-1124h]
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
    if (!OpenThreadToken(v7, 0x28u, 1, &TokenHandle))
    {
        if (GetLastError() != 1008)
            return 0;
        v8 = GetCurrentProcess();
        if (!OpenProcessToken(v8, 0x28u, &TokenHandle))
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
            v12[v6->m_szAppBaseName - v11] = *v12;
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
    *(_DWORD*)v16 = *(_DWORD*)aDbgh;
    *((_DWORD*)v16 + 1) = 777014341;
    *((_DWORD*)v16 + 2) = &loc_4C4C44;
    hModule = LoadLibraryA(LibFileName);
    if (!hModule)
        LABEL_38:
    hModule = LoadLibraryA(aDbgh);
    lpText = 0;
    if (hModule)
    {
        v31 = GetProcAddress(hModule, "MiniDumpWriteDump");
        if (v31)
        {
            miniDumper::setMiniDumpFileName(v6, taga);
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
                    v19 = miniDumper::enablePrivilege(v6, "SeDebugPrivilege", TokenHandle, &ptpOld);
                    EnterCriticalSection(miniDumper::s_pCriticalSection);
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
                    LeaveCriticalSection(miniDumper::s_pCriticalSection);
                    if (v19)
                        miniDumper::restorePrivilege(v6, TokenHandle, &ptpOld);
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
                    if (v22 && upload)
                        UploadMinidump(taga);
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

void miniDumper::SetDumpDir_f()
{
    const char* dirArg; // esi

    dirArg = Cmd_Argv(1);
    Sys_MkdirEx(dirArg);
    strncpy(miniDumper::s_pMiniDumper->m_szMiniDumpDir, dirArg, 0x104u);
}

void Sys_OutOfMemErrorInternal(int a1, const char* filename, int line)
{
    const char* localizedMemTitle; // ST1C_4
    const char* localizedMemBody; // ST18_4
    HWND activeWindow; // eax

    ShowCursor(1);
    Sys_EnterCriticalSection(CRITSECT_FATAL_ERROR);
    blackbox::BB_Alert(__FILE__, __LINE__, "error", "Out of Memory");
    common::Com_Printf(a1, 10, "Out of memory: filename '%s', line %d\n", filename, line);
    localizedMemTitle = Win_LocalizeRef("WIN_OUT_OF_MEM_TITLE");
    localizedMemBody = Win_LocalizeRef("WIN_OUT_OF_MEM_BODY");
    activeWindow = GetActiveWindow();
    MessageBoxA(activeWindow, localizedMemBody, localizedMemTitle, 0x10u);
    //LiveSteam_Shutdown();
    exit(-1);
}

int __stdcall WinMain(HINSTANCE__* hInstance, HINSTANCE__* hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    HMODULE user32; // eax
    HMODULE freeUser32; // edi
    void (*v6)(void); // eax
    bool v7; // bl
    char pnbuf; // [esp+10h] [ebp-104h]
    int savedregs; // [esp+114h] [ebp+0h]

    user32 = LoadLibraryA("user32.dll");
    freeUser32 = user32;
    if (user32)
    {
        v6 = (void (*)(void))GetProcAddress(user32, "SetProcessDPIAware");
        if (v6)
            v6();
        FreeLibrary(freeUser32);
    }
    /*InitMiniDumper(lpCmdLine);
    Sys_CreateConsole(hInstance);
    Sys_ShowConsole();
    if (I_stristr(lpCmdLine, "usedevlsg"))
    {
        g_dwUseDevLSG = 1;
    }
    else if (I_stristr(lpCmdLine, "useprodlsg"))
    {
        g_dwUseDevLSG = 0;
    }
    Sys_InitializeCriticalSections();
    Sys_InitMainThread();
    g_quitRequested = 0;
    Com_InitParse();
    DedicatedInit(lpCmdLine);
    PMem_Init();
    track_init();
    v7 = Win_CheckForZombieMode(lpCmdLine);
    R_SetIsMultiplayer(1);
    R_SetIsZombie(v7);
    Win_InitLocalization(v7);
    if (G_ExitAfterConnectPaths())
    {
        g_pc_nosnd = 1;
    }
    else if (GetSystemMetrics(4096))
    {
        g_pc_nosnd = 1;
    }
    else if (I_stristr(lpCmdLine, "nosnd"))
    {
        g_pc_nosnd = 1;
    }
    else if (I_stristr(lpCmdLine, "shield"))
    {
        g_pc_snd_defaultdevice = 1;
    }
    if (!hPrevInstance)
    {
        SL_Init();
        Dvar_Init((int)&savedregs);
        InitTiming();
        Sys_FindInfo();
        I_strncpyz(sys_cmdline, lpCmdLine, 1024);
        SetErrorMode(1u);
        Sys_Milliseconds();
        tlPrintf("Hello from the wonderful world of TL\n");
        Sys_SetupTLCallbacks(9437184);
        Con_InitChannels();
        TaskManager2_Init();
        if (v7)
        {
            Com_Printf(10, "----> Zombie Session Mode Set! <----\n");
            Com_SessionMode_SetMode(SESSIONMODE_ZOMBIES, 1);
        }
        Win_CheckForEnv(lpCmdLine);
        Live_Base_Init();
        Live_Base_Pump();
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
        Com_Init(sys_cmdline, 0);
        Live_Base_Pump();
        _getcwd(&pnbuf, 256);
        Com_Printf(10, "Working directory: %s\n", &pnbuf);
        if (Dvar_GetBool(com_script_debugger_smoke_test))
            exit(31415);
        while (1)
        {
            Sleep(5u);
            if (g_quitRequested && Sys_IsMainThread())
                break;
            Com_Frame();
        }
        if (errPtr[0])
            Sys_Error(errPtr);
        Sys_Error("Error quit was not requested in the main thread\n");
    }
    Win_ShutdownLocalization();
    track_shutdown(0);
    return 0;*/
}
