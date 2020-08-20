#pragma once
#include "../defs.h"
#include "../gfx_d3d/gfx.h"

#include "../universal/assertive.h"
#include "../universal/com_tasks.h"
#include "../universal/dvar.h"
#include "../universal/win_common.h"

GfxViewParms pBlock;

int com_errorEntered;
int com_fixedConsolePosition;

common::errorParm_t errorcode;

class common
{
public:
	static enum errorParm_t
	{
		ERR_FATAL = 0x0,
		ERR_DROP = 0x1,
		ERR_FROM_STARTUP = 0x2,
		ERR_SERVERDISCONNECT = 0x3,
		ERR_DISCONNECT = 0x4,
		ERR_SCRIPT = 0x5,
		ERR_SCRIPT_DROP = 0x6,
		ERR_LOCALIZATION = 0x7,
	};
	static GfxViewParms com_errorMessage;
	static char g_stackTrace[8192];
	static char unknownByte;
	// _________________________________________________________________________________
	// Function declarations
	// _________________________________________________________________________________
	static void Com_Error(errorParm_t code, const char* fmt, ...);
	void Com_ErrorCleanup();;
	void Com_Frame_Try_Block_Function();
	static const char* Com_GetBuildName();
	static const char* Com_GetBuildVersion();
	int Com_GetPrivateClients();
	static void Com_Printf(int a1, int channel, const char* fmt, ...);
	bool Com_SetPrivateClients();

	static char* Sys_Cwd();
	static char* Sys_DefaultInstallPath();

private:
	static char cwd[256];
	static char exePath[256];
};

