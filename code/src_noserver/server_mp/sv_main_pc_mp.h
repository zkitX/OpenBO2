#pragma once
#include <WinSock2.h>

#include <universal/dvar.h>

SOCKET s_syslogSocket;
volatile unsigned int s_bufferPos;
sockaddr_in s_sysLogAddr;
char s_syslogBuffer[1400];
_RTL_CRITICAL_SECTION s_sysLogCritSec;
volatile bool s_syslogInited;

void enqueueSyslogMessage(const char* msg);
void SV_SysLog_ForceFlush();
void SV_SysLog_Init();
void SV_SysLog_LogMessage(int severity, const char* msg);
