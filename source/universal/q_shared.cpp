#include "q_shared.h"
#include "assertive.h"

#include "../qcommon/common.h"

void q_shared::I_strncpyz(char* dest, const char* src, int destsize)
{
    const char* v3; // eax
    int i; // edi
    char v5; // cl
#ifdef _DEBUG
    if (!src
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(src)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (!dest
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(dest)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (destsize < 1
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((destsize >= 1))",
            "(destsize) = %i",
            destsize))
    {
        __debugbreak();
    }
#endif // _DEBUG
    strncpy(dest, src, destsize-1);
}

bool Com_BitCheckAssert(const unsigned int* array, int bitNum, int size)
{
    if (!array
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(array)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (bitNum >= (unsigned int)(8 * size)
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(bitNum) < (unsigned)(size * 8)",
            "bitNum doesn't index size * 8\n\t%i not in [0, %i)",
            bitNum,
            8 * size))
    {
        __debugbreak();
    }
    return (array[bitNum >> 5] & (1 << (bitNum & 0x1F))) != 0;
}

char* va(const char* format, ...)
{
    _DWORD* v1; // eax
    int v2; // ecx
    char* v3; // esi
    int v4; // eax
    va_list ap; // [esp+10h] [ebp+Ch]

    va_start(ap, format);
    v1 = Sys_GetValue(1);
    v2 = v1[1024];
    v3 = (char*)&v1[256 * v2];
    v1[1024] = (v2 + 1) % 4;
    v4 = _vsnprintf(v3, 0x400u, format, ap);
    v3[1023] = 0;
    if (v4 < 0 || v4 >= 1024)
    {
        v3[1023] = 0;
        common::Com_Error(common::ERR_DROP, &byte_C80B84, v3);
    }
    return v3;
}
