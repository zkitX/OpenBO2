#pragma once

void NET_Sleep(unsigned int timeInMs);
const char* NET_ErrorString();
hostent Sys_StringToSockaddr(char const* s, sockaddr* sadr);
