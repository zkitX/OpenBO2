#include "win_main.h"
#include "win_localize.h"
#include "win_net.h"
#include "win_input.h"
#include "miniDumper.h"
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

void Sys_LoadingKeepAlive()
{
	// TODO
}

sysEvent_t* Sys_GetEvent(sysEvent_t* result)
{
	sysEvent_t* event; // eax
	sysEvent_t v3; // [esp+0h] [ebp-30h]
	int evTime; // [esp+18h] [ebp-18h]
	sysEventType_t v5; // [esp+1Ch] [ebp-14h]
	int v6; // [esp+20h] [ebp-10h]
	int v7; // [esp+24h] [ebp-Ch]
	int v8; // [esp+28h] [ebp-8h]
	void* v9; // [esp+2Ch] [ebp-4h]

	event = Win_GetEvent(&v3);
	evTime = event->evTime;
	v5 = event->evType;
	v6 = event->evValue;
	v7 = event->evValue2;
	v8 = event->evPtrLength;
	v9 = event->evPtr;
	result->evTime = v4;
	result->evType = v5;
	result->evValue = v6;
	result->evValue2 = v7;
	result->evPtrLength = v8;
	result->evPtr = v9;
	return result;
}

void Sys_Net_Restart_f()
{
	NET_Restart();
}

void Sys_Init()
{
	const char* v0; // eax
	const char* v1; // eax
	const char* v2; // eax
	const char* v3; // eax
	const char* v4; // eax
	const char* v5; // eax
	_OSVERSIONINFOA osversion; // [esp+8h] [ebp-98h]

	timeBeginPeriod(1u);
	Cmd_AddCommandInternal("net_restart", Sys_Net_Restart_f, &Sys_Net_Restart_f_VAR);
	osversion.dwOSVersionInfoSize = 148;
	if (!GetVersionExA(&osversion))
		Sys_Error("Couldn't get OS info");
	if (osversion.dwMajorVersion < 4)
	{
		v0 = Com_GetBuildDisplayNameR();
		v1 = va("%s requires Windows version 4 or greater", v0);
		Sys_Error(v1);
	}
	if (!osversion.dwPlatformId)
	{
		v2 = Com_GetBuildDisplayNameR();
		v3 = va("%s doesn't run on Win32s", v2);
		Sys_Error(v3);
	}
	Com_Printf(10, "CPU vendor is \"%s\"\n", sys_info.cpuVendor);
	Com_Printf(10, "CPU name is \"%s\"\n", sys_info.cpuName);
	v4 = (const char*)&pBlock;
	if (sys_info.logicalCpuCount != 1)
		v4 = "s";
	Com_Printf(10, "%i logical CPU%s reported\n", sys_info.logicalCpuCount, v4);
	v5 = (const char*)&pBlock;
	if (sys_info.physicalCpuCount != 1)
		v5 = "s";
	Com_Printf(10, "%i physical CPU%s detected\n", sys_info.physicalCpuCount, v5);
	Com_Printf(10, "Measured CPU speed is %.2lf GHz\n", sys_info.cpuGHz);
	Com_Printf(10, "Total CPU performance is estimated as %.2lf GHz\n", sys_info.configureGHz);
	Com_Printf(10, "System memory is %i MB (capped at 1 GB)\n", sys_info.sysMB);
	Com_Printf(10, "Video card is \"%s\"\n", sys_info.gpuDescription);
	Com_Printf(10, "\n");
}

char* Sys_GetIdentityParam(IdentityParam p)
{
#ifdef _DEBUG
	if ((unsigned int)p >= 7
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(unsigned)(p) < (unsigned)(IDENTITY_PARAM_COUNT)",
			"p doesn't index IDENTITY_PARAM_COUNT\n\t%i not in [0, %i)",
			p,
			7))
	{
		__debugbreak();
	}
#endif
	return &g_identityParams[256 * p];
}

char* getArgFromString(char const* src, char const* marker, char* dst, unsigned int dstSize)
{
	const char* v4; // eax
	unsigned int v5; // kr00_4
	char* v6; // edx
	char* v7; // eax
	unsigned int v8; // esi
	char v9; // cl

	v4 = I_stristr(src, marker);
	if (!v4 || v4 != src && *(v4 - 1) != 32)
		return 0;
	v5 = strlen(marker);
	v6 = dst;
	v7 = (char*)&v4[v5];
	v8 = 0;
	if (dstSize != 1)
	{
		do
		{
			v9 = *v7;
			if (!*v7)
				break;
			if (v9 == 32)
				break;
			if (v9 == 9)
				break;
			*v6 = v9;
			++v8;
			++v7;
			++v6;
		} while (v8 < dstSize - 1);
	}
	*v6 = 0;
	return v6;
}

void checkdmlineValid()
{
	char* v0; // esi
	signed int v1; // edi
	const char* v2; // eax

	v0 = s_cmdlineArgs[0].val;
	v1 = 7;
	do
	{
		if (!v0[128] && !*v0)
		{
			v2 = va("Missing cmdline argument %s\n ", v0 - 64);
			Sys_Error(v2);
		}
		v0 += 200;
		--v1;
	} while (v1);
}

