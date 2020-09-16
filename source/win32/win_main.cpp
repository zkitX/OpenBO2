#include "win_main.h"
#include "win_localize.h"
#include "win_net.h"
#include "timing.h"

#include <qcommon/threads.h>
#include <universal/assertive.h>
#include <universal/blackbox.h>
#include <universal/com_memory.h>
#include <universal/win_common.h>

void Sys_FindInfo()
{
	sys_info.logicalCpuCount = Sys_GetCpuCount();
	sys_info.cpuGHz = 1.0 / (((double)1i64 - (double)0i64) * msecPerRawTimerTick * 1000000.0);
	sys_info.sysMB = Sys_SystemMemoryMB();
	Sys_DetectVideoCard(512, sys_info.gpuDescription);
	Sys_DetectCpuVendorAndName(sys_info.cpuVendor, sys_info.cpuName);
	Sys_SetAutoConfigureGHz(&sys_info);
}

void Sys_OutOfMemErrorInternal(const char* filename, int line)
{
	ShowCursor(1);
	Sys_EnterCriticalSection(CRITSECT_FATAL_ERROR);
	BB_Alert(__FILE__, __LINE__, "error", "Out of Memory");
	Com_Printf(10, "Out of memory: filename '%s', line %d\n", filename, line);
	MessageBoxA(GetActiveWindow(), Win_LocalizeRef("WIN_OUT_OF_MEM_BODY"), Win_LocalizeRef("WIN_OUT_OF_MEM_TITLE"), 0x10u);
	//LiveStream_Shutdown()
	exit(-1);
}

void Sys_QuitAndStartProcess(const char* exeName, const char* parameters)
{
	LocalClientNum_t v2; // eax
	char pathOrig[260]; // [esp+8h] [ebp-108h]

	GetCurrentDirectoryA(0x104u, pathOrig);
	if (parameters)
		Com_sprintf(sys_exitCmdLine, 1024, "\"%s\\%s\" %s", pathOrig, exeName, parameters);
	else
		Com_sprintf(sys_exitCmdLine, 1024, "\"%s\\%s\"", pathOrig, exeName);
	v2 = Com_LocalClients_GetPrimary();
	Cbuf_AddText(v2, "quit\n");
}

void Sys_SpawnQuitProcess()
{
	_STARTUPINFOA dst;
	_PROCESS_INFORMATION pi;
	void* msgBuf;

	if (sys_exitCmdLine[0])
	{
		memset((char*)&dst, 0, 0x44u);
		dst.cb = 68;
		if (!CreateProcessA(0, sys_exitCmdLine, 0, 0, 0, 0, 0, 0, &dst, &pi))
		{
			FormatMessageA(0x1300u, 0, GetLastError(), 0x400u, (LPSTR)&msgBuf, 0, 0);
			Com_Error(ERR_FATAL, "%s '%s' %s(0x^08x)", Win_LocalizeRef("EXE_COULDNT_START_PROCESS"), sys_exitCmdLine, msgBuf, GetLastError());
		}
	}
}

void Sys_Error(const char* error, ...)
{
	tagMSG Msg; // [esp+Ch] [ebp-1020h]
	char string; // [esp+28h] [ebp-1004h]
	va_list ap; // [esp+1038h] [ebp+Ch]

	va_start(ap, error);
	_vsnprintf(&string, 0x1000u, error, ap);
	Com_PrintStackTrace(0, 0);
	Com_PrintError(10, "\nSys_Error: %s\n\n", &string);
	BB_Alert(__FILE__, __LINE__, "sys_error", &string);
	FixWindowsDesktop();
	if (IsDebuggerConnected())
		__debugbreak();
	SV_SysLog_LogMessage(0, &string);
	SV_SysLog_ForceFlush();
	if (Sys_IsMainThread())
	{
		//LiveSteam_Shutdown();
		Sys_ShowConsole();
		Conbuf_AppendText("\n\n");
		Conbuf_AppendText(&string);
		Conbuf_AppendText("\n");
		Sys_SetErrorText(&string);
		while (GetMessageA(&Msg, 0, 0, 0))
		{
			TranslateMessage(&Msg);
			DispatchMessageA(&Msg);
		}
		exit(0);
	}
	if (Sys_IsServerThread())
		Sys_ServerCompleted();
	I_strncpyz(errPtr, &string, 4096);
	if (Sys_IsMainThread())
	{
		if (errPtr[0])
			Sys_Error(errPtr);
		Sys_Error("Error quit was requested in the main thread\n");
	}
	g_quitRequested = 1;
	while (1)
		NET_Sleep(0x64u);
}

void Sys_Print(const char* msg)
{
	if (enable_OutputDebugString)
		OutputDebugStringA(msg);
	Conbuf_AppendTextInMainThread(msg);
	SV_SysLog_LogMessage(0, msg);
}

char* Sys_GetClipboardData()
{
	char* result;
	HANDLE clipBoardData;
	void* clipBoardDataPointer;
	const char* memLock;
	SIZE_T clipboardSize;
	int destSize;

	result = 0;
	if (OpenClipboard(0))
	{
		clipBoardData = GetClipboardData(1u);
		clipBoardDataPointer = clipBoardData;
		if (clipBoardData)
		{
			memLock = (const char*)GlobalLock(clipBoardData);
			if (memLock)
			{
				clipboardSize = GlobalSize(clipBoardDataPointer);
				result = (char*)Z_Malloc(clipboardSize + 1, "Sys_GetClipboardData", 12);
				destSize = GlobalSize(clipBoardDataPointer);
				I_strncpyz(result, memLock, destSize);
				GlobalUnlock(clipBoardDataPointer);
				strtok(result, "\n\r\b");
			}
		}
		CloseClipboard();
	}
	return result;
}

void Sys_FreeClipboardData(char* text)
{
	Z_Free(text, 12);
}

void Sys_QueEvent(int time, sysEventType_t type, int value, int value2, int ptrLength, void* ptr)
{
	sysEvent_t* ev;
	Sys_EnterCriticalSection(CRITSECT_SYS_EVENT_QUEUE);
	ev = &eventQue[(unsigned __int8)eventHead];
	if (eventHead - eventTail >= 256)
	{
		Com_Printf(16, "Sys_QueEvent: overflow\n");
		if (ev->evPtr)
			Z_Free(ev->evPtr, 11);
		++eventTail;
	}
	++eventHead;
	if (!time)
		time = Sys_Milliseconds();
	ev->evTime = time;
	ev->evType = type;
	ev->evValue = value;
	ev->evValue2 = value2;
	ev->evPtrLength = ptrLength;
	ev->evPtr = ptr;
	Sys_LeaveCriticalSection(CRITSECT_SYS_EVENT_QUEUE);
}

void Sys_ShutdownEvents()
{
	sysEvent_t* ev; // [esp+0h] [ebp-4h]

	Sys_EnterCriticalSection(CRITSECT_SYS_EVENT_QUEUE);
	while (eventHead > eventTail)
	{
		ev = &eventQue[eventTail++];
		if (ev->evPtr)
			Z_Free(ev->evPtr, 11);
	}
	Sys_LeaveCriticalSection(CRITSECT_SYS_EVENT_QUEUE);
}

sysEvent_t* Win_GetEvent(sysEvent_t* result)
{
	sysEvent_t* v1; // edx
	sysEvent_t* v2; // eax
	char v3; // cl
	char v4; // cl
	int consoleInputLen; // ST18_4
	char* b; // ST28_4
	tagMSG msg; // [esp+18h] [ebp-38h]
	char* s; // [esp+34h] [ebp-1Ch]
	int ev; // [esp+38h] [ebp-18h]
	sysEventType_t v11; // [esp+3Ch] [ebp-14h]
	int v12; // [esp+40h] [ebp-10h]
	int v13; // [esp+44h] [ebp-Ch]
	int v14; // [esp+48h] [ebp-8h]
	void* v15; // [esp+4Ch] [ebp-4h]

	Sys_EnterCriticalSection(CRITSECT_SYS_EVENT_QUEUE);
	if (eventHead <= eventTail)
	{
		if (Sys_QueryWin32QuitEvent())
			Com_Quit_f(v3);
		while (PeekMessageA(&msg, 0, 0, 0, 0))
		{
			if (!GetMessageA(&msg, 0, 0, 0))
				Com_Quit_f(v4);
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		s = Sys_ConsoleInput();
		if (s)
		{
			consoleInputLen = strlen(s);
			b = (char*)Com_AllocEvent(consoleInputLen + 1);
			I_strncpyz(b, s, consoleInputLen);
			Sys_QueEvent(0, SE_CONSOLE, 0, 0, consoleInputLen + 1, b);
		}
		if (eventHead <= eventTail)
		{
			ev = 0;
			v11 = SE_NONE;
			v12 = 0;
			v13 = 0;
			v14 = 0;
			v15 = 0;
			ev = Sys_Milliseconds();
		}
		else
		{
			v1 = &eventQue[(++eventTail - 1)];
			ev = v1->evTime;
			v11 = v1->evType;
			v12 = v1->evValue;
			v13 = v1->evValue2;
			v14 = v1->evPtrLength;
			v15 = v1->evPtr;
		}
		Sys_LeaveCriticalSection(CRITSECT_SYS_EVENT_QUEUE);
		result->evTime = ev;
		result->evType = v11;
		result->evValue = v12;
		result->evValue2 = v13;
		result->evPtrLength = v14;
		result->evPtr = v15;
		v2 = result;
	}
	else
	{
		v1 = &eventQue[(++eventTail - 1)];
		ev = v1->evTime;
		v11 = v1->evType;
		v12 = v1->evValue;
		v13 = v1->evValue2;
		v14 = v1->evPtrLength;
		v15 = v1->evPtr;
		Sys_LeaveCriticalSection(CRITSECT_SYS_EVENT_QUEUE);
		result->evTime = ev;
		result->evType = v11;
		result->evValue = v12;
		result->evValue2 = v13;
		result->evPtrLength = v14;
		result->evPtr = v15;
		v2 = result;
	}
	return v2;
}
