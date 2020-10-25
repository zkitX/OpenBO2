#include <pch.h>

SOCKET bdPlatformSocket::create(const bool blocking, const bool broadcast) {
	SOCKET newSocket;
	u_long nonblocking;

    newSocket = socket(2, 2, 17);
    if (broadcast)
    {
        *(int*)&broadcast = 1;
        if (setsockopt(newSocket, 0xFFFF, 32, (const char*)&broadcast, 4))
            return -1;
    }
    nonblocking = !blocking;
    if (ioctlsocket(newSocket, -2147195266, &nonblocking))
        newSocket = -1;
    return newSocket;
}

bdSocketStatusCode bdPlatformSocket::bind1(int* handle, const bdInAddr addr, const unsigned __int16 port)
{
    enum bdSocketStatusCode result; // eax
    unsigned __int16 v4; // ax
    int v5; // ecx
    int v6; // edi
    sockaddr_in localAddr; // [esp+4h] [ebp-14h]

    if (*handle == 4294967295)
        return BD_NET_INVALID_HANDLE;
    localAddr.sin_port = 0;
    *(_DWORD*)localAddr.sin_zero = 0;
    *(_DWORD*)&localAddr.sin_zero[4] = 0;
    localAddr.sin_family = 2;
    localAddr.sin_addr = addr;
    v4 = htons(port);
    v5 = *handle;
    localAddr.sin_port = v4;
    if (bind((SOCKET)v5, (const struct sockaddr*)&localAddr, 16) != -1)
        return BD_NET_SUCCESS;
    v6 = WSAGetLastError();
    //bdLogMessage(
    //    BD_LOG_WARNING,
    //    "warn/",
    //    "platform/socket",
    //    __FILE__,
    //    __FUNCTION__,
    //    0x7Cu,
    //    "Call to bind() failed, WSAGetLastError: %d",
    //    v6);
    closesocket(*handle);
    if (v6 == 10013 || v6 > 10047 && v6 <= 10049)
        result = BD_NET_ADDRESS_IN_USE;
    else
        result = BD_NET_ERROR;
    return result;
}

int bdPlatformSocket::sendTo(int handle, bdInAddr addr, unsigned __int16 port, void* data, unsigned int length)
{
    int result; // eax
    int v6; // eax
    int v7; // ecx
    int lastError; // eax
    __int64 v9; // rax
    bool v10; // cf
    sockaddr_in recpt; // [esp+8h] [ebp-14h]

    if (handle == -1)
        return -10;
    *(int*)recpt.sin_zero = 0;
    *(int*)&recpt.sin_zero[4] = 0;
    recpt.sin_family = 2;
    recpt.sin_addr = (in_addr)addr;
    recpt.sin_port = htons(port);
    v6 = sendto(handle, (const char*)data, length, 0, (const struct sockaddr*)&recpt, 16);
    v7 = v6;
    if (v6 >= 0)
    {
        v9 = v6 + 28;
        v10 = __CFADD__((int)v9, (int)m_totalBytesSent);
        m_totalBytesSent = v9 + m_totalBytesSent;
        result = v7;
        m_totalPacketsSent += v9 + v10;
        ++m_totalPacketsSent;
    }
    else
    {
        lastError = WSAGetLastError();
        switch (lastError)
        {
        case 10004:
            result = -8;
            break;
        case 10022:
        case 10039:
        case 10047:
        case 10049:
            result = -7;
            break;
        case 10035:
            result = -2;
            break;
        case 10040:
            result = -6;
            break;
        case 10050:
        case 10051:
        case 10053:
        case 10054:
        case 10060:
        case 10065:
            /*bdLogMessage(
                BD_LOG_INFO,
                "info/",
                "platform/socket",
                __FILE__,
                __FUNCTION__,
                0xC0u,
                "send: Received connection reset : %i",
                lastError);*/
            result = -5;
            break;
        default:
            result = -1;
            break;
        }
    }
    return result;
}

int bdPlatformSocket::receiveFrom(int handle, bdInAddr* addr, unsigned __int16* port, void* data, unsigned int size)
{
    int result; // eax
    int v6; // esi
    int lastError; // eax
    unsigned __int16 v8; // ax
    ULONG v9; // edx
    unsigned __int64 v10; // rax
    bool v11; // cf
    int remoteAddrLen; // [esp+8h] [ebp-18h]
    sockaddr_in remoteAddr; // [esp+Ch] [ebp-14h]

    if (handle == -1)
        return -10;
    *(int*)&remoteAddr.sin_family = 0;
    remoteAddr.sin_addr.S_un.S_addr = 0;
    *(int*)remoteAddr.sin_zero = 0;
    *(int*)&remoteAddr.sin_zero[4] = 0;
    remoteAddrLen = 16;
    v6 = recvfrom(handle, (char*)data, size, 0, (struct sockaddr*)&remoteAddr, &remoteAddrLen);
    if (v6 >= 0)
    {
        v8 = ntohs(remoteAddr.sin_port);
        v9 = remoteAddr.sin_addr.S_un.S_addr;
        *port = v8;
        addr->inUn.m_iaddr = v9;
        v10 = v6 + 28;
        v11 = __CFADD__((int)v10, (int)m_totalBytesRecvd);
        m_totalBytesRecvd = v10 + m_totalBytesRecvd;
        result = v6;
        m_totalPacketsSent += v10 + v11;
        ++m_totalPacketsRecvd;
    }
    else
    {
        lastError = WSAGetLastError();
        switch (lastError)
        {
        //Interrupted function call.
        case WSAEINTR:
            result = -8;
            break;
        //Invalid argument.
        case WSAEINVAL:
            result = -9;
            break;
        //Resource temporarily unavailable.
        case WSAEWOULDBLOCK:
            result = -2;
            break;
        //Message too long.
        case WSAEMSGSIZE:
            result = -6;
            break;
        case WSAENETDOWN:
        case WSAENETUNREACH:
        case WSAECONNABORTED:
        case WSAECONNRESET:
        case WSAETIMEDOUT:
        case WSAEHOSTUNREACH:
            /*bdLogMessage(
                BD_LOG_INFO,
                "info/",
                "platform/socket",
                __FILE__,
                __FUNCTION__,
                0xF9u,
                "recv: Received connection reset : %i",
                lastError);*/
            *port = ntohs(remoteAddr.sin_port);
            *addr = remoteAddr.sin_addr;
            result = -5;
            break;
        default:
            result = -1;
            break;
        }
    }
    return result;
}

bool bdPlatformSocket::close(int* handle)
{
    bool result; // al

    result = *handle == -1;
    if (*handle != -1)
    {
        result = closesocket(*handle) == 0;
        *handle = -1;
    }
    return result;
}

unsigned int bdPlatformSocket::getHostByName(const char* const name, bdInAddr* adresses, unsigned int numAdresses)
{
    struct hostent* v3; // edi
    unsigned int result; // eax
    char** v5; // ecx
    bool v6; // zf
    in_addr** v7; // ecx
    int v8; // eax
    unsigned int v9; // eax

    if (isalpha(*name))
    {
        v3 = gethostbyname(name);
        if (v3)
        {
            for (result = 0; result < numAdresses; adresses[result++] = **v7)
            {
                v5 = v3->h_addr_list;
                v6 = v5[result] == 0;
                v7 = (in_addr**)v5[result];
                if (v6)
                    break;
            }
        }
        else
        {
            v8 = WSAGetLastError();
            /*bdLogMessage(
                BD_LOG_WARNING,
                "warn/",
                "platform/socket",
                __FILE__,
                __FUNCTION__,
                0x14Bu,
                "gethostbyname: Error : %i",
                v8);
            result = 0;*/
        }
    }
    else
    {
        v9 = inet_addr(name);
        adresses->inUn.m_iaddr = v9;
        if (v9 == -1)
            result = strcmp(magicCase, name) == 0;
        else
            result = 1;
    }
    return result;
}
