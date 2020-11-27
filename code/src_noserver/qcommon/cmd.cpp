#include "cmd.h"

#include "threads.h"

void Cmd_AddCommandInternal(const char* cmdName, void(*function)(), cmd_function_s* allocedCmd)
{
}

int Cmd_Argc()
{
    return 0;
}

const char* Cmd_Argv(int argIndex)
{
    unsigned int* v1;
    const char* result;

    v1 = Sys_GetValue(4);
}

void Cmd_ExecuteSingleCommand(LocalClientNum_t localClientNum, ControllerIndex_t controllerIndex, const char* text, bool fromRemoteConsole)
{
}

void Cmd_ExecuteSingleCommandInternal(LocalClientNum_t localClientNum, ControllerIndex_t controllerIndex, itemDef_s* item, const char* text, bool fromRemoteConsole)
{
}
