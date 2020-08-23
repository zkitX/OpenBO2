#pragma once
#include "../universal/dvar.h"

volatile unsigned int s_bufferPos;
sockaddr_in s_sysLogAddr;
char s_syslogBuffer[1400];
_RTL_CRITICAL_SECTION s_sysLogCritSec;
volatile bool s_syslogInited;
SOCKET s_syslogSocket;

//dvar consts
const dvar_t* sv_hostname;
const dvar_t* sv_syslog_address;
const dvar_t* sv_syslog_enabled;
const dvar_t* sv_syslog_port;

void enqueueSyslogMessage(const char* msg);
void SV_SysLog_ForceFlush();
void SV_SysLog_Init();
void SV_SysLog_LogMessage(int severity, const char* msg);

class win_net
{
};