#pragma once
#include "../defs.h"

#include "../cgame/cg_main.h"
#include "../ui/ui_shared.h"
#include "../win32/win_gamepad.h"

struct cmd_function_s
{
	cmd_function_s* next;
	const char* name;
	const char* autoCompleteDir;
	const char* autoCompleteExt;
	void(__cdecl* function)();
};

struct CmdArgs
{
	int nesting;
	LocalClientNum_t localClientNum[8];
	ControllerIndex_t controllerIndex[8];
	itemDef_s* itemDef[8];
	int argshift[8];
	int argc[8];
	const char** argv[8];
	char textPool[8192];
	const char* argvPool[512];
	int usedTextPool[8];
	int totalUsedArgvPool;
	int totalUsedTextPool;
};

void Cmd_AddCommandInternal(const char* cmdName, void(* function)(), cmd_function_s* allocedCmd);
int Cmd_Argc();
const char* Cmd_Argv(int argIndex);
void Cmd_ExecuteSingleCommand(LocalClientNum_t localClientNum, ControllerIndex_t controllerIndex, const char* text, bool fromRemoteConsole);
void Cmd_ExecuteSingleCommandInternal(LocalClientNum_t localClientNum, ControllerIndex_t controllerIndex, itemDef_s* item, const char* text, bool fromRemoteConsole);

class cmd
{
};

