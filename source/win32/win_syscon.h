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
	int(*SysInputLineWndProc)(HWND__*, unsigned int, unsigned int, int);
};

struct Message
{
	int startTime;
	int endTime;
};

struct MessageLine
{
	int messageIndex;
	int textBufPos;
	int textBufSize;
	int typingStartTime;
	int lastTypingSoundTime;
	int flags;
};

struct MessageWindow
{
	MessageLine* lines;
	Message* messages;
	char* circularTextBuffer;
	int textBufSize;
	int lineCount;
	int padding;
	int scrollTime;
	int fadeIn;
	int fadeOut;
	int textBufPos;
	int firstLineIndex;
	int activeLineCount;
	int messageIndex;
};

int enable_OutputDebugString = 1;

unsigned int s_totalChars;

WinConData s_wcd;
MessageWindow msgwnd;

void Conbuf_AppendText(const char* pMsg);
void Conbuf_AppendTextInMainThread(const char* msg);
char* Conbuf_CleanText(const char* source, char* target, int sizeofTarget);
void Con_GetTextCopy(char* text, int maxSize);

LRESULT ConWndProc(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, int lParam);
LRESULT InputLineWndProc(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, int lParam);

void Sys_CreateConsole(HINSTANCE__* hInstance);
void Sys_Print(const char* msg);
void Sys_ShowConsole();