#include "win_input.h"

#include "../defs.h"
#include "win_main.h"
#include "win_wndproc.h"

#include "../gfx_d3d/r_dvars.h"

#include "../universal/assertive.h"

void IN_GamepadsMove()
{
	DWORD v0; // esi
	double v1; // st7
	int v2; // edi
	double v3; // st7
	int v4; // ebx
	char v5; // bl
	unsigned int v6; // edi
	LocalClientNum_t v7; // eax
	int lx; // [esp+4h] [ebp-18h]
	int ly; // [esp+8h] [ebp-14h]
	signed int rx; // [esp+Ch] [ebp-10h]
	signed int ry; // [esp+10h] [ebp-Ch]
	signed int leftTrig; // [esp+14h] [ebp-8h]
	signed int rightTrig; // [esp+18h] [ebp-4h]

	GPad_UpdateAll();
	v0 = Sys_Milliseconds();
	if (GPad_IsActive(0))
	{
		v1 = GPad_GetStick(0, GPAD_LX) * 65535.0;
		v2 = (signed int)v1;
		lx = (signed int)v1;
		v3 = GPad_GetStick(0, GPAD_LY) * 65535.0;
		v4 = (signed int)v3;
		ly = (signed int)v3;
		rx = (signed int)(GPad_GetStick(0, GPAD_RX) * 65535.0);
		ry = (signed int)(GPad_GetStick(0, GPAD_RY) * 65535.0);
		leftTrig = (signed int)(GPad_GetButton(0, GPAD_L_TRIG) * 65535.0);
		rightTrig = (signed int)(GPad_GetButton(0, GPAD_R_TRIG) * 65535.0);
		CL_GamepadEvent(0, 0, rx, v0);
		CL_GamepadEvent(0, 1, ry, v0);
		CL_GamepadEvent(0, 2, v2, v0);
		CL_GamepadEvent(0, 3, v4, v0);
		CL_GamepadEvent(0, 5, leftTrig, v0);
		CL_GamepadEvent(0, 4, rightTrig, v0);
		v5 = 0;
		v6 = 0;
		do
		{
			if (GPad_IsButtonPressed(0, buttonList[v6 / 8].padButton))
			{
				CL_GamepadButtonEventForPort(0, dword_DD710C[v6 / 4], 1, v0, buttonList[v6 / 8].padButton);
				v5 = 1;
			}
			else if (GPad_ButtonRequiresUpdates(0, buttonList[v6 / 8].padButton))
			{
				CL_GamepadButtonEventForPort(0, dword_DD710C[v6 / 4], 2, v0, buttonList[v6 / 8].padButton);
			}
			else if (GPad_IsButtonReleased(0, buttonList[v6 / 8].padButton))
			{
				CL_GamepadButtonEventForPort(0, dword_DD710C[v6 / 4], 0, v0, buttonList[v6 / 8].padButton);
			}
			v6 += 8;
		} while (v6 < 0x80);
		if (v5 || rightTrig || leftTrig || ry || rx || ly || lx)
		{
			v7 = Com_ControllerIndex_GetLocalClientNum(0);
			Com_LocalClient_LastInput_Set(v7, LAST_INPUT_GAMEPAD);
		}
	}
}

void IN_RecenterMouse()
{
	tagRECT window_rect;

	GetWindowRect(g_wv.hWnd, &window_rect);
	window_center_x = (window_rect.right + window_rect.left) / 2;
	window_center_y = (window_rect.top + window_rect.bottom) / 2;
	SetCursorPos((window_rect.right + window_rect.left) / 2, (window_rect.top + window_rect.bottom) / 2);
}

void IN_ActivateMouse(int force)
{
	if (s_wmv.mouseInitialized)
	{
#ifdef _DEBUG
		if (!r_fullscreen
			&& !assertive::Assert_MyHandler(__FILE__, __LINE__, 0, "(r_fullscreen)", &pBlock))
		{
			__debugbreak();
		}
#endif
		if (Dvar_GetBool(in_mouse))
		{
			if (force || !s_wmv.mouseActive)
				s_wmv.mouseActive = GetForegroundWindow() == g_wv.hWnd;
		}
		else
		{
			s_wmv.mouseActive = 0;
		}
	}
}

