#pragma once
#include <Windows.h>

#include "../defs.h"

struct WinConData
{
	HWND__* hWnd;
	HWND__* hwndBuffer;
	HWND__* codLogo;
	HFONT__* hfBufferFont;
	HWND__* hwndInputLine;
	char errorString[512];
	char consoleText[512];
	char returnedText[512];
	int windowWidth;
	int windowHeight;
	int(* SysInputLineWndProc)(HWND__*, unsigned int, unsigned int, int);
};

int enable_OutputDebugString = 1;

char s_syslogBuffer[1400];

unsigned int s_totalChars;

WinConData s_wcd;

void Conbuf_AppendText(const char* pMsg);
void Conbuf_AppendTextInMainThread(const char* msg);
char* Conbuf_CleanText(const char* source, char* target, int sizeofTarget);

LRESULT ConWndProc(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, int lParam);
LRESULT InputLineWndProc(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, int lParam);

void Sys_CreateConsole(HINSTANCE__* hInstance);
void Sys_Print(const char* msg);