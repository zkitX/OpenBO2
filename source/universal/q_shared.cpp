#include <direct.h>

#include "q_shared.h"

#include "../qcommon/common.h"
#include "../qcommon/threads.h"

struct _iobuf
{
    char* _ptr;
    int _cnt;
    char* _base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char* _tmpfname;
};

void I_strncpyz(char* dest, const char* src, int destsize)
{
    const char* v3; // eax
    int i; // edi
    char v5; // cl
#ifdef _DEBUG
    if (!src
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(src)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (!dest
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(dest)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (destsize < 1
        && !(unsigned char)assertive::Assert_MyHandler(
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

int I_stricmp(const char* s0, const char* s1)
{
#ifdef _DEBUG
    if (!s0
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(s0)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (!s1
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(s1)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
#endif // _DEBUG
    return I_strnicmp(s0, s1, 0x7FFFFFFF);
}

int I_strncmp(const char* s0, const char* s1, int n)
{
    const char* v3; // esi
    int v4; // ecx
    int v5; // eax
    int v6; // edx
    int v7; // ebx

    if (!s0
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(s0)",
            &pBlock))
    {
        __debugbreak();
    }
    v3 = s1;
    if (!s1
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            (int)s1,
            "(s1)",
            &pBlock))
    {
        __debugbreak();
    }
    if (!s0 || !s1)
        return s1 - s0;
    v4 = n;
    while (1)
    {
        v5 = v3[s0 - s1];
        v6 = *v3;
        v7 = v4;
        ++v3;
        --v4;
        if (!v7)
            return 0;
        if (v5 != v6)
            break;
        if (!v5)
            return 0;
    }
    return 2 * (v5 >= v6) - 1;
}

int I_strnicmp(const char* s0, const char* s1, int n)
{
    const char* v3; // esi
    int v4; // edx
    int v5; // eax
    int v6; // ecx
    int v7; // ebx
    #ifdef _DEBUG
    if (!s0
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(s0)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    #endif
    v3 = s1;
    #ifdef _DEBUG
    if (!s1
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            (int)s1,
            "(s1)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    #endif
    if (!s0 || !s1)
        return s1 - s0;
    v4 = n;
    while (1)
    {
        v5 = (unsigned char)v3[s0 - s1];
        v6 = *(unsigned char*)v3;
        v7 = v4;
        ++v3;
        --v4;
        if (!v7)
            return 0;
        if (v5 != v6)
        {
            if ((unsigned int)(v5 - 65) <= 0x19)
                v5 += 32;
            if ((unsigned int)(v6 - 65) <= 0x19)
                v6 += 32;
            if (v5 != v6)
                break;
        }
        if (!v5)
            return 0;
    }
    return 2 * (v5 >= v6) - 1;
}

bool Com_BitCheckAssert(const unsigned int* array, int bitNum, int size)
{
    if (!array
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(array)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (bitNum >= (unsigned int)(8 * size)
        && !(unsigned char)assertive::Assert_MyHandler(
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

int Com_sprintf(char* dest, int size, const char* fmt, ...)
{
    int result; // eax
    va_list ap; // [esp+1Ch] [ebp+14h]

    va_start(ap, fmt);
    result = _vsnprintf(dest, size, fmt, ap);
    dest[size - 1] = 0;
    return result;
}

const char* I_stristr(const char* s0, const char* substr)
{
    const char* v2; // edi
    signed int v3; // esi
    int v4; // ebx
    bool v5; // zf
    int s0Char; // [esp+8h] [ebp-4h]
    const char* substra; // [esp+18h] [ebp+Ch]

    if (!s0
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(s0)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    v2 = substr;
    if (!substr
        && !(unsigned char)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            (int)substr,
            "(substr)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
    if (!s0 || !substr)
        return 0;
    s0Char = 0;
    if (*s0)
    {
        substra = s0;
        while (2)
        {
            v3 = -1;
            do
            {
                if (!v2[++v3])
                    return &s0[s0Char];
                v4 = tolower(substra[v3]);
            } while (v4 == tolower(v2[v3]));
            ++s0Char;
            v5 = (substra++)[1] == 0;
            if (!v5)
                continue;
            break;
        }
    }
    return 0;
}

void Sys_MkdirEx(const char* _path)
{
    signed int v1; // kr00_4
    signed int i; // esi
    char v3; // bl
    char path[1024]; // [esp+0h] [ebp-404h]

    strcpy(path, _path);
    v1 = strlen(path);
    for (i = 0; i < v1; ++i)
    {
        v3 = path[i];
        if (v3 == 47 || v3 == 92)
        {
            path[i] = 0;
            _mkdir(path);
            path[i] = v3;
        }
    }
}

char* va(const char* format, ...)
{
    unsigned int* v1; // eax
    int v2; // ecx
    char* v3; // esi
    int v4; // eax
    va_list ap; // [esp+10h] [ebp+Ch]

    va_start(ap, format);
    v1 = (unsigned int*)Sys_GetValue(1);
    v2 = v1[1024];
    v3 = (char*)&v1[256 * v2];
    v1[1024] = (v2 + 1) % 4;
    v4 = _vsnprintf(v3, 0x400u, format, ap);
    v3[1023] = 0;
    if (v4 < 0 || v4 >= 1024)
    {
        v3[1023] = 0;
        Com_Error(ERR_DROP, "Attempted to overrun string in call to va(): '%s'", v3);
    }
    return v3;
}