void IN_ClampMouseMove(tagPOINT* curPos)
{
	int v1; // edx
	char v2; // cl
	int v3; // eax
	int v4; // edx
	tagRECT rc; // [esp+0h] [ebp-14h]

	GetWindowRect(g_wv.hWnd, &rc);
	v1 = rc.left;
	v2 = 0;
	if (curPos->x >= rc.left)
	{
		if (curPos->x < rc.right)
			goto LABEL_5;
		v1 = rc.right - 1;
	}
	v2 = 1;
	curPos->x = v1;
LABEL_5:
	v3 = curPos->y;
	v4 = rc.top;
	if (v3 >= rc.top)
	{
		if (v3 < rc.bottom)
		{
			if (!v2)
				return;
			goto LABEL_10;
		}
		v4 = rc.bottom - 1;
	}
	curPos->y = v4;
LABEL_10:
	SetCursorPos(curPos->x, curPos->y);
}

HWND IN_MouseMove()
{
	HWND result; // eax
	int v1; // edi
	int v2; // esi
	LocalClientNum_t v3; // eax
	tagPOINT curPos; // [esp+0h] [ebp-8h]

#ifdef _DEBUG
	if (!s_wmv.mouseInitialized
		&& !assertive::Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(s_wmv.mouseInitialized)",
			(const char*)&pBlock))
	{
		__debugbreak();
	}
	if (!r_fullscreen
		&& !assertive::Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(r_fullscreen)",
			(const char*)&pBlock))
	{
		__debugbreak();
	}
#endif
	result = GetForegroundWindow();
	if (result == g_wv.hWnd)
	{
		GetCursorPos(&curPos);
		if (Dvar_GetInt(r_fullscreen) == 1)
			IN_ClampMouseMove(&curPos);
		v1 = curPos.y - s_wmv.oldPos.y;
		v2 = curPos.x - s_wmv.oldPos.x;
		if (curPos.x != s_wmv.oldPos.x || curPos.y != s_wmv.oldPos.y)
		{
			v3 = Com_LocalClients_GetPrimary();
			Com_LocalClient_LastInput_Set(v3, LAST_INPUT_MOUSEMOVE);
		}
		s_wmv.oldPos = curPos;
		ScreenToClient(g_wv.hWnd, &curPos);
		result = (HWND)CL_MouseEvent(0, curPos.x, curPos.y, v2, v1);
		g_wv.recenterMouse = (int)result;
		if (result && (v2 || v1))
		{
			IN_RecenterMouse();
			result = (HWND)window_center_y;
			s_wmv.oldPos.x = window_center_x;
			s_wmv.oldPos.y = window_center_y;
		}
	}
	return result;
}

void IN_SetCursorPos(int x, int y)
{
	tagPOINT curPos;

	curPos.y = y;
	curPos.x = y;
	ClientToScreen(g_wv.hWnd, &curPos);
	SetCursorPos(curPos.x, curPos.y);
	s_wmv.oldPos = curPos;
}

void IN_ShowSystemCursor(int show)
{
	int v1;
	int i;

	g_showCursor = show;
	v1 = (show != 0) - 1;
	for (i = ShowCursor(show); i != v1; i = ShowCursor(i < v1));
}

int IN_DeactivateWin32Mouse()
{
	int result;

	g_showCursor = 1;
	for (result = ShowCursor(1); result; result = ShowCursor(result < 0));
	return result;
}

void IN_Shutdown()
{
	if (s_wmv.mouseInitialized)
	{
		if (s_wmv.mouseActive)
		{
			s_wmv.mouseActive = 0;
			IN_DeactivateWin32Mouse();
		}
	}
}

void IN_Frame()
{
	if (Dvar_GetBool(ClickToContinue))
		PostMessageA(g_wv.hWnd, 0x201u, 1u, 0);
	if (s_wmv.mouseInitialized)
	{
		if (in_appactive)
		{
			IN_ActivateMouse(0);
			IN_MouseMove();
			if (GetForegroundWindow() == g_wv.hWnd)
				IN_GamepadsMove();
		}
		else if (s_wmv.mouseActive)
		{
			s_wmv.mouseActive = 0;
			IN_DeactivateWin32Mouse();
		}
	}
}
