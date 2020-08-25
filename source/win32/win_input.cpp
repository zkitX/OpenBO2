#include "win_input.h"

#include "../defs.h"
#include "win_main.h"
#include "win_wndproc.h"

#include "../gfx_d3d/r_dvars.h"

#include "../universal/assertive.h"

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

int IN_DeactivateWin32Mouse()
{
	int result;

	g_showCursor = 1;
	for (result = ShowCursor(1); result; result = ShowCursor(result < 0));
	return result;
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

void IN_RecenterMouse()
{
	tagRECT window_rect;

	GetWindowRect(g_wv.hWnd, &window_rect);
	window_center_x = (window_rect.right + window_rect.left) / 2;
	window_center_y = (window_rect.top + window_rect.bottom) / 2;
	SetCursorPos((window_rect.right + window_rect.left) / 2, (window_rect.top + window_rect.bottom) / 2);
}

void IN_ShowSystemCursor(int show)
{
	int v1;
	int i;

	g_showCursor = show;
	v1 = (show != 0) - 1;
	for (i = ShowCursor(show); i != v1; i = ShowCursor(i < v1));
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
