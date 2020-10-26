#include "pch.h"

_DWORD* bdAlignedOffsetMalloc(const unsigned int size, const unsigned int align, const unsigned int offset)
{
    _DWORD* result; // eax
    unsigned int v4; // edi
    _DWORD* v5; // ecx

    if ((align - 1) & align)
        return 0;
    v4 = align + offset + 4;
    result = (_DWORD*)malloc(v4 + size);
    v5 = result;
    if (result)
    {
        result = (_DWORD*)((~(align - 1) & ((unsigned int)result + v4)) - offset);
        *(result - 1) = (_DWORD)v5;
    }
    return result;
}

void bdAlignedOffsetFree(void* p)
{
    free(*((void**)p - 1));
}

char* bdAlignedOffsetRealloc(void* p, const unsigned int origSize, const unsigned int size, const unsigned int align, const unsigned int offset)
{
    unsigned int v5; // esi
    char* v6; // edi

    v5 = size;
    v6 = (char*)bdAlignedOffsetMalloc(size, align, offset);
    if (size >= origSize)
        v5 = origSize;
    memcpy(v6, (char*)p, v5);
    bdAlignedOffsetFree(p);
    return v6;
}