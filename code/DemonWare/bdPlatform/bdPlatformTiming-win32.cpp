#include <pch.h>

void bdPlatformTiming::sleep(unsigned int)
{
}

_LARGE_INTEGER bdPlatformTiming::getHiResTimeStamp(void)
{
    _LARGE_INTEGER time; // [esp+0h] [ebp-8h]

    QueryPerformanceCounter(&time);
    return time;
}

float bdPlatformTiming::getElapsedTime(unsigned __int64 time1, unsigned __int64 time2)
{
    _LARGE_INTEGER frequency; // [esp+0h] [ebp-8h]

    QueryPerformanceFrequency(&frequency);
    return (float)((double)(__int64)(time2 - time1) / (double)frequency.QuadPart);
}

unsigned int bdPlatformTiming::getLoResTimeStamp(void)
{
    return GetTickCount();
}

unsigned int bdPlatformTiming::getLoResElapsedTime(unsigned int time1, unsigned int time2)
{
    unsigned int v2; // ecx
    unsigned int v3; // eax

    v2 = time2;
    v3 = 0;
    if (time2 < time1)
    {
        v3 = ((unsigned __int64)time2 + 0xFFFFFFFF) >> 32;
        v2 = time2 - 1;
    }
    return (__PAIR64__(v3, v2) - time1) / 0x3E8;
}
