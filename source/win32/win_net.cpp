#include <WinSock2.h>

#include "win_main.h"
#include "win_net.h"

void enqueueSyslogMessage(const char* msg)
{
    unsigned int v1; // esi

    EnterCriticalSection(&s_sysLogCritSec);
    v1 = strlen(msg);
    if (v1 > 0x578)
        v1 = 1400;
    if (v1 > 1400 - s_bufferPos)
    {
        sendto(s_syslogSocket, s_syslogBuffer, s_bufferPos, 0, (const struct sockaddr*)&s_sysLogAddr, 16);
        s_bufferPos = 0;
    }
    memcpy(&s_syslogBuffer[s_bufferPos], (char*)msg, v1);
    s_bufferPos += v1;
    if (s_bufferPos >= 0x400)
    {
        sendto(s_syslogSocket, s_syslogBuffer, s_bufferPos, 0, (const struct sockaddr*)&s_sysLogAddr, 16);
        s_bufferPos = 0;
    }
    LeaveCriticalSection(&s_sysLogCritSec);
}

void SV_SysLog_ForceFlush()
{
    if (s_syslogInited)
    {
        sendto(s_syslogSocket, s_syslogBuffer, s_bufferPos, 0, (const struct sockaddr*)&s_sysLogAddr, 16);
        s_bufferPos = 0;
    }
}

void SV_SysLog_Init()
{
    int lastError; // eax
    const char* syslogAddress; // esi
    unsigned short syslogPort; // ax

    if (Dvar_GetBool(sv_syslog_enabled))
    {
        InitializeCriticalSection(&s_sysLogCritSec);
        s_syslogSocket = WSASocketA(2, 2, 17, 0, 0, 0);
        if (s_syslogSocket == -1)
        {
            lastError = WSAGetLastError();
            Sys_Error("Couldn't open syslog socket - error %u", lastError);
        }
        syslogAddress = Dvar_GetString(sv_syslog_address);
        syslogPort = Dvar_GetInt(sv_syslog_port);
        s_sysLogAddr.sin_family = 2;
        s_sysLogAddr.sin_port = htons(syslogPort);
        s_sysLogAddr.sin_addr.S_un.S_addr = inet_addr(syslogAddress);
        s_syslogInited = 1;
    }
}

void SV_SysLog_LogMessage(int severity, const char* msg)
{
    const char* v2; // eax
    char syslogMsg[1264]; // [esp+4h] [ebp-4F4h]

    if (s_syslogInited && Dvar_GetBool(sv_syslog_enabled))
    {
        syslogMsg[0] = 0;
        memset(&syslogMsg[1], 0, 0x4EFu);
        v2 = Dvar_GetString(sv_hostname);
        Com_sprintf(syslogMsg, 1263, "<%u>%s: %s", severity + 128, v2, msg);
        enqueueSyslogMessage(syslogMsg);
    }
}
