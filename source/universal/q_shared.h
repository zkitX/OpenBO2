#pragma once

#include "../qcommon/common.h"
#include <cstring>

bool Com_BitCheckAssert(const unsigned int* array, int bitNum, int size);

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
private:
	// _________________________________________________________________________________
	// Private variable declarations
	// _________________________________________________________________________________

};
