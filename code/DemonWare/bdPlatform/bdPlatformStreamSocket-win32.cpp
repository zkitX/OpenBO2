#include "pch.h"

int bdPlatformStreamSocket::create(bool blocking)
{
    int v1; // esi
    int result; // eax
    unsigned int nonblocking; // [esp+4h] [ebp-4h]

    v1 = socket(2, 1, 6);
    nonblocking = !blocking;
    if (ioctlsocket(v1, -2147195266, (u_long*)nonblocking))
        result = -1;
    else
        result = v1;
    return result;
}

bdSocketStatusCode bdPlatformStreamSocket::connect1(int handle, bdInAddr addr, unsigned short port)
{
    bdSocketStatusCode result; // eax
    sockaddr_in remoteAddr; // [esp+0h] [ebp-14h]

    *(_DWORD*)remoteAddr.sin_zero = 0;
    *(_DWORD*)&remoteAddr.sin_zero[4] = 0;
    remoteAddr.sin_family = 2;
    remoteAddr.sin_addr = (in_addr)addr;
    remoteAddr.sin_port = htons(port);
    if (connect(handle, (const struct sockaddr*)&remoteAddr, 16) >= 0)
        return BD_NET_SUCCESS;
    switch (WSAGetLastError())
    {
    case 10004:
        result = BD_NET_BLOCKING_CALL_CANCELED;
        break;
    case 10022:
        result = BD_NET_NOT_BOUND;
        break;
    case 10035:
        result = BD_NET_WOULD_BLOCK;
        break;
    case 10040:
        result = BD_NET_MSG_SIZE;
        break;
    case 10050:
    case 10051:
    case 10053:
    case 10054:
    case 10060:
    case 10065:
        result = BD_NET_CONNECTION_RESET;
        break;
    default:
        result = BD_NET_ERROR;
        break;
    }
    return result;
}

bool bdPlatformStreamSocket::close(int handle)
{
    bool result; // al
    int v2; // eax
    int v3; // [esp-4h] [ebp-4h]

    result = 1;
    if (handle != -1)
    {
        v2 = closesocket(handle);
        if (v2)
        {
            if (v2 == -1)
            {
                v3 = WSAGetLastError();
                bdLogMessage(
                    bdLogMessageType::BD_LOG_WARNING,
                    "warn/",
                    "platform stream socket",
                    "c:\\t6\\code\\DemonWare\\bdPlatform\\bdPlatformSocket\\bdPlatformStreamSocket-win32.cpp",
                    "bdPlatformStreamSocket::close",
                    0x160u,
                    "Failed to close socket Error %i.",
                    v3);
            }
            else
            {
                bdLogMessage(
                    bdLogMessageType::BD_LOG_WARNING,
                    "warn/",
                    "platform stream socket",
                    "c:\\t6\\code\\DemonWare\\bdPlatform\\bdPlatformSocket\\bdPlatformStreamSocket-win32.cpp",
                    "bdPlatformStreamSocket::close",
                    0x165u,
                    "Failed to close socket. Unknown Error Code %i.",
                    v2);
            }
            result = 0;
        }
        else
        {
            result = 1;
        }
    }
    return result;
}

bool bdPlatformStreamSocket::checkSocketException(int handle)
{
    return false;
}

bool bdPlatformStreamSocket::isWritable(int handle, bdSocketStatusCode* error)
{
    return false;
}

char bdPlatformStreamSocket::getSocketAddr(int handle, bdInAddr* socketAddr)
{
    return 0;
}

int bdPlatformStreamSocket::send(int handle, void const* const data, unsigned int length)
{
    return 0;
}

int bdPlatformStreamSocket::recieve(int handle, void const* const data, unsigned int length)
{
    return 0;
}

bool bdPlatformStreamSocket::isWritable(int handle)
{
    return false;
}
