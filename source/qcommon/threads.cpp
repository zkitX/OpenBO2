#include "threads.h"

#include "common.h"
#include "../universal/assertive.h"

void Com_InitThreadData(int threadContext)
{
    Sys_SetValue(1, &va_info[threadContext]);
    Sys_SetValue(2, g_com_error[threadContext]);
    Sys_SetValue(3, (void*)g_traceThreadInfo);
    Sys_SetValue(4, g_cmd_args);
}

void Sys_CreateEvent(int manualReset, int initialState, void** evt)
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
            __FILE__,
            __LINE__,
            (int)v4,
            "(e != 0)",
            (const char*)&pBlock))
            __debugbreak();
#endif // _DEBUG
        *evt = v4;
    }
}

void Sys_CreateThread(unsigned int threadContext, void(*function)(unsigned int))
{
}

void Sys_DatabaseCompleted()
{
}

void Sys_FrontEndSleep()
{
}

int Sys_GetThreadContext()
{
    return 0;
}

void Sys_InitDemoStreamingEvent()
{
}

void Sys_InitMainThread()
{
    if (!g_currentThreadId)
    {
        g_currentThreadId = GetCurrentThreadId();
    }
    threadId[0] = *(&g_currentThreadId);
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), threadHandle, 0, 0, 2u);
    Win_InitThreads();
    *g_dwTlsIndex = g_threadValues;
    Com_InitThreadData(0);
}

void Sys_InitWebMStreamingEvent()
{
}

bool Sys_IsMainThread()
{
    if (!g_currentThreadId)
        g_currentThreadId = GetCurrentThreadId();
    return g_currentThreadId == threadId[0];
}

void Sys_NotifyRenderer()
{
}

void Sys_ResetServerNetworkCompletedEvent()
{
}

void Sys_SetServerNetworkCompletedEvent()
{
}

char Sys_SpawnDatabaseThread(void(*function)(unsigned int))
{
    return 0;
}

char Sys_SpawnServerThread(void(*function)(unsigned int))
{
    return 0;
}

char Sys_SpawnStreamThread(void(*function)(unsigned int))
{
    return 0;
}

void Sys_StreamSleep()
{
}

unsigned int Sys_ThreadMain(void* parameter)
{
    return 0;
}

void Sys_WaitAllowServerNetworkLoop()
{
}

void Sys_WaitForDemoStreamingEvent()
{
}

bool Sys_WaitForDemoStreamingEventTimeout(unsigned int msec)
{
    return false;
}

bool Sys_WaitForGumpFlush(int timeout)
{
    return false;
}

bool Sys_WaitForGumpLoad(int timeout)
{
    return false;
}

void Sys_WaitForSingleObject(void** event)
{
}

bool Sys_WaitForSingleObjectTimeout(void** event, unsigned int msec)
{
    return false;
}

bool Sys_WaitServer(int timeout)
{
    return false;
}

void Sys_WaitServerNetworkCompleted()
{
}

void Sys_WaitStartDatabase()
{
}

bool Sys_WaitStartServer(int timeout)
{
    return false;
}

unsigned int Win_InitThreads()
{
    HANDLE v0; // eax
    unsigned int v1; // edx
    unsigned int result; // eax
    unsigned int v3; // ecx
    unsigned int v4; // ecx
    unsigned int systemAffinityMask; // [esp+0h] [ebp-8Ch]
    unsigned int processAffinityMask; // [esp+4h] [ebp-88h]
    unsigned int affinityMaskBits[32]; // [esp+8h] [ebp-84h]

    v0 = GetCurrentProcess();
    GetProcessAffinityMask(v0, (PDWORD_PTR)processAffinityMask, (PDWORD_PTR)systemAffinityMask);
    v1 = processAffinityMask;
    result = 0;
    s_affinityMaskForProcess = processAffinityMask;
    v3 = 1;
    if (!processAffinityMask)
        goto LABEL_17;
    do
    {
        if (v3 & v1)
        {
            affinityMaskBits[result++] = v3;
            if (result == 32)
                break;
        }
        v3 *= 2;
    } while (-v3 & v1);
    if (result && result != 1)
    {
        v4 = *(&processAffinityMask + result);
        s_cpuCount = result;
        s_affinityMaskForCpu[0] = affinityMaskBits[0];
        s_affinityMaskForCpu[1] = v4;
        if (result != 2)
        {
            if (result == 3)
            {
                s_affinityMaskForCpu[2] = affinityMaskBits[1];
            }
            else if (result == 4)
            {
                result = affinityMaskBits[1];
                s_affinityMaskForCpu[2] = affinityMaskBits[1];
                s_affinityMaskForCpu[3] = affinityMaskBits[2];
            }
            else
            {
                s_affinityMaskForCpu[0] = -1;
                s_affinityMaskForCpu[1] = -1;
                s_affinityMaskForCpu[2] = -1;
                s_affinityMaskForCpu[3] = -1;
                s_affinityMaskForCpu[4] = -1;
                s_affinityMaskForCpu[5] = -1;
                s_affinityMaskForCpu[6] = -1;
                s_affinityMaskForCpu[7] = -1;
                if (result >= 8)
                    s_cpuCount = 8;
            }
        }
    }
    else
    {
    LABEL_17:
        s_cpuCount = 1;
        s_affinityMaskForCpu[0] = -1;
    }
    return result;
}
