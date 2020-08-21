#pragma once

struct cmd_function_s
{
	cmd_function_s* next;
	const char* name;
	const char* autoCompleteDir;
	const char* autoCompleteExt;
	void(__cdecl* function)();
};

void Cmd_AddCommandInternal(const char* cmdName, void(* function)(), cmd_function_s* allocedCmd);
int Cmd_Argc();
const char* Cmd_Argv(int argIndex);

class cmd
{
};

