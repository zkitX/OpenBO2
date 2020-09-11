#include <direct.h>
#include <Windows.h>

#include "common.h"
#include "threads.h"
#include "../universal/q_shared.h"
#include "../win32/win_gamepad.h"
#include "../win32/win_main.h"
#include "../win32/win_syscon.h"
#include "../xanim/xmodel.h"

void Com_Error(errorParm_t code, const char* fmt, ...)
{
    int v2; // esi
    ControllerIndex_t v3; // eax
    const char* v4; // esi
    const char* v5; // eax
    LocalClientNum_t v6; // eax
    const char* v7; // eax
    XModelRigidCullInfoContext* v8; // ST3C_4
    _iobuf* v9; // eax
    int* v10; // eax
    va_list ap; // [esp+18h] [ebp+10h]

    va_start(ap, fmt);
    Dvar_SetCanSetConfigDvars(0);
    v2 = code;
    if ((code == 1 || code == 6) && G_ExitAfterConnectPaths())
    {
        _vsnprintf((char*)&com_errorMessage, 0x1000u, fmt, ap);
        unknownByte = 0;
        printf((const char*)&com_errorMessage);
        Com_Printf(10, (const char*)&com_errorMessage);
        Com_Printf(10, "STACKBEGIN -------------------------------------------------------------------\n");
        assertive::StackTrace_Walk(1, 0);
        assertive::StackTrace_ResolveSymbols();
        assertive::StackTrace_Generate(0x2000, g_stackTrace);
        Com_Printf(10, "%s", g_stackTrace);
        Com_Printf(10, "STACKEND ---------------------------------------------------------------------\n");
        return;
    }
    Sys_EnterCriticalSection(CRITSECT_COM_ERROR);
    assertive::StackTrace_ResetAddressInfo();
    if (!Demo_IsIdle() && code != 5)
        Demo_End(1);
    if (g_taskProcessNesting > 0)
    {
        Sys_LeaveCriticalSection(CRITSECT_COM_ERROR);
        Sys_Error("Com_Error called within a task success/failure callback");
    }
    if (com_errorEntered)
    {
        Sys_LeaveCriticalSection(CRITSECT_COM_ERROR);
        Sys_Error("%s", &com_errorMessage);
    }
    com_errorEntered = 1;
    _vsnprintf((char*)&com_errorMessage, 0x1000u, fmt, ap);
    unknownByte = 0;
    if (code == 5)
        BG_EvalVehicleName((XModelRigidCullInfoContext*)0xA, (const GfxViewParms*)0xBB8);
    Sys_Print((const char*)&com_errorMessage);
    if (code != 2)
    {
        if (code == 5 || code == 7)
        {
            if (Sys_IsRemoteDebugServer())
            {
                com_errorEntered = 0;
                Sys_LeaveCriticalSection(CRITSECT_COM_ERROR);
                return;
            }
        }
        else if (code != 6)
        {
            com_fixedConsolePosition = 0;
        LABEL_32:
            if (!IsDebuggerConnected() || v2)
            {
                if (v2 != 1 && v2)
                    goto LABEL_40;
            }
            else
            {
                __debugbreak();
            }
            Com_Printf(10, "STACKBEGIN -------------------------------------------------------------------\n");
            assertive::StackTrace_Walk(1, 0);
            assertive::StackTrace_ResolveSymbols();
            assertive::StackTrace_Generate(0x2000, g_stackTrace);
            Com_Printf(10, "%s", g_stackTrace);
            Com_Printf(10, "STACKEND ---------------------------------------------------------------------\n");
            v7 = "err_drop";
            if (v2 != 1)
                v7 = "err_fatal";
            BB_Alert("c:\\t6\\code\\src_noserver\\qcommon\\common.cpp", 2436, v7, (const char*)&com_errorMessage);
        LABEL_40:
            errorcode = v2;
            Sys_LeaveCriticalSection(CRITSECT_COM_ERROR);
            Com_Printf(10, "\n====================================================\n");
            Com_Printf(10, "Com_ERROR: %s", &com_errorMessage);
            Com_Printf(10, "\n====================================================\n\n");
            if (G_ExitOnComError(v2))
            {
                printf((const char*)&com_errorMessage);
                //LiveSteam_Shutdown();
                BG_EvalVehicleName(v8, &com_errorMessage);
                v9 = __iob_func();
                fflush(v9 + 1);
                ExitProcess(0xFFFFFFFF);
            }
            TaskManager2_ComErrorCleanup();
            PartyMigrate_ComErrorCleanup();
            GlassCl_WaitUpdate();
            v10 = (int*)Sys_GetValue(2);
            longjmp(v10, -1);
        }
        v2 = 1;
        goto LABEL_32;
    }
    if (Dvar_GetBool(com_report_syserrors))
    {
        Sys_EnterCriticalSection(CRITSECT_DEMONWARE);
        v3 = Com_ControllerIndexes_GetPrimary();
        dwRecordEventFormat(v3, DW_EVENT_SYS_ERROR, "%s", &com_errorMessage);
        Sys_LeaveCriticalSection(CRITSECT_DEMONWARE);
    }
    Com_Printf(10, "STACKBEGIN -------------------------------------------------------------------\n");
    assertive::StackTrace_Walk(1, 0);
    assertive::StackTrace_ResolveSymbols();
    assertive::StackTrace_Generate(0x2000, g_stackTrace);
    Com_Printf(10, "%s", g_stackTrace);
    Com_Printf(10, "STACKEND ---------------------------------------------------------------------\n");
    if (errorcode == 3 || (v4 = "MENU_ERROR", Com_ErrorIsNotice((const char*)&com_errorMessage)))
        v4 = "MENU_NOTICE";
    v5 = SEH_LocalizeTextMessage((const char*)&com_errorMessage, "error message", LOCMSG_NOERR);
    if (!v5)
        v5 = (const char*)&com_errorMessage;
    Com_SetLocalizedErrorMessage(v5, v4);
    v6 = Com_LocalClients_GetPrimary();
    UI_SetActiveMenu(v6, 1);
    com_errorEntered = 0;
    Sys_LeaveCriticalSection(CRITSECT_COM_ERROR);
}