#ifdef DEDICATED_SERVER

bool Sys_ReadIdentity(char const* id)
{
	return false;
}

void DedicatedInit(const char* cmdline)
{
	char* v1; // esi
	const char* v2; // eax
	const char* v3; // eax
	signed int v4; // [esp+Ch] [ebp-88h]
	char identity[128]; // [esp+10h] [ebp-84h]

	if (getArgFromString(cmdline, "id:", identity, 0x80u))
	{
		Com_Printf(10, "Using identity: %s\n", identity);
		if (!Sys_ReadIdentity(identity))
			Sys_Error("Could not find identity '%s' in %s", identity, "identities.csv");
	}
	else
	{
		v1 = s_cmdlineArgs[0].val;
		v4 = 7;
		do
		{
			if (getArgFromString(cmdline, v1 - 64, v1, 0x80u))
			{
				Com_Printf(26, "%s: %s\t%s\n", "DedicatedInit", v1 - 64, v1);
				strncpy(&g_identityParams[256 * *((_DWORD*)v1 + 33)], v1, 0x100u);
			}
			v1 += 200;
			--v4;
		} while (v4);
		Live_SetLsgAddrFromCommandLine();
		Live_SetAuthAddrFromCommandLine();
		Live_SetTitleIDFromCommandLine();
	}
	Com_Printf(10, "Platform: %s\n", &g_identityParams[1536]);
	if (I_stricmp(&g_identityParams[1536], "ps3"))
	{
		if (I_stricmp(&g_identityParams[1536], "xenon"))
		{
			if (I_stricmp(&g_identityParams[1536], "pc"))
			{
				if (I_stricmp(&g_identityParams[1536], "wiiu"))
					Com_PrintError(26, "No clientplatform found on cmdline, defaulting to PC!\n");
				else
					g_clientPlatform = 3;
			}
			else
			{
				g_clientPlatform = 0;
			}
		}
		else
		{
			g_clientPlatform = 2;
		}
	}
	else
	{
		g_clientPlatform = 1;
	}
	Com_Printf(10, "Environment: %s\n", &g_identityParams[1280]);
	if (I_stricmp(&g_identityParams[1280], "dev"))
	{
		if (I_stricmp(&g_identityParams[1280], "cert"))
		{
			if (I_stricmp(&g_identityParams[1280], "live"))
				Com_PrintWarning(26, "No client lsg found, defaulting to dev!\n");
			else
				g_clientLSG = 2;
		}
		else
		{
			g_clientLSG = 1;
		}
	}
	else
	{
		g_clientLSG = 0;
	}
	if (Sys_IsMiniDumpStarted())
	{
		v2 = Sys_DefaultHomePath();
		v3 = va("%s\\dumps\\", v2);
		Sys_SetMiniDumpDir(v3);
	}
}
#endif

void InitMiniDumper(char *lpCmdLine)
{
	if (I_stristr(lpCmdLine, "autodump"))
	{
		Sys_StartMiniDump(0);
	}
	else if (I_stristr(lpCmdLine, "minidump") || !I_stristr(lpCmdLine, "nodump"))
	{
		Sys_StartMiniDump(1);
	}
	else
	{
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)PrivateUnhandledExceptionFilter);
	}
}

char* Win_GetTheOtherExeName(const char* mode)
{
	const char* v1; // eax
	const char* v2; // ebx
	int v3; // esi
	int v4; // eax
	const char* v5; // eax
	bool zm; // [esp+Fh] [ebp-309h]
	char fullpath[260]; // [esp+10h] [ebp-308h]
	char ext[256]; // [esp+114h] [ebp-204h]
	char filename[256]; // [esp+214h] [ebp-104h]

#ifdef _DEBUG
	if ((!*mode || !mode[1] || mode[2])
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(mode[0] && mode[1] && !mode[2])",
			(const char*)&pBlock))
	{
		__debugbreak();
	}
#endif
	if (tolower(*mode) != 122 || (zm = 1, tolower(mode[1]) != 109))
		zm = 0;
