#include "win_syscon.h"
#include "win_main.h"
#include "win_localize.h"
#include "win_net.h"

#include "../qcommon/common.h"
#include "../qcommon/threads.h"

void Conbuf_AppendText(const char* pMsg)
{
	const char* v1; // ecx
	char* v2; // edi
	char target; // [esp+8h] [ebp-8004h]

#ifdef _DEBUG
	if (!s_wcd.hwndBuffer
		&& !(unsigned char)Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(s_wcd.hwndBuffer)",
			&scratch))
	{
		__debugbreak();
	}
#endif

	if (strlen(pMsg) <= 0x3FFF)
		v1 = pMsg;
	else
		v1 = &pMsg[strlen(pMsg) - 0x3FFF];
	v2 = Conbuf_CleanText(v1, &target, 0x8000);
	s_totalChars += (unsigned int)v2;
	if (s_totalChars <= 0x4000)
	{
		SendMessageA(s_wcd.hwndBuffer, 0xB1u, 0xFFFFu, 0xFFFF);
	}
	else
	{
		SendMessageA(s_wcd.hwndBuffer, 0xB1u, 0, -1);
		s_totalChars = (unsigned int)v2;
	}
	SendMessageA(s_wcd.hwndBuffer, 0xB6u, 0, 0xFFFF);
	SendMessageA(s_wcd.hwndBuffer, 0xB7u, 0, 0);
	SendMessageA(s_wcd.hwndBuffer, 0xC2u, 0, (LPARAM)&target);
}

void Conbuf_AppendTextInMainThread(const char* msg)
{
	if (s_wcd.hwndBuffer) {
		if ((unsigned char)Sys_IsMainThread())
			Conbuf_AppendText(msg);
	}
}

char* Conbuf_CleanText(const char* source, char* target, int sizeofTarget)
{
	char* v3; // esi
	char v4; // dl
	char* i; // edi
	char v6; // bl

	v3 = &target[sizeofTarget - 3];
	v4 = *source;
	for (i = target; *source; v4 = *source)
	{
		if (target > v3)
			break;
		if (v4 != 10 || source[1] != 13)
		{
			if (v4 == 13)
			{
				*target = 13;
				target[1] = 10;
				target += 2;
			LABEL_17:
				++source;
				continue;
			}
			if (v4 == 10)
			{
				*target = 13;
				target[1] = 10;
				target += 2;
				goto LABEL_17;
			}
			if (v4 != 94 || (v6 = source[1]) == 0 || v6 == 94 || v6 < 48 || v6 > 57)
			{
				*target++ = v4;
				goto LABEL_17;
			}
			source += 2;
		}
		else
		{
			*target = 13;
			target[1] = 10;
			target += 2;
			source += 2;
		}
	}
	*target = 0;
	return (char*)(target - i);
}

LRESULT ConWndProc(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, int lParam)
{
	LRESULT result;
	switch (uMsg) {
	case WM_SIZE:
		SetWindowPos(s_wcd.hwndBuffer, 0, 5, 70, (unsigned short)lParam - 15, ((unsigned int)lParam >> 16) - 100, 0);
		SetWindowPos(
			s_wcd.hwndInputLine,
			0,
			5,
			((unsigned int)lParam >> 16) - 100 + 78,
			(unsigned short)lParam - 15,
			20,
			0);
		s_wcd.windowWidth = (unsigned short)lParam;
		s_wcd.windowHeight = (unsigned int)lParam >> 16;
		result = DefWindowProcA(hWnd, uMsg, wParam, lParam);
		break;
	case WM_ACTIVATE:
		if ((unsigned short)wParam)
			SetFocus(s_wcd.hwndInputLine);
		result = DefWindowProcA(hWnd, uMsg, wParam, lParam);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		result = 0;
		break;
	}
	return result;
}

LRESULT InputLineWndProc(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, int lParam)
{
	char displayBuffer[1024];
	char inputBuffer[1024];

	if (uMsg == 8)
	{
		if ((HWND__*)wParam == s_wcd.hWnd)
		{
			SetFocus(hWnd);
			return 0;
		}
	}
	else if (uMsg == 258 && wParam == 13)
	{
		GetWindowTextA(s_wcd.hwndInputLine, inputBuffer, 1024);
		strncat(s_wcd.consoleText, inputBuffer, 507 - strlen(s_wcd.consoleText));
		strcat(s_wcd.consoleText, "\n");
		SetWindowTextA(s_wcd.hwndInputLine, (LPCSTR)&scratch);
		Com_sprintf(displayBuffer, 1024, "]%s\n", inputBuffer);
		Sys_Print(displayBuffer);
		return 0;
	}
	return CallWindowProcA((WNDPROC)s_wcd.SysInputLineWndProc, hWnd, uMsg, wParam, lParam);
}

