#include "threads.h"

#include "common.h"
#include "../universal/assertive.h"

void threads::Sys_CreateEvent(int manualReset, int initialState, void** evt)
{
    HANDLE v3; // eax
    void* v4; // esi
    DWORD v5; // eax

    v3 = CreateEventA(0, manualReset, initialState, 0);
    v4 = v3;
    if (v3)
    {
        *evt = v3;
    }
    else
    {
        v5 = GetLastError();
        Com_Printf((int)v4, 1, "error %d while creating event\n", v5);
#ifdef _DEBUG
        if (!(unsigned __int8)assertive::Assert_MyHandler(
            "c:\\t6\\code\\src_noserver\\qcommon\\threads.cpp",
            292,
            (int)v4,
            "(e != 0)",
            (const char*)&pBlock))
            __debugbreak();
#endif // _DEBUG
        *evt = v4;
    }
}

void threads::Sys_CreateThread(unsigned int threadContext, void(*function)(unsigned int))
{
}

void threads::Sys_DatabaseCompleted()
{
}

void threads::Sys_FrontEndSleep()
{
}

int threads::Sys_GetThreadContext()
{
    return 0;
}

void threads::Sys_InitDemoStreamingEvent()
{
}

void threads::Sys_InitWebMStreamingEvent()
{
}

void threads::Sys_NotifyRenderer()
{
}

void threads::Sys_ResetServerNetworkCompletedEvent()
{
}

void threads::Sys_SetServerNetworkCompletedEvent()
{
}

char threads::Sys_SpawnDatabaseThread(void(*function)(unsigned int))
{
    return 0;
}

char threads::Sys_SpawnServerThread(void(*function)(unsigned int))
{
    return 0;
}

char threads::Sys_SpawnStreamThread(void(*function)(unsigned int))
{
    return 0;
}

void threads::Sys_StreamSleep()
{
}

unsigned int threads::Sys_ThreadMain(void* parameter)
{
    return 0;
}

void threads::Sys_WaitAllowServerNetworkLoop()
{
}

void threads::Sys_WaitForDemoStreamingEvent()
{
}

bool threads::Sys_WaitForDemoStreamingEventTimeout(unsigned int msec)
{
    return false;
}

bool threads::Sys_WaitForGumpFlush(int timeout)
{
    return false;
}

bool threads::Sys_WaitForGumpLoad(int timeout)
{
    return false;
}

void threads::Sys_WaitForSingleObject(void** event)
{
}

bool threads::Sys_WaitForSingleObjectTimeout(void** event, unsigned int msec)
{
    return false;
}

bool threads::Sys_WaitServer(int timeout)
{
    return false;
}

void threads::Sys_WaitServerNetworkCompleted()
{
}

void threads::Sys_WaitStartDatabase()
{
}

bool threads::Sys_WaitStartServer(int timeout)
{
    return false;
}
