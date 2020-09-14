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

void SetThreadName(unsigned int dwThreadID, const char* szThreadName)
{
    tagTHREADNAME_INFO info; // [esp+10h] [ebp-28h]
    CPPEH_RECORD ms_exc; // [esp+20h] [ebp-18h]

    info.dwType = 4096;
    info.szName = szThreadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;
    ms_exc.registration.TryLevel = 0;
    RaiseException(0x406D1388u, 0, 4u, (const ULONG_PTR*)&info.dwType);
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
        Com_Printf(1, "error %d while creating event\n", v5);
#ifdef _DEBUG
        if (!(unsigned char)assertive::Assert_MyHandler(
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
    HANDLE newThread; // eax
    DWORD v3; // eax
#ifdef _DEBUG
    if (threadFunc[threadContext]
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(threadFunc[threadContext] == 0)",
            &pBlock))
    {
        __debugbreak();
    }
    if (threadContext >= 0x11
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(threadContext < THREAD_CONTEXT_COUNT)",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    threadFunc[threadContext] = function;
    newThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Sys_ThreadMain, (LPVOID)threadContext, 4u, (LPDWORD)(4 * threadContext + 125788032));
    threadHandle[threadContext] = newThread;
#ifdef _DEBUG
    if (!newThread
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(threadHandle[threadContext] != 0)",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    if (!threadHandle[threadContext])
    {
        v3 = GetLastError();
        Com_Printf(1, "error %d while creating thread %d\n", v3, threadContext);
    }
    SetThreadName(threadId[threadContext], s_threadNames[threadContext]);
}

void Sys_DatabaseCompleted()
{
    DWORD v0; // eax

    g_databaseStopServer = 1;
    if (serverCompletedEvent)
    {
        v0 = WaitForSingleObject(serverCompletedEvent, 0xFFFFFFFF);
        if (v0)
        {
            if (!(unsigned __int8)assertive::Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "((result == ((((DWORD )0x00000000L) ) + 0 )))",
                "(result) = %i",
                v0))
                __debugbreak();
        }
    }
    SetEvent(databaseCompletedEvent);
}

void Sys_FrontEndSleep()
{
}

int Sys_GetThreadContext()
{
    int curthreadid;
    int result;
    if (!g_currentThreadId) {
        g_currentThreadId = GetCurrentThreadId();
    }
    curthreadid = g_currentThreadId;
    for (result = 0; threadId[result] != curthreadid; result++) {
        if (++result >= 17) {
            Com_Printf(1, "Current thread is not in thread table\n");
            if (!(unsigned __int8)assertive::Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(0)",
                &pBlock))
                __debugbreak();
            return 17;
        }
    }
    return result;
}

void* Sys_GetValue(int valueIndex)
{
    return g_dwTlsIndex[valueIndex];
}

void Sys_InitDemoStreamingEvent()
{
    HANDLE demoStreamingEvent; // esi
    DWORD v1; // eax

    demoStreamingEvent = CreateEventA(0, 0, 0, 0);
    if (!demoStreamingEvent)
    {
        v1 = GetLastError();
        Com_Printf(1, "error %d while creating event\n", v1);
        if (!(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(e != 0)",
            &pBlock))
            __debugbreak();
    }
    demoStreamingReady = demoStreamingEvent;
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

bool Sys_IsServerThread()
{
    if (!g_currentThreadId)
        g_currentThreadId = GetCurrentThreadId();
    return g_currentThreadId == threadId[10];
}

bool Sys_IsRenderThread()
{
    if (!g_currentThreadId)
        g_currentThreadId = GetCurrentThreadId();
    return g_currentThreadId == threadId[1];
}

bool Sys_IsDatabaseThread()
{
    if (!g_currentThreadId)
        g_currentThreadId = GetCurrentThreadId();
    return g_currentThreadId == threadId[12];
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

void Sys_SetValue(int valueIndex, void* data)
{
    g_dwTlsIndex[valueIndex] = data;
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
#ifdef _DEBUG
    if ((unsigned int)parameter >= 0x11
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(threadContext) < (unsigned)(THREAD_CONTEXT_COUNT)",
            "threadContext doesn't index THREAD_CONTEXT_COUNT\n\t%i not in [0, %i)",
            parameter,
            17))
    {
        __debugbreak();
    }
    if (!threadFunc[(unsigned int)parameter]
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(threadFunc[threadContext])",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    SetThreadName(0xFFFFFFFF, s_threadNames[(unsigned int)parameter]);
    *g_dwTlsIndex = g_threadValues[(int)parameter];
    Com_InitThreadData((int)parameter);
    threadFunc[(unsigned int)parameter]((unsigned int)parameter);
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

unsigned int Sys_GetCpuCount()
{
    return s_cpuCount;
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
