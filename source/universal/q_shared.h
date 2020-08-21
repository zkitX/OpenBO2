#pragma once

#include "../qcommon/common.h"
#include <cstring>

bool Com_BitCheckAssert(const unsigned int* array, int bitNum, int size);

const char* I_stristr(const char* s0, const char* substr);
void* Sys_GetValue(int valueIndex);
void Sys_MkdirEx(const char* _path);

char* va(const char* format, ...);

class q_shared
{
public:
	// _________________________________________________________________________________
	// Structure definitions
	// _________________________________________________________________________________

	// _________________________________________________________________________________
	// Public variable declarations
	// _________________________________________________________________________________

	// _________________________________________________________________________________
	// Function declarations
	// _________________________________________________________________________________
	static void I_strncpyz(char* dest, const char* src, int destsize);
	static int I_stricmp(const char* s0, const char* s1);
	static int I_strnicmp(const char* s0, const char* s1, int n);
private:
	// _________________________________________________________________________________
	// Private variable declarations
	// _________________________________________________________________________________

};

