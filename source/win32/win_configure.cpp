#include "win_configure.h"

#include <intrin.h>
#include <immintrin.h>
#include <win32/win_localize.h>

int Sys_SystemMemoryMB()
{
    HMODULE v0; // eax
    FARPROC v1; // eax
    char* v2; // ST08_4
    char* v3; // ST04_4
    HWND v4; // eax
    float v5; // ST20_4
    signed int v6; // edx
    int result; // eax
    char* v8; // ST08_4
    char* v9; // ST04_4
    HWND v10; // eax
    float v11; // ST20_4
    signed int v12; // ecx
    const struct entityState_s* v13; // [esp+0h] [ebp-78h]
    const struct clientInfo_t* v14; // [esp+4h] [ebp-74h]
    _MEMORYSTATUSEX statusEx; // [esp+14h] [ebp-64h]
    _MEMORYSTATUS status; // [esp+54h] [ebp-24h]

    v0 = GetModuleHandleA("kernel32.dll");
    if (v0 && (v1 = GetProcAddress(v0, "GlobalMemoryStatusEx")) != 0)
    {
        statusEx.dwLength = 64;
        ((void(__stdcall*)(_MEMORYSTATUSEX*))v1)(&statusEx);
        if (statusEx.ullAvailVirtual < 0x8000000)
        {
            v2 = Win_LocalizeRef("WIN_LOW_MEMORY_TITLE");
            v3 = Win_LocalizeRef("WIN_LOW_MEMORY_BODY");
            v4 = GetActiveWindow();
            if (MessageBoxA(v4, v3, v2, 0x34u) != 6)
                exit(0);
        }
        v5 = (double)statusEx.ullTotalPhys * 0.00000095367432;
        v6 = (signed int)(v5 + 0.4999999990686774);
        if ((double)statusEx.ullTotalPhys > (double)v6 * 1048576.0 || v6 > 4096)
            v6 = 4096;
        result = v6;
    }
    else
    {
        status.dwLength = 32;
        GlobalMemoryStatus(&status);
        if (status.dwAvailVirtual < 0x8000000)
        {
            v8 = Win_LocalizeRef("WIN_LOW_MEMORY_TITLE");
            v9 = Win_LocalizeRef("WIN_LOW_MEMORY_BODY");
            v10 = GetActiveWindow();
            if (MessageBoxA(v10, v9, v8, 0x34u) != 6)
                exit(0);
        }
        v11 = (double)status.dwTotalPhys * 0.00000095367432;
        v12 = (signed int)(v11 + 0.4999999990686774);
        if ((double)status.dwTotalPhys > (double)v12 * 1048576.0 || v12 > 4096)
            v12 = 4096;
        result = v12;
    }
    return result;
}

void Sys_DetectVideoCard(int descLimit, char* description)
{
    IDXGIAdapter* adapter;
    IDXGIFactory* factory;
    DXGI_ADAPTER_DESC AdapterDesc;

    if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory) >= 0)
    {
        if (factory->EnumAdapters(0, &adapter) >= 0)
        {
            adapter->GetDesc(&AdapterDesc);
            adapter->Release();
            wcstombs(description, AdapterDesc.Description, descLimit);
        }
        factory->Release();
    }
}

void Sys_CopyCpuidString(unsigned int maxLen, char* dest, const char* source)
{
    strcpy_s(dest, maxLen, source);
}

void Sys_DetectCpuVendorAndName(char* vendor, char* name)
{
    int CPUInfo[4] = { -1 };
    unsigned nExIds, i = 0;
    int regs[4] = { 0 };

    // Get the information associated with each extended ID.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string
        if (i == 0x80000002)
            memcpy(name, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(name + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(name + 32, CPUInfo, sizeof(CPUInfo));
    }

    __cpuid(regs, 0);              // mov eax,0; cpuid
    memcpy(vendor, &regs[1], 4);   // copy EBX
    memcpy(vendor + 4, &regs[3], 4); // copy EDX
    memcpy(vendor + 8, &regs[2], 4); // copy ECX
    vendor[12] = '\0';
}

void Sys_GetPhysicalCpuCount(SysInfo* sysInfo)
{
    sysInfo->physicalCpuCount = sysInfo->logicalCpuCount;
}

long double Sys_BenchmarkGHz()
{
    HANDLE threadInfo;
    int priority;
    LARGE_INTEGER qwWait, qwStart, qwCurrent;

    threadInfo = GetCurrentThread();
    priority = GetThreadPriority(threadInfo);
    SetThreadPriority(threadInfo, 15);

    QueryPerformanceCounter(&qwStart);
    QueryPerformanceFrequency(&qwWait);
    qwWait.QuadPart >>= 5;
    unsigned __int64 Start = __rdtsc();
    do
    {
        QueryPerformanceCounter(&qwCurrent);
    } while (qwCurrent.QuadPart - qwStart.QuadPart < qwWait.QuadPart);
    SetThreadPriority(threadInfo, priority);
    return ((__rdtsc() - Start) << 5) / 1000000.0;
}

void Sys_SetAutoConfigureGHz(SysInfo* sysInfo)
{
    int v1; // eax
    int v2; // eax
    double v3; // xmm0_8

    Sys_GetPhysicalCpuCount(sysInfo);
    v1 = sysInfo->physicalCpuCount;
#ifdef _DEBUG
    if (v1 < 1
        && !Assert_MyHandler(
            "c:\\t6\\code\\src_noserver\\win32\\win_configure.cpp",
            457,
            0,
            "(sysInfo->physicalCpuCount) >= (1)",
            "sysInfo->physicalCpuCount >= 1\n\t%i, %i",
            v1,
            1))
    {
        __debugbreak();
    }
#endif // _DEBUG
    v2 = sysInfo->physicalCpuCount;
    if (v2 == 1)
    {
        v3 = DOUBLE_1_0;
    }
    else if (v2 == 2)
    {
        v3 = DOUBLE_1_75;
    }
    else
    {
        v3 = DOUBLE_2_0;
    }
    sysInfo->configureGHz = Sys_BenchmarkGHz() * v3;
}
