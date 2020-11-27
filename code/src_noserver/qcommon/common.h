#pragma once
#include "../defs.h"
#include "../gfx_d3d/r_utils.h"

#include "../universal/assertive.h"
#include "../universal/com_tasks.h"
#include "../universal/dvar.h"
#include "../universal/win_common.h"

struct _iobuf
{
	char* _ptr;
	int _cnt;
	char* _base;
	int _flag;
	int _file;
	int _charbuf;
	int _bufsiz;
	char* _tmpfname;
};

enum errorParm_t
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

errorParm_t errorcode;

static const dvar_t* quit_on_error;

char exePath[256];
char unknownByte;

unsigned int com_errorPrintsCount;

int com_errorEntered;
int com_fixedConsolePosition;

const char* com_errorMessage;

void Com_Error(errorParm_t code, const char* fmt, ...);
void Com_ErrorCleanup();;
void Com_Frame_Try_Block_Function();
const char* Com_GetBuildDisplayNameR();
const char* Com_GetBuildName();
const char* Com_GetBuildVersion();
int Com_GetPrivateClients();
void Com_PrintError(int channel, const char* fmt, ...);
void Com_Printf(int channel, const char* fmt, ...);
void Com_PrintMessage(int channel, const char* msg, int error);
bool Com_SetPrivateClients();