#ifdef _DEBUG
	if ((tolower(*mode) != 115 || tolower(mode[1]) != 112)
		&& (tolower(*mode) != 109 || tolower(mode[1]) != 112)
		&& !zm
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(( tolower( mode[0] ) == 's' && tolower( mode[1] ) == 'p' ) || ( tolower( mode[0] ) == 'm' && tolower( mode[1]"
			" ) == 'p' ) || ( zm ))",
			(const char*)&pBlock))
	{
		__debugbreak();
	}
#endif
	GetModuleFileNameA(0, fullpath, 0x104u);
	_splitpath(fullpath, 0, 0, filename, ext);
	strrchr(filename, 95);
	v2 = v1;
	if (v1)
		v3 = v1 - filename;
	else
		v3 = strlen(filename);
#ifdef _DEBUG
	if (v3 <= 2
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(2 < len)", (const char*)&pBlock))
	{
		__debugbreak();
	}
#endif
	v4 = v3 - 1;
	if (v3 - 1 > 256)
		v4 = 256;
	I_strncpyz(theOtherExeName, filename, v4);
	v5 = "mp";
	if (!zm)
		v5 = mode;
	I_strncat(theOtherExeName, 256, v5);
	if (v2)
		I_strncat(theOtherExeName, 256, v2);
	I_strncat(theOtherExeName, 256, ext);
	return theOtherExeName;
}

void Sys_CheckQuitRequest()
{
	if (g_quitRequested && (unsigned __int8)Sys_IsMainThread())
	{
		if (errPtr[0])
			Sys_Error(errPtr);
		Sys_Error("Error quit was not requested in the main thread\n");
	}
}

void Sys_Quit()
{
	Sys_EnterCriticalSection(CRITSECT_COM_ERROR);
	timeEndPeriod(1u);
	Sys_SpawnQuitProcess();
	IN_Shutdown();
	Key_Shutdown();
	Sys_DestroyConsole();
	Win_ShutdownLocalization();
	RefreshQuitOnErrorCondition();
	Dvar_Shutdown();
	Cmd_Shutdown();
	Sys_ShutdownEvents();
	SL_Shutdown();
	if (!com_errorEntered)
		track_shutdown(0);
	Con_ShutdownChannels();
	LiveSteam_Shutdown();
	ExitProcess(0);
}

int __stdcall WinMain(HINSTANCE* hInstance, HINSTANCE* hPrevInstance, char* lpCmdLine, int nCmdShow)
{
	HMODULE v4; // eax
	HMODULE v5; // edi
	void (*v6)(void); // eax
	bool v7; // bl
	char pnbuf; // [esp+10h] [ebp-104h]

	v4 = LoadLibraryA("user32.dll");
	v5 = v4;
	if (v4)
	{
		v6 = (void (*)(void))GetProcAddress(v4, "SetProcessDPIAware");
		if (v6)
			v6();
		FreeLibrary(v5);
	}
	InitMiniDumper(lpCmdLine);
	Sys_CreateConsole(hInstance);
	Sys_ShowConsole();
	if (I_stristr(lpCmdLine, "usedevlsg"))
	{
		g_dwUseDevLSG = 1;
	}
	else if (I_stristr(lpCmdLine, "useprodlsg"))
	{
		g_dwUseDevLSG = 0;
	}
	Sys_InitializeCriticalSections();
	Sys_InitMainThread();
	g_quitRequested = 0;
	Com_InitParse();
	DedicatedInit(lpCmdLine);
	PMem_Init();
	track_init();
	v7 = Win_CheckForZombieMode(lpCmdLine);
	R_SetIsMultiplayer(1);
	R_SetIsZombie(v7);
	Win_InitLocalization(v7);
	if (G_ExitAfterConnectPaths())
	{
		g_pc_nosnd = 1;
	}
	else if (GetSystemMetrics(4096))
	{
		g_pc_nosnd = 1;
	}
	else if (I_stristr(lpCmdLine, "nosnd"))
	{
		g_pc_nosnd = 1;
	}
	else if (I_stristr(lpCmdLine, "shield"))
	{
		g_pc_snd_defaultdevice = 1;
	}
	if (!hPrevInstance)
	{
		SL_Init();
		Dvar_Init();
		InitTiming();
		Sys_FindInfo();
		I_strncpyz(sys_cmdline, lpCmdLine, 1024);
		SetErrorMode(1u);
		Sys_Milliseconds();
		tlPrintf("Hello from the wonderful world of TL\n");
		Sys_SetupTLCallbacks(0x900000);
		Con_InitChannels();
		TaskManager2_Init();
		if (v7)
		{
			Com_Printf(10, "----> Zombie Session Mode Set! <----\n");
			Com_SessionMode_SetMode(SESSIONMODE_ZOMBIES, 1);
		}
		Win_CheckForEnv(lpCmdLine);
		Live_Base_Init();
		Live_Base_Pump();
		if (!(unsigned __int8)Sys_IsMainThread()
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(Sys_IsMainThread())",
				(const char*)&clientinfo))
		{
			__debugbreak();
		}
		Com_Init(sys_cmdline, 0);
		Live_Base_Pump();
		_getcwd(&pnbuf, 256);
		Com_Printf(10, "Working directory: %s\n", &pnbuf);
		if (Dvar_GetBool(com_script_debugger_smoke_test))
			exit(31415);
		while (1)
		{
			Sleep(5u);
			if (g_quitRequested)
			{
				if ((unsigned __int8)Sys_IsMainThread())
					break;
			}
			Com_Frame();
		}
		if (errPtr[0])
			Sys_Error(errPtr);
		Sys_Error("Error quit was not requested in the main thread\n");
	}
	Win_ShutdownLocalization();
	track_shutdown(0);
	return 0;
}
