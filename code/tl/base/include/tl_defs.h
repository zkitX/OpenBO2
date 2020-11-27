#pragma once
#include "tl_system.h"

inline void tlEndianSwapMemory32(char* Data, unsigned int Bytes) {
	unsigned int i;

	if (Bytes & 3 && _tlAssert(__FILE__, __LINE__, "(Bytes&3) == 0)", "Size must be a multiple of 4"))
		__debugbreak();
	for (i = Bytes >> 2; i; --i)
	{
		*Data = (((*Data << 16) | *Data & 0xFF00) << 8) | (((*Data >> 16) | *Data & 0xFF0000u) >> 8);
		++Data;
	}
}