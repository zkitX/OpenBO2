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
    _DWORD* v1;
    const char* result;

    v1 = Sys_GetValue
}
