#include "win_main.h"
#include "win_localize.h"

#include "../qcommon/cmd.h"
#include "../qcommon/common.h"
#include "../universal/blackbox.h"

void Sys_Error(const char* error, ...)
{
    tagMSG Msg;
    char string;
    va_list ap;

    va_start(ap, error);
    _vsnprintf(&string, 0x1000u, error, ap);
    
}

void Sys_OutOfMemErrorInternal(int a1, const char* filename, int line)
{
    const char* localizedMemTitle; // ST1C_4
    const char* localizedMemBody; // ST18_4
    HWND activeWindow; // eax

    ShowCursor(1);
    Sys_EnterCriticalSection(CRITSECT_FATAL_ERROR);
    blackbox::BB_Alert(__FILE__, __LINE__, "error", "Out of Memory");
    Com_Printf(10, "Out of memory: filename '%s', line %d\n", filename, line);
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
    InitMiniDumper(lpCmdLine);
    /*Sys_CreateConsole(hInstance);
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