void Com_ErrorCleanup()
{
}

void Com_Frame_Try_Block_Function()
{
}

const char* Com_GetBuildDisplayNameR()
{
    return "Call of Duty®";
}

const char* Com_GetBuildName()
{
    return "COD_T6";
}

const char* Com_GetBuildVersion()
{
    return "alpha_version";
}

int Com_GetPrivateClients()
{
    return 0;
}

void Com_PrintError(int channel, const char* fmt, ...)
{
    unsigned int v3; // kr00_4
    char dest[4095]; // [esp+0h] [ebp-1004h]
    char v5; // [esp+FFFh] [ebp-5h]
    va_list ap; // [esp+1014h] [ebp+10h]

    va_start(ap, fmt);
    if (I_stristr(fmt, "error"))
        I_strncpyz(dest, "^1", 4096);
    else
        I_strncpyz(dest, "^1Error: ", 4096);
    v3 = strlen(dest);
    _vsnprintf(&dest[v3], 4096 - v3, fmt, ap);
    ++com_errorPrintsCount;
    v5 = 0;
    Com_PrintMessage(channel, dest, 3);
}

void Com_Printf(int channel, const char* fmt, ...)
{
    signed int v3; // kr00_4
    char* v4; // edx
    char* v5; // esi
    char string; // [esp+Ch] [ebp-1044h]
    char v8; // [esp+Dh] [ebp-1043h]
    char msg; // [esp+Eh] [ebp-1042h]
    char v10; // [esp+100Bh] [ebp-45h]
    char source; // [esp+100Ch] [ebp-44h]
    char dst; // [esp+100Dh] [ebp-43h]
    va_list ap; // [esp+1060h] [ebp+10h]

    va_start(ap, fmt);
    if (channel < 46 || Con_IsChannelVisible(0, channel, 0))
    {
        _vsnprintf(&string, 0x1000u, fmt, ap);
        v10 = 0;
        if ((!Demo_IsPlaying() || !Dvar_GetBool(spawnsystem_debug))
            && (channel < 46 || Con_IsChannelVisible(0, channel, 0)))
        {
            Sys_EnterCriticalSection(CRITSECT_CONSOLE);
            source = 0;
            memset(&dst, 0, 0x3Fu);
            v3 = strlen(&string);
            if (v3 <= 256)
            {
                strncpy(&com_consoleBuffer[256 * com_consoleBufferCurLine], &source, 0);
                strncpy(&com_consoleBuffer[256 * com_consoleBufferCurLine], &string, v3);
            }
            else
            {
                strncpy(&com_consoleBuffer[256 * com_consoleBufferCurLine], &source, 0);
                strncpy(&com_consoleBuffer[256 * com_consoleBufferCurLine], &string, 0xFFu);
                com_consoleBuffer[256 * com_consoleBufferCurLine + 255] = 0;
            }
            if (++com_consoleBufferCurLine >= 100)
                com_consoleBufferCurLine = 0;
            Sys_LeaveCriticalSection(CRITSECT_CONSOLE);
            if (rd_buffer)
            {
                if (channel != 8)
                {
                    Sys_EnterCriticalSection(CRITSECT_RD_BUFFER);
                    v4 = rd_buffer;
                    if (strlen(rd_buffer) + strlen(&string) > rd_buffersize - 1)
                    {
                        rd_flush(rd_buffer);
                        v4 = rd_buffer;
                        *rd_buffer = 0;
                    }
                    I_strncat(v4, rd_buffersize, &string);
                    Sys_LeaveCriticalSection(CRITSECT_RD_BUFFER);
                }
            }
            else
            {
                if (string != 94 || (v5 = &msg, !v8))
                    v5 = &string;
                if (channel != 8 && (!Dvar_GetBool(com_filter_output) || Con_IsChannelVisible(0, channel, 3)))
                    Sys_Print(v5);
                if (channel != 9 && com_logfile && Dvar_GetInt(com_logfile))
                    Com_LogPrintMessage(v5);
            }
        }
    }
}

void Com_PrintMessage(int channel, const char* msg, int error)
{
}

bool Com_SetPrivateClients()
{
    return false;
}