void Sys_CreateConsole(HINSTANCE__* hInstance) {
	WNDCLASSA WndClass;
	tagRECT Rect;
	HDC hDC;

	char text;
	char target;

	WndClass.style = 0;
	WndClass.lpfnWndProc = (WNDPROC)ConWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIconA(hInstance, (LPCSTR)1);
	WndClass.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
	WndClass.hbrBackground = (HBRUSH__*)5;
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = "CoD Black Ops II WinConsole";
	if (RegisterClassA(&WndClass))
	{
		Rect.left = 0;
		Rect.right = 620;
		Rect.top = 0;
		Rect.bottom = 450;
		AdjustWindowRect(&Rect, 0x80CA0000, 0);
		hDC = (HDC)GetDeviceCaps(GetDC(GetDesktopWindow()), 8);
		ReleaseDC(GetDesktopWindow(), GetDC(GetDesktopWindow()));
		s_wcd.windowWidth = Rect.right - Rect.left + 1;
		s_wcd.windowHeight = Rect.bottom - Rect.top + 1;
		s_wcd.hWnd = CreateWindowExA(
			0,
			"CoD Black Ops II WinConsole",
			"CoD Black Ops II Server",
			0x80CA0000,
			(signed int)(hDC - 150) / 2,
			(GetDeviceCaps(GetDC(GetDesktopWindow()), 10) - 450) / 2,
			Rect.right - Rect.left + 1,
			Rect.bottom - Rect.top + 1,
			0,
			0,
			hInstance,
			0);
		if (s_wcd.hWnd)
		{
			hDC = GetDC(s_wcd.hWnd);
			int var1 = MulDiv(8, GetDeviceCaps(GetDC(s_wcd.hWnd), 90), 72);
			s_wcd.hfBufferFont = CreateFontA(-var1, 0, 0, 0, 300, 0, 0, 0, 1u, 0, 0, 0, 0x31u, "Courier New");
			ReleaseDC(s_wcd.hWnd, hDC);
			HANDLE logoLoad = LoadImageA(0, "codlogo.bmp", 0, 0, 0, 0x10u);
			if (logoLoad)
			{
				HWND codlogo = CreateWindowExA(0, "Static", 0, 0x5000000Eu, 5, 5, 0, 0, s_wcd.hWnd, (HMENU)1, hInstance, 0);
				s_wcd.codLogo = codlogo;
				SendMessageA(codlogo, 0x172u, 0, (LPARAM)logoLoad);
			}
			s_wcd.hwndInputLine = CreateWindowExA(
				0,
				"edit",
				0,
				0x50800080u,
				6,
				400,
				608,
				20,
				s_wcd.hWnd,
				(HMENU)0x65,
				hInstance,
				0);
			s_wcd.hwndBuffer = CreateWindowExA(
				0,
				"edit",
				0,
				0x50A00844u,
				6,
				70,
				606,
				324,
				s_wcd.hWnd,
				(HMENU)0x64,
				hInstance,
				0);
			SendMessageA(s_wcd.hwndBuffer, 0x30u, (WPARAM)s_wcd.hfBufferFont, 0);
			s_wcd.SysInputLineWndProc = (HWND*)SetWindowLongA(
				s_wcd.hwndInputLine,
				-4,
				(LONG)InputLineWndProc);
			SendMessageA(s_wcd.hwndInputLine, 0x30u, (WPARAM)s_wcd.hfBufferFont, 0);
			SetFocus(s_wcd.hwndInputLine);
			Con_GetTextCopy(&text, 0x4000);
			Conbuf_CleanText(&text, &target, 0x4000);
			SetWindowTextA(s_wcd.hwndBuffer, &target);
		}
	}
}

void Sys_DestroyConsole()
{
	if (s_wcd.hWnd)
	{
		ShowWindow(s_wcd.hWnd, 0);
		CloseWindow(s_wcd.hWnd);
		DestroyWindow(s_wcd.hWnd);
		s_wcd.hWnd = 0;
	}
}

void Sys_SetErrorText(const char* buf)
{
	const char* v1; // ST08_4
	HWND v2; // eax

	I_strncpyz(s_wcd.errorString, buf, 512);
	DestroyWindow(s_wcd.hwndInputLine);
	s_wcd.hwndInputLine = 0;
	ShowCursor(1);
	v1 = Win_LocalizeRef("WIN_ERROR");
	v2 = GetActiveWindow();
	MessageBoxA(v2, buf, v1, 0x10u);
}

void Sys_ShowConsole()
{
	if (!s_wcd.hWnd) {
		Sys_CreateConsole(GetModuleHandleA(0));
		#ifdef _DEBUG
		if (!s_wcd.hWnd
			&& !(unsigned __int8)Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(s_wcd.hWnd)",
				&scratch))
		{
			__debugbreak();
		}
		#endif
	}
	ShowWindow(s_wcd.hWnd, 1);
	SendMessageA(s_wcd.hwndBuffer, 0xB6u, 0, 0xFFFF);
	ShowCursor(1);
}
