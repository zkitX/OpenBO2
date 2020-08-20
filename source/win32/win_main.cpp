#include "win_main.h"

int __stdcall WinMain(HINSTANCE__* hInstance, HINSTANCE__* hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    HMODULE v4; // eax
    HMODULE v5; // edi
    void (*v6)(void); // eax
    bool v7; // bl
    char pnbuf; // [esp+10h] [ebp-104h]
    int savedregs; // [esp+114h] [ebp+0h]

    v4 = LoadLibraryA("user32.dll");
    v5 = v4;
    if (v4)
    {
        v6 = (void (*)(void))GetProcAddress(v4, "SetProcessDPIAware");
        if (v6)
            v6();
        FreeLibrary(v5);
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
