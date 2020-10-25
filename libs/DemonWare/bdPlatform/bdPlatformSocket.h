#pragma once

enum bdSocketStatusCode
{
	BD_NET_SUCCESS = 0x1,
	BD_NET_CONNECTION_CLOSED = 0x0,
	BD_NET_ERROR = 0xFFFFFFFF,
	BD_NET_WOULD_BLOCK = 0xFFFFFFFE,
	BD_NET_SUBSYTEM_ERROR = 0xFFFFFFFD,
	BD_NET_ADDRESS_IN_USE = 0xFFFFFFFC,
	BD_NET_CONNECTION_RESET = 0xFFFFFFFB,
	BD_NET_MSG_SIZE = 0xFFFFFFFA,
	BD_NET_ADDRESS_INVALID = 0xFFFFFFF9,
	BD_NET_BLOCKING_CALL_CANCELED = 0xFFFFFFF8,
	BD_NET_NOT_BOUND = 0xFFFFFFF7,
	BD_NET_INVALID_HANDLE = 0xFFFFFFF6,
	BD_NET_NOT_CONNECTED = 0xFFFFFFF5,
	BD_NET_CONNECTION_REFUSED = 0xFFFFFFF4,
	BD_NET_HOST_UNREACH = 0xFFFFFFF3,
};

class bdPlatformSocket
{
private:
	unsigned __int64 m_totalPacketsRecvd;
	unsigned __int64 m_totalPacketsSent;
	unsigned __int64 m_totalBytesSent;
	unsigned __int64 m_totalBytesRecvd;
	const char* magicCase = "255.255.255.255";
public:
	SOCKET create(const bool blocking, const bool broadcast);
	bdSocketStatusCode bind1(int* handle, const bdInAddr addr, const unsigned __int16 port);
	int sendTo(int handle, bdInAddr addr, unsigned __int16 port, void* data, unsigned int length);
	int receiveFrom(int handle, bdInAddr* addr, unsigned __int16* port, void* data, unsigned int size);
	bool close(int* handle);
	unsigned int getHostByName(const char* const name, bdInAddr* adresses, unsigned int numAdresses);
};