#pragma once

#include <Windows.h>

// _________________________________________________________________________________
// Structure declarations
// _________________________________________________________________________________


// _________________________________________________________________________________
// Enumeration declarations
// _________________________________________________________________________________

enum IdentityParam
{
	IDENTITY_ID = 0x0,
	IDENTITY_TITLE_ID = 0x1,
	IDENTITY_AUTH_ADDR = 0x2,
	IDENTITY_LSG_ADDR = 0x3,
	IDENTITY_DW_KEY = 0x4,
	IDENTITY_ENVIRONMENT = 0x5,
	IDENTITY_PLATFORM = 0x6,
	IDENTITY_PARAM_COUNT = 0x7,
};

// _________________________________________________________________________________
// Functions declarations
// _________________________________________________________________________________

int PrivateUnhandledExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);

void Sys_Error(const char* error, ...);
char* Sys_GetIdentityParam(IdentityParam p);
void Sys_OutOfMemErrorInternal(const char* filename, int line);

// Startup function
int __stdcall WinMain(HINSTANCE__* hInstance, HINSTANCE__* hPrevInstance, char* lpCmdLine, int nCmdShow);

char* Win_GetTheOtherExeName(const char* mode);

// _________________________________________________________________________________
// END
// _________________________________________________________________________________