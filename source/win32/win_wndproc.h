#pragma once

#include "win_main.h"

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

int marker_win_wndproc;
int g_blockHook;