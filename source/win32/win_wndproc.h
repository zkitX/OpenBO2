#pragma once

#include <Windows.h>

struct WinVars_t
{
	HINSTANCE__* reflib_library;
	int reflib_active;
	HWND__* hWnd;
	HINSTANCE__* hInstance;
	int activeApp;
	int isMinimized;
	int recenterMouse;
	HHOOK__* lowLevelKeyboardHook;
	unsigned int sysMsgTime;
} g_wv;

int g_blockHook;