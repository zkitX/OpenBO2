#include "win_shared.h"

#include <Windows.h>

int Sys_Milliseconds()
{
    if (!initialized_1)
    {
        sys_timeBase = timeGetTime();
        initialized_1 = 1;
    }
    return timeGetTime() - sys_timeBase;
}

int Sys_MillisecondsRaw()
{
    return timeGetTime();
}

void Sys_SnapVector(float* v)
{
    *v = (float)(signed int)*v;
    v[1] = (float)(signed int)v[1];
    v[2] = (float)(signed int)v[2];
}
