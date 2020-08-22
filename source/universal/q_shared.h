#pragma once

#include "../qcommon/common.h"

bool Com_BitCheckAssert(const unsigned int* array, int bitNum, int size);
int Com_sprintf(char* dest, int size, const char* fmt, ...);

int I_stricmp(const char* s0, const char* s1);
int I_strncmp(const char* s0, const char* s1, int n);
const char* I_stristr(const char* s0, const char* substr);
void I_strncpyz(char* dest, const char* src, int destsize);
int I_strnicmp(const char* s0, const char* s1, int n);

void Sys_MkdirEx(const char* _path);

char* va(const char* format, ...);

class q_shared
{

};

