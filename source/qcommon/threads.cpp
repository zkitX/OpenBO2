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
    tagTHREADNAME_INFO info;
    CPPEH_RECORD ms_exc;

    info.dwType = 4096;
    info.szName = szThreadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;
    ms_exc.registration.TryLevel = 0;
    RaiseException(0x406D1388, 0, 4, (const ULONG_PTR*)&info.dwType);
}

void Sys_CreateEvent(int manualReset, int initialState, void** evt)
{
    HANDLE eventResponse; // eax
    void* eventPointer; // esi
    DWORD lastError; // eax

    eventResponse = CreateEventA(0, manualReset, initialState, 0);
    eventPointer = eventResponse;
    if (eventResponse)
        *evt = eventResponse;
    else
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
#ifdef _DEBUG
        if (!Assert_MyHandler(
            __FILE__,
            __LINE__,
            (int)eventPointer,
            "(e != 0)",
            nullptr))
            __debugbreak();
#endif // _DEBUG
        *evt = eventPointer;
    }
}

void Sys_CreateThread(unsigned int threadContext, void(*function)(unsigned int))
{
    HANDLE newThread; // eax
    DWORD lastError; // eax
#ifdef _DEBUG
    if (threadFunc[threadContext]
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(threadFunc[threadContext] == 0)",
            nullptr))
    {
        __debugbreak();
    }
    if (threadContext >= 0x11
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(threadContext < THREAD_CONTEXT_COUNT)",
            nullptr))
    {
        __debugbreak();
    }
#endif
    threadFunc[threadContext] = function;
    newThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Sys_ThreadMain, (LPVOID)threadContext, 4u, (LPDWORD)(4 * threadContext + 125788032));
    threadHandle[threadContext] = newThread;
#ifdef _DEBUG
    if (!newThread
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(threadHandle[threadContext] != 0)",
            nullptr))
    {
        __debugbreak();
    }
#endif
    if (!threadHandle[threadContext])
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating thread %d\n", lastError, threadContext);
    }
    SetThreadName(threadId[threadContext], s_threadNames[threadContext]);
}

void Sys_DatabaseCompleted()
{
    DWORD eventComplete; // eax

    g_databaseStopServer = 1;
    if (serverCompletedEvent)
    {
        eventComplete = WaitForSingleObject(serverCompletedEvent, 0xFFFFFFFF);
        if (eventComplete)
        {
            if (!Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "((result == ((((DWORD )0x00000000L) ) + 0 )))",
                "(result) = %i",
                eventComplete))
                __debugbreak();
        }
    }
    SetEvent(databaseCompletedEvent);
}

void Sys_FrontEndSleep()
{
    // TODO
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
            if (!(unsigned __int8)Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(0)",
                nullptr))
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
        if (!Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(e != 0)",
            nullptr))
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
    HANDLE webmEvent; // esi
    DWORD lastError; // eax

    webmEvent = CreateEventA(0, 0, 0, 0);
    if (!webmEvent)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(e != 0)",
            nullptr))
            __debugbreak();
    }
    webmStreamingReady = webmEvent;
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
    if (!backendEvent[1]
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(Sys_IsEventInitialized( backendEvent[BACKEND_EVENT_GENERIC] ))",
            nullptr))
    {
        __debugbreak();
    }
}

void Sys_ResetServerNetworkCompletedEvent()
{
    Sys_EnterCriticalSection(CRITSECT_NETTHREAD_OVERRIDE);
    if (g_networkOverrideThread
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(!g_networkOverrideThread)",
            nullptr)) {
        __debugbreak();
    }
    if (!g_currentThreadId) {
        g_currentThreadId = GetCurrentThreadId();
    }
    g_networkOverrideThread = g_currentThreadId;
    ResetEvent(serverNetworkCompletedEvent);
    Sys_LeaveCriticalSection(CRITSECT_NETTHREAD_OVERRIDE);
}

void Sys_SetServerNetworkCompletedEvent()
{
    Sys_EnterCriticalSection(CRITSECT_NETTHREAD_OVERRIDE);
    if (!g_currentThreadId)
        g_currentThreadId = GetCurrentThreadId();
    if (g_networkOverrideThread != g_currentThreadId
        && g_networkOverrideThread
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(g_networkOverrideThread == Sys_GetCurrentThreadId() || g_networkOverrideThread == 0)",
            nullptr))
    {
        __debugbreak();
    }
}

void Sys_SetValue(int valueIndex, void* data)
{
    g_dwTlsIndex[valueIndex] = data;
}

char Sys_SpawnDatabaseThread(void(* function)(unsigned int))
{
    HANDLE eventHandle;
    DWORD lastError;

    eventHandle = CreateEventA(0, 0, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    wakeDatabaseEvent = eventHandle;
    eventHandle = CreateEventA(0, 1, 1, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    databaseCompletedEvent = eventHandle;
    eventHandle = CreateEventA(0, 1, 1, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    databaseCompletedEvent2 = eventHandle;
    eventHandle = CreateEventA(0, 1, 1, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    resumedDatabaseEvent = eventHandle;
    eventHandle = CreateEventA(0, 0, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    gumpLoadedEvent = eventHandle;
    eventHandle = CreateEventA(0, 0, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    gumpFlushedEvent = eventHandle;
    Sys_CreateThread(12, function);
    if (!threadHandle[12])
        return 0;
    ResumeThread(threadHandle[12]);
    return 1;
}

char Sys_SpawnServerThread(void(*function)(unsigned int))
{
    HANDLE eventHandle;
    DWORD lastError;

    eventHandle = CreateEventA(0, 1, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    wakeServerEvent = eventHandle;
    eventHandle = CreateEventA(0, 1, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    serverCompletedEvent = eventHandle;
    eventHandle = CreateEventA(0, 1, 1, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    allowServerNetworkEvent = eventHandle;
    eventHandle = CreateEventA(0, 1, 1, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    serverNetworkCompletedEvent = eventHandle;
    Sys_CreateThread(10, function);
    if (!threadHandle[10])
        return 0;
    ResumeThread(threadHandle[10]);
    return 1;
}

char Sys_SpawnStreamThread(void(*function)(unsigned int))
{
    HANDLE eventHandle;
    DWORD lastError;

    eventHandle = CreateEventA(0, 1, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    sndInitializedEvent = eventHandle;
    eventHandle = CreateEventA(0, 1, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    streamCompletedEvent = eventHandle;
    eventHandle = CreateEventA(0, 1, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    streamDatabasePausedReading = eventHandle;
    eventHandle = CreateEventA(0, 0, 0, 0);
    if (!eventHandle)
    {
        lastError = GetLastError();
        Com_Printf(1, "error %d while creating event\n", lastError);
        if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(e != 0)", nullptr))
            __debugbreak();
    }
    streamEvent = eventHandle;
    Sys_CreateThread(13u, function);
    if (!threadHandle[13])
        return 0;
    ResumeThread(threadHandle[13]);
    return 1;

}

void Sys_StreamSleep()
{
    DWORD returnedValue; // eax

    SetEvent(streamCompletedEvent);
    SetEvent(streamDatabasePausedReading);
    returnedValue = WaitForSingleObject(streamEvent, 0xFFFFFFFF);
    if (returnedValue
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((result == ((((DWORD )0x00000000L) ) + 0 )))",
            "(result) = %i",
            returnedValue))
    {
        __debugbreak();
    }
    ResetEvent(streamCompletedEvent);
    ResetEvent(streamDatabasePausedReading);
}

unsigned int Sys_ThreadMain(void* parameter)
{
#ifdef _DEBUG
    if ((unsigned int)parameter >= 0x11
        && !(unsigned __int8)Assert_MyHandler(
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
        && !(unsigned __int8)Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(threadFunc[threadContext])",
            nullptr))
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
    DWORD response; // eax

    response = WaitForSingleObject(demoStreamingReady, 0xFFFFFFFF);
    if (response)
    {
        if (!Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((result == ((((DWORD )0x00000000L) ) + 0 )))",
            "(result) = %i",
            response))
            __debugbreak();
    }
}

bool Sys_WaitForDemoStreamingEventTimeout(unsigned int msec)
{
    if (msec == -1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(msec != 0xFFFFFFFF)",
            "%s",
            "msec != INFINITE"))
    {
        __debugbreak();
    }
    return WaitForSingleObject(demoStreamingReady, msec) == 0;
}

bool Sys_WaitForGumpFlush(int timeout)
{
    if (timeout == -1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(msec != 0xFFFFFFFF)",
            "%s",
            "msec != INFINITE"))
    {
        __debugbreak();
    }
    return WaitForSingleObject(gumpFlushedEvent, timeout) == 0;
}

bool Sys_WaitForGumpLoad(int timeout)
{
    if (timeout == -1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(msec != 0xFFFFFFFF)",
            "%s",
            "msec != INFINITE"))
    {
        __debugbreak();
    }
    return WaitForSingleObject(gumpLoadedEvent, timeout) == 0;
}

void Sys_WaitForSingleObject(void** event)
{
    DWORD eventResponse;

    eventResponse = WaitForSingleObject(*event, 0xFFFFFFFF);
    if (eventResponse)
    {
        if (!Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((result == ((((DWORD )0x00000000L) ) + 0 )))",
            "(result) = %i",
            eventResponse))
            __debugbreak();
    }
}

bool Sys_WaitForSingleObjectTimeout(void** event, unsigned int msec)
{
    if (msec == -1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(msec != 0xFFFFFFFF)",
            "%s",
            "msec != INFINITE"))
    {
        __debugbreak();
    }
    return WaitForSingleObject(*event, msec) == 0;
}

bool Sys_WaitServer(int timeout)
{
    if (timeout == -1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(msec != 0xFFFFFFFF)",
            "%s",
            "msec != INFINITE"))
    {
        __debugbreak();
    }
    return WaitForSingleObject(serverCompletedEvent, timeout) == 0;
}

void Sys_WaitServerNetworkCompleted()
{
    DWORD response;

    if (!g_currentThreadId)
        g_currentThreadId = GetCurrentThreadId();
    response = WaitForSingleObject(serverNetworkCompletedEvent, 0xFFFFFFFF);
    if (response)
    {
        if (!Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((result == ((((DWORD )0x00000000L) ) + 0 )))",
            "(result) = %i",
            response))
            __debugbreak();
    }
}

void Sys_WaitStartDatabase()
{
    DWORD response; // eax

    response = WaitForSingleObject(wakeDatabaseEvent, 0xFFFFFFFF);
    if (response)
    {
        if (!Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((result == ((((DWORD )0x00000000L) ) + 0 )))",
            "(result) = %i",
            response))
            __debugbreak();
    }
}

bool Sys_WaitStartServer(int timeout)
{
    DWORD response;
    bool responseCheck;

    if (timeout == -1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(msec != 0xFFFFFFFF)",
            "%s",
            "msec != INFINITE"))
    {
        __debugbreak();
    }
    response = WaitForSingleObject(wakeServerEvent, timeout);
    responseCheck = response == 0;
    if (!response)
        ResetEvent(serverCompletedEvent);
    if (g_databaseStopServer)
        return false;
    else
        return responseCheck;
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
