#pragma once

_DWORD* bdAlignedOffsetMalloc(const unsigned int size, const unsigned int align, const unsigned int offset);
void bdAlignedOffsetFree(void* p);
char* bdAlignedOffsetRealloc(void* p, const unsigned int origSize, const unsigned int size, const unsigned int align, const unsigned int offset);