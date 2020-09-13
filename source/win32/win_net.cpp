#include <WinSock2.h>

#include "win_main.h"
#include "win_shared.h"
#include "win_net.h"

#include <immintrin.h>

#include <qcommon/common.h>

void NET_Sleep(unsigned int timeInMs)
{
	Sleep(timeInMs);
}

const char *NET_ErrorString()
{
	int socketError;

	socketError = WSAGetLastError();
	switch (socketError) {
		case WSAEINTR: return "WSAEINTR";
		case WSAEBADF: return "WSAEBADF";
		case WSAEACCES: return "WSAEACCES";
		case WSAEDISCON: return "WSAEDISCON";
		case WSAEFAULT: return "WSAEFAULT";
		case WSAEINVAL: return "WSAEINVAL";
		case WSAEMFILE: return "WSAEMFILE";
		case WSAEWOULDBLOCK: return "WSAEWOULDBLOCK";
		case WSAEINPROGRESS: return "WSAEINPROGRESS";
		case WSAEALREADY: return "WSAEALREADY";
		case WSAENOTSOCK: return "WSAENOTSOCK";
		case WSAEDESTADDRREQ: return "WSAEDESTADDRREQ";
		case WSAEMSGSIZE: return "WSAEMSGSIZE";
		case WSAEPROTOTYPE: return "WSAEPROTOTYPE";
		case WSAENOPROTOOPT: return "WSAENOPROTOOPT";
		case WSAEPROTONOSUPPORT: return "WSAEPROTONOSUPPORT";
		case WSAESOCKTNOSUPPORT: return "WSAESOCKTNOSUPPORT";
		case WSAEOPNOTSUPP: return "WSAEOPNOTSUPP";
		case WSAEPFNOSUPPORT: return "WSAEPFNOSUPPORT";
		case WSAEAFNOSUPPORT: return "WSAEAFNOSUPPORT";
		case WSAEADDRINUSE: return "WSAEADDRINUSE";
		case WSAEADDRNOTAVAIL: return "WSAEADDRNOTAVAIL";
		case WSAENETDOWN: return "WSAENETDOWN";
		case WSAENETUNREACH: return "WSAENETUNREACH";
		case WSAENETRESET: return "WSAENETRESET";
		case WSAECONNABORTED: return "WSWSAECONNABORTEDAEINTR";
		case WSAECONNRESET: return "WSAECONNRESET";
		case WSAENOBUFS: return "WSAENOBUFS";
		case WSAEISCONN: return "WSAEISCONN";
		case WSAENOTCONN: return "WSAENOTCONN";
		case WSAESHUTDOWN: return "WSAESHUTDOWN";
		case WSAETOOMANYREFS: return "WSAETOOMANYREFS";
		case WSAETIMEDOUT: return "WSAETIMEDOUT";
		case WSAECONNREFUSED: return "WSAECONNREFUSED";
		case WSAELOOP: return "WSAELOOP";
		case WSAENAMETOOLONG: return "WSAENAMETOOLONG";
		case WSAEHOSTDOWN: return "WSAEHOSTDOWN";
		case WSASYSNOTREADY: return "WSASYSNOTREADY";
		case WSAVERNOTSUPPORTED: return "WSAVERNOTSUPPORTED";
		case WSANOTINITIALISED: return "WSANOTINITIALISED";
		case WSAHOST_NOT_FOUND: return "WSAHOST_NOT_FOUND";
		case WSATRY_AGAIN: return "WSATRY_AGAIN";
		case WSANO_RECOVERY: return "WSANO_RECOVERY";
		case WSANO_DATA: return "WSANO_DATA";
		default: return "NO ERROR";
	}
}

hostent* Sys_StringToSockaddr(char const* s, sockaddr* sadr)
{
	struct hostent* result; // eax

	_mm_storel_epi64((__m128i*)sadr, _mm_setzero_si128());				// Need a constructor for long long to m128i
	_mm_storel_epi64((__m128i*) & sadr->sa_data[6], _mm_setzero_si128());
	*(unsigned int*)&sadr->sa_family = 2;
	if (I_isdigit(*s))
	{
		*(unsigned int*)&sadr->sa_data[2] = inet_addr(s);
		result = (struct hostent*)1;
	}
	else
	{
		result = gethostbyname(s);
		if (result)
		{
			*(unsigned int*)&sadr->sa_data[2] = **(unsigned int**)result->h_addr_list;
			result = (struct hostent*)1;
		}
	}
	return result;
}

void NetadrToSockadr(netadr_t* a, sockaddr* s)
{
	netadrtype_t adrType;
	unsigned __int16 adrPort;

	float zero[] = { 0.0, 0.0, 0.0, 0.0 };
	_mm_storel_epi64((__m128i*)s, _mm_setzero_si128());
	_mm_storel_epi64((__m128i*) & s->sa_data[6], _mm_setzero_si128());
	adrType = a->type;
	if (adrType == NA_BROADCAST)
	{
		s->sa_family = 2;
		adrPort = a->port;
		*(unsigned int*)&s->sa_data[2] = -1;
		*(unsigned short*)s->sa_data = adrPort;
	}
	else if (adrType == NA_IP)
	{
		s->sa_family = 2;
		*(unsigned int*)&s->sa_data[2] = a->inaddr;
		*(unsigned short*)s->sa_data = a->port;
	}
}

void SockadrToNetadr(sockaddr* s, netadr_t* a)
{
	if (s->sa_family == 2)
	{
		a->type = NA_IP;
		a->inaddr = *(unsigned int*)&s->sa_data[2];
		a->port = *(unsigned short*)s->sa_data;
	}
}

int Sys_StringToAdr(char const* s, netadr_t* a)
{
	unsigned int netAddr; // eax
	struct hostent* hostent; // eax

	if (I_isdigit(*s))
	{
		netAddr = inet_addr(s);
	LABEL_3:
		a->inaddr = netAddr;
		a->port = 0;
		a->type = NA_IP;
		return 1;
	}
	hostent = gethostbyname(s);
	if (hostent)
	{
		netAddr = **(unsigned int**)hostent->h_addr_list;
		goto LABEL_3;
	}
	return 0;
}

int Sys_GetPacket(netadr_t* net_from, msg_t* net_message)
{
	SOCKET v2; // ebx
	int v3; // eax
	int v4; // ebx
	unsigned int v5; // eax
	int v6; // ebx
	unsigned int v7; // edx
	unsigned __int16 v8; // cx
	u_short v9; // ax
	netadr_t v10; // ST10_20
	const char* v11; // eax
	unsigned __int16 v13; // dx
	int v14; // eax
	netadr_t v15; // ST10_20
	const char* v16; // eax
	netadr_t v17; // ST10_20
	const char* v18; // ST20_4
	int v19; // ST1C_4
	char* v20; // eax
	int v21; // eax
	int v22; // eax
	char v23; // cl
	netsrc_t v24; // eax
	int v25; // ebx
	unsigned int clientAddr; // [esp+Ch] [ebp-520h]
	int fromlen; // [esp+10h] [ebp-51Ch]
	int clientPort; // [esp+14h] [ebp-518h]
	int shimPort; // [esp+18h] [ebp-514h]
	in_addr shimaddr; // [esp+1Ch] [ebp-510h]
	sockaddr from; // [esp+20h] [ebp-50Ch]
	char packetData[1270]; // [esp+30h] [ebp-4FCh]

	v2 = ip_socket;
	if (Com_UseRawUDP() && !v2 || G_ExitAfterConnectPaths())
		return 0;
	if (!Com_UseRawUDP())
		goto LABEL_43;
	fromlen = 0xFFFF;
#ifdef _DEBUG
	if ((!shim_addr || !shim_port)
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(shim_addr && shim_port)", &pBlock))
	{
		__debugbreak();
	}
#endif
	packetData[0] = 0;
	memset(&packetData[1], 0, 0x4F5u);
	v3 = recvfrom(v2, packetData, 1270, 0, &from, &fromlen);
	v4 = v3;
	if (v3 != -1)
		net_from->type = NA_IP;
	if (v3 <= 6)
		return 0;
	v5 = s_shiminaddr;
	shimPort = s_shimport;
	v6 = v4 - 8;
	shimaddr = (in_addr)s_shiminaddr;
	clientAddr = *(unsigned int*)packetData;
	clientPort = *(unsigned __int16*)&packetData[4];
	*(unsigned __int64*)&from.sa_data[6] = 0i64;
	if (from.sa_family == 2)
	{
		v7 = *(unsigned int*)&from.sa_data[2];
		v8 = *(unsigned short*)from.sa_data;
		net_from->type = NA_IP;
		net_from->inaddr = v7;
		net_from->port = v8;
	}
	net_message->readcount = 0;
	if (net_from->inaddr != v5 || (v9 = ntohs(net_from->port), v9 != (unsigned short)shimPort))
	{
		*(unsigned __int64*)v17.ip = *(unsigned __int64*)net_from->ip;
		*(unsigned __int64*)&v17.type = *(unsigned __int64*)&net_from->type;
		*(unsigned __int64*)&v17.serverID = *(unsigned __int64*)&net_from->serverID;
		v18 = NET_AdrToString(v17);
		v19 = (unsigned __int16)shimPort;
		v20 = inet_ntoa(shimaddr);
		Com_PrintError(26, "Only expecting packets from shim adr %s:%u, but we got packets from %s!", v20, v19, v18);
		return 0;
	}
	if (v6 == net_message->maxsize)
	{
	LABEL_17:
		*(unsigned __int64*)v10.ip = *(unsigned __int64*)net_from->ip;
		*(unsigned __int64*)&v10.type = *(unsigned __int64*)&net_from->type;
		*(unsigned __int64*)&v10.serverID = *(unsigned __int64*)&net_from->serverID;
		v11 = NET_AdrToString(v10);
		Com_Printf(10, "Oversize packet from %s\n", v11);
		return 0;
	}
	memcpy(net_message->data, &packetData[8], v6);
	v13 = clientPort;
	net_from->inaddr = clientAddr;
	net_from->port = v13;
	if (v6 == -1)
	{
	LABEL_43:
		if (Com_UseRawUDP())
			return 0;
		v14 = dwRecvFrom(net_from, net_message->data, net_message->maxsize);
		v6 = v14;
		if (v14 == -2 || v14 == -9)
			return 0;
		*(unsigned __int64*)&from.sa_data[6] = 0i64;
	}
	if (v6 == net_message->maxsize)
		goto LABEL_17;
	if (v6 < 1)
	{
		*(unsigned __int64*)v15.ip = *(unsigned __int64*)net_from->ip;
		*(unsigned __int64*)&v15.type = *(unsigned __int64*)&net_from->type;
		*(unsigned __int64*)&v15.serverID = *(unsigned __int64*)&net_from->serverID;
		v16 = NET_AdrToString(v15);
		Com_Printf(10, "Invalid packet size %d from %s\n", v6, v16);
		return 0;
	}
	net_message->readcount = 0;
	net_message->cursize = v6;
	shimaddr = (in_addr)(unsigned __int16)MSG_ReadShort(net_message);
	if (Com_UseRawUDP())
		net_from->serverID = shimaddr.S_un.S_un_w.s_w1;
	else
		net_from->serverID = 1111;
	v21 = MSG_ReadByte(net_message);
	net_message->targetLocalNetID = (v21 >> 4) & 0xF;
	v22 = v21 & 0xF;
	net_from->localNetID = v22;
	v23 = 0;
	if (v22 < 0 || v22 > 4)
	{
		Com_Printf(
			10,
			"Bad localNetID %d. From: %d.%d.%d.%d\n",
			v22,
			(unsigned __int8)net_from->ip[0],
			(unsigned __int8)net_from->ip[1],
			(unsigned __int8)net_from->ip[2],
			(unsigned __int8)net_from->ip[3]);
		v23 = 1;
	}
	v24 = net_message->targetLocalNetID;
	if ((v24 & 0x80000000) != 0 || (signed int)v24 > 4)
	{
		Com_Printf(
			10,
			"Bad remoteTargetClient %d. From %d.%d.%d.%d\n",
			v24,
			(unsigned __int8)net_from->ip[0],
			(unsigned __int8)net_from->ip[1],
			(unsigned __int8)net_from->ip[2],
			(unsigned __int8)net_from->ip[3]);
		return 0;
	}
	if (v23)
		return 0;
	net_message->data += 3;
	v25 = v6 - 3;
	if (!Com_UseRawUDP())
	{
		v25 = Sys_VerifyPacketChecksum(net_message->data, v25);
		if (v25 == -1)
			return 0;
	}
	net_message->cursize = v25;
	return 1;
}

bool Sys_SendPacket(netsrc_t sock, int length, const void* data, netadr_t to)
{
	int v4; // ebx
	__int16 v5; // ax
	unsigned __int16 v6; // ax
	unsigned int v7; // esi
	int v8; // esi
	unsigned __int16 v9; // ax
	netadrtype_t v10; // edi
	int v11; // eax
	netadr_t v12; // ST18_20
	int v13; // esi
	int v14; // eax
	netadr_t v15; // ST18_20
	const char* v16; // eax
	int v17; // eax
	netadr_t v18; // ST18_20
	const char* v19; // ST28_4
	char* v20; // eax
	msg_t msg; // [esp+0h] [ebp-A48h]
	int dataLen; // [esp+30h] [ebp-A18h]
	void* v24; // [esp+34h] [ebp-A14h]
	netadr_t c; // [esp+38h] [ebp-A10h]
	bool ok; // [esp+4Fh] [ebp-9F9h]
	sockaddr addr; // [esp+50h] [ebp-9F8h]
	char buffer[1264]; // [esp+60h] [ebp-9E8h]
	char cBuffer[1266]; // [esp+550h] [ebp-4F8h]

	v4 = length;
	c = to;
	v24 = (void*)data;
	ok = 0;
	if (!Com_UseRawUDP())
	{
		v5 = Sys_CheckSumPacketCopy(cBuffer, (const char*)data, length);
		v6 = BigShort(v5);
		*(unsigned short*)&cBuffer[length] = v6;
		if (v6 == -1)
			return 0;
		v24 = cBuffer;
		v4 = length + 2;
	}
	MSG_Init(&msg, buffer, 1264);
#ifdef _DEBUG
	if (((sock & 0x80000000) != 0 || (signed int)sock > 4)
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(sock >= NS_CLIENT1 && sock <= NS_SERVER)",
			&pBlock))
	{
		__debugbreak();
	}
#endif
	v7 = c.localNetID;
#ifdef _DEBUG
	if (((c.localNetID & 0x80000000) != 0 || (signed int)c.localNetID > 4)
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(to.localNetID >= NS_CLIENT1 && to.localNetID <= NS_SERVER)",
			&pBlock))
	{
		__debugbreak();
	}
#endif
	if ((unsigned int)sock <= 4 && v7 <= 4)
	{
		v8 = sock & 0xF | 16 * (v7 & 0xF);
		if (Com_UseRawUDP())
		{
			MSG_WriteLong(&msg, c.inaddr);
			MSG_WriteShort(&msg, c.port);
			c.inaddr = s_shiminaddr;
			c.port = htons(s_shimport);
			dataLen = (unsigned __int16)(v4 + 3);
			MSG_WriteData(&msg, &dataLen, 2);
			v9 = s_serverID;
		}
		else
		{
			v9 = 1111;
		}
		MSG_WriteShort(&msg, v9);
		NetadrToSockadr(&c, &addr);
		MSG_WriteByte(&msg, v8);
		MSG_WriteData(&msg, v24, v4);
		v10 = c.type;
		if (c.type == 3)
		{
			if (ip_socket)
			{
				v17 = sendto(ip_socket, msg.data, msg.cursize, 0, &addr, 16);
				ok = v17 >= 0;
				if (v17 >= 0)
					return ok;
			}
		}
		else if (c.type == 4)
		{
			if (Com_UseRawUDP())
			{
				v11 = sendto(ip_socket, msg.data, msg.cursize, 0, &addr, 16);
			}
			else
			{
				*(unsigned __int64*)v12.ip = *(unsigned __int64*)c.ip;
				*(unsigned __int64*)&v12.type = *(unsigned __int64*)&c.type;
				*(unsigned int*)&v12.serverID = *(unsigned int*)&c.serverID;
				v11 = dwSendTo(msg.cursize, msg.data, v12);
			}
			v13 = v11;
			ok = v11 >= 0;
			if (v11 >= 0)
				return ok;
			if (Com_UseRawUDP())
			{
				v14 = WSAGetLastError();
				Com_PrintError(26, "XenonClient: sendto last error is %u\n", v14);
			}
			else
			{
				*(unsigned __int64*)v15.ip = *(unsigned __int64*)c.ip;
				*(unsigned __int64*)&v15.type = *(unsigned __int64*)&c.type;
				*(unsigned int*)&v15.serverID = *(unsigned int*)&c.serverID;
				v16 = NET_AdrToString(v15);
				Com_PrintError(10, "dwSendTo ERROR: Demonware Error Code: %d length %d type %d to %s\n", v13, v4, 4, v16);
			}
		}
		else
		{
			Com_Error(0, "NET_SendPacket: bad address type");
			ok = 0;
		}
		*(unsigned __int64*)v18.ip = *(unsigned __int64*)c.ip;
		*(unsigned __int64*)&v18.type = *(unsigned __int64*)&c.type;
		*(unsigned int*)&v18.serverID = *(unsigned int*)&c.serverID;
		v19 = NET_AdrToString(v18);
		v20 = NET_ErrorString();
		Com_PrintError(10, "NET_SendPacket ERROR: %s type %d to %s\n", v20, v10, v19);
		return ok;
	}
	return 0;
}

void Sys_ShowIP()
{
	int i; // esi

	for (i = 0; i < numIP; ++i)
		Com_Printf(
			10,
			"IP: %i.%i.%i.%i\n",
			(unsigned __int8)localIP[i][0],
			(unsigned __int8)localIP[i][1],
			(unsigned __int8)localIP[i][2],
			(unsigned __int8)localIP[i][3]);
}

SOCKET NET_IPSocket(const char* net_interface, int port)
{
	SOCKET v2; // eax
	SOCKET v3; // esi
	const char* v4; // eax
	SOCKET result; // eax
	const char* v6; // eax
	const char* v7; // eax
	const char* v8; // eax
	int _true; // [esp+8h] [ebp-1Ch]
	int i; // [esp+Ch] [ebp-18h]
	sockaddr_in address; // [esp+10h] [ebp-14h]

	_true = 1;
	i = 1;
	if (net_interface)
		Com_Printf(10, "Opening IP socket: %s:%i\n", net_interface, port);
	else
		Com_Printf(10, "Opening IP socket: localhost:%i\n", port);
	v2 = socket(2, 2, 17);
	v3 = v2;
	if (v2 == -1)
	{
		if (WSAGetLastError() != 10047)
		{
			v4 = NET_ErrorString();
			Com_PrintWarning(10, "WARNING: UDP_OpenSocket: socket: %s\n", v4);
		}
		result = 0;
	}
	else if (ioctlsocket(v2, -2147195266, (u_long*)&_true) == -1)
	{
		v6 = NET_ErrorString();
		Com_PrintWarning(10, "WARNING: UDP_OpenSocket: ioctl FIONBIO: %s\n", v6);
		result = 0;
	}
	else if (setsockopt(v3, 0xFFFF, 32, (const char*)&i, 4) != -1
		&& (i = 0x40000, setsockopt(v3, 0xFFFF, 4098, (const char*)&i, 4) != -1)
		&& (i = 0x10000, setsockopt(v3, 0xFFFF, 4097, (const char*)&i, 4) != -1))
	{
		if (net_interface && *net_interface && I_stricmp(net_interface, "localhost"))
			Sys_StringToSockaddr(net_interface, (sockaddr*)&address);
		else
			address.sin_addr.S_un.S_addr = 0;
		if (Com_UseRawUDP() && (port == 1000 || port == 1001))
			Sys_Error("Can't start up on port %u or %u for protocol reasons. Choose another port.\n", 1000, 1001);
		if (port == -1)
			address.sin_port = 0;
		else
			address.sin_port = htons(port);
		address.sin_family = 2;
		if (bind(v3, (const struct sockaddr*)&address, 16) == -1)
		{
			v8 = NET_ErrorString();
			Com_PrintWarning(10, "WARNING: UDP_OpenSocket: bind: %s\n", v8);
			closesocket(v3);
			result = 0;
		}
		else
		{
			result = v3;
		}
	}
	else
	{
		v7 = NET_ErrorString();
		Com_PrintWarning(10, "WARNING: UDP_OpenSocket: setsockopt SO_BROADCAST: %s\n", v7);
		result = 0;
	}
	return result;
}

int NET_GetLocalAddress()
{
	int result; // eax
	struct hostent* v1; // eax
	struct hostent* v2; // edi
	int v3; // esi
	char* v4; // esi
	signed int v5; // eax
	int v6; // ecx
	char hostname[256]; // [esp+0h] [ebp-104h]

	if (gethostname(hostname, 256) == -1)
		return WSAGetLastError();
	v1 = gethostbyname(hostname);
	v2 = v1;
	if (!v1)
		return WSAGetLastError();
	Com_Printf(10, "Hostname: %s\n", v1->h_name);
	result = (int)*v2->h_aliases;
	if (result)
	{
		v3 = 0;
		do
		{
			++v3;
			Com_Printf(10, "Alias: %s\n", result);
			result = (int)v2->h_aliases[v3];
		} while (result);
	}
	if (v2->h_addrtype == 2)
	{
		v4 = *v2->h_addr_list;
		result = 0;
		for (numIP = 0; v4; result = numIP++ + 1)
		{
			if (result >= 16)
				break;
			v5 = ntohl(*(unsigned int*)v4);
			v6 = numIP;
			localIP[numIP][0] = *v4;
			localIP[v6][1] = v4[1];
			localIP[v6][2] = v4[2];
			localIP[v6][3] = v4[3];
			Com_Printf(10, "IP: %i.%i.%i.%i\n", (v5 >> 24) & 0xFF, (v5 >> 16) & 0xFF, (v5 >> 8) & 0xFF, (unsigned __int8)v5);
			v4 = v2->h_addr_list[numIP + 1];
		}
	}
	return result;
}

void NET_OpenIP()
{
	int v0; // esi
	int v1; // esi
	int v2; // ST30_4
	const char* v3; // eax
	int v4; // eax
	const char* v5; // eax
	int v6; // ST30_4
	const char* v7; // eax
	const char* v8; // eax
	const char* v9; // eax
	int v10; // [esp-4h] [ebp-Ch]

	switch (Com_GetClientPlatform())
	{
	case 0:
		v0 = 4976;
		break;
	case 1:
		v0 = 3075;
		break;
	case 2:
		v0 = 1001;
		break;
	case 3:
		v0 = 2020;
		break;
	}
	port = _Dvar_RegisterInt("net_port", 4976, 0, 0xFFFF, 0x20u, "Network port");
	broadcastport = _Dvar_RegisterInt("net_broadcast_port", v0, 0, 0xFFFF, 0x20u, "Broadcast Network port");
	if (Dvar_GetBool(net_broadcast))
	{
		if (Com_UseRawUDP())
			Sys_Error("Broadcast socket and raw sockets are mutually exclusive\n");
		v1 = 0;
		while (1)
		{
			v2 = v1 + Dvar_GetInt(broadcastport);
			v3 = Dvar_GetString(ip);
			ip_socket = NET_IPSocket(v3, v2);
			if (ip_socket)
				break;
			if (++v1 >= 10)
			{
				Com_PrintWarning(10, "WARNING: Couldn't allocate IP port\n");
				return;
			}
		}
		v4 = Dvar_GetInt(broadcastport);
		Dvar_SetInt(port, v1 + v4);
		goto LABEL_14;
	}
	if (Com_UseRawUDP())
	{
		shim_addr = _Dvar_RegisterString(
			"shim_addr",
			&pBlock,
			0,
			"Address to expect packets from. Should be the internal addr in the shim config");
		shim_port = _Dvar_RegisterInt(
			"shim_port",
			0,
			0,
			0xFFFF,
			0,
			"Port to expect packets from. Should be the internal port in the shim config");
		if (!strlen(Dvar_GetString(shim_addr)))
			Sys_Error("We're in shim mode, but shim_addr isn't set - it should be the internal addr in shim.ini\n");
		if (!Dvar_GetInt(shim_port))
			Sys_Error("We're in shim mode, but shim_port isn't set - it should be the internal port in shim.ini\n");
		v5 = Dvar_GetString(shim_addr);
		s_shiminaddr = inet_addr(v5);
		s_shimport = Dvar_GetInt(shim_port);
		s_serverID = Dvar_GetInt(port);
		v6 = Dvar_GetInt(port);
		v7 = Dvar_GetString(ip);
		ip_socket = NET_IPSocket(v7, v6);
		if (!ip_socket)
		{
			v10 = Dvar_GetInt(port);
			v8 = Dvar_GetString(ip);
			v9 = va("Couldn't bind to %s:%u\n", v8, v10);
			Sys_Error(v9);
		}
	LABEL_14:
		NET_GetLocalAddress();
	}
}

int NET_GetDvars()
{
	signed int result; // esi

	result = 0;
	if (net_noudp && Dvar_GetModified(net_noudp))
		result = 1;
	net_noudp = _Dvar_RegisterBool("net_noudp", 0, 0x21u, "Disable UDP");
	if (net_socksEnabled && Dvar_GetModified(net_socksEnabled))
		result = 1;
	net_socksEnabled = _Dvar_RegisterBool("net_socksEnabled", 0, 0x21u, "Enable network sockets");
	if (net_socksServer && Dvar_GetModified(net_socksServer))
		result = 1;
	net_socksServer = _Dvar_RegisterString("net_socksServer", &pBlock, 0x21u, "Network socket server");
	if (net_socksPort && Dvar_GetModified(net_socksPort))
		result = 1;
	net_socksPort = _Dvar_RegisterInt("net_socksPort", 1080, 0, 0xFFFF, 0x21u, "Network socket port");
	if (net_socksUsername && Dvar_GetModified(net_socksUsername))
		result = 1;
	net_socksUsername = _Dvar_RegisterString("net_socksUsername", &pBlock, 0x21u, "Network socket username");
	if (net_socksPassword && Dvar_GetModified(net_socksPassword))
		result = 1;
	net_socksPassword = _Dvar_RegisterString("net_socksPassword", &pBlock, 0x21u, "Network socket password");
	return result;
}

void Sys_CheckForNATOverflow()
{
	int v0; // esi
	int v1; // ebx
	int* v2; // edi
	int v3; // edi
	int* v4; // esi
	const dvar_t* v5; // eax

	if (!Dvar_GetBool(cl_socketpool_enabled))
		return;
	v0 = 0;
	if (cl_nat_overflow_detected > 0)
	{
		if ((signed int)(Sys_Milliseconds() - cl_nat_overflow_detected) > 20000)
		{
			Com_Printf(0, "SOCKETPOOL: Clearing timeout\n");
			cl_nat_overflow_detected = 0;
		}
		return;
	}
	v1 = Dvar_GetInt(cl_socketpool_size);
	if (v1 <= 0)
		return;
	v2 = &poolsockets[0].ready;
	while (1)
	{
		if (*v2 || (signed int)(Sys_Milliseconds() - *(v2 - 2)) <= 2000)
			goto LABEL_11;
		Com_Printf(0, "SOCKETPOOL: Timing out socket %i\n", v0);
		if ((signed int)(Sys_Milliseconds() - cl_socketpool_lastpacket_time) > 5000)
			break;
		*v2 = 1;
	LABEL_11:
		++v0;
		v2 += 4;
		if (v0 >= v1)
			return;
	}
	cl_nat_overflow_detected = Sys_Milliseconds();
	v3 = 0;
	if (Dvar_GetInt(cl_socketpool_size) > 0)
	{
		v4 = &poolsockets[0].ready;
		do
		{
			v5 = cl_socketpool_size;
			*v4 = 1;
			++v3;
			v4 += 4;
		} while (v3 < Dvar_GetInt(v5));
	}
}

int Sys_SocketPool_GetPacket(netadr_t* net_from, msg_t* net_message)
{
	msg_t* v2; // esi
	int result; // eax
	int v4; // edi
	SOCKET* v5; // ebx
	int v6; // esi
	int v7; // eax
	const char* v8; // eax
	int v9; // eax
	int v10; // edx
	__int16 v11; // cx
	int v12; // edx
	int fromlen; // [esp+4h] [ebp-24h]
	netadr_t* v14; // [esp+8h] [ebp-20h]
	int socketpool_size; // [esp+Ch] [ebp-1Ch]
	msg_t* v16; // [esp+10h] [ebp-18h]
	sockaddr from; // [esp+14h] [ebp-14h]

	v2 = net_message;
	v14 = net_from;
	v16 = net_message;
	if (!Sys_IsMainThread() || !Dvar_GetBool(cl_socketpool_enabled))
		return 0;
	fromlen = 16;
	v4 = 0;
	socketpool_size = Dvar_GetInt(cl_socketpool_size);
	if (socketpool_size <= 0)
		return 0;
	v5 = &poolsockets[0].m_sock;
	while (1)
	{
		if (v5[1])
			goto LABEL_12;
		v6 = recvfrom(*v5, v2->data, v2->maxsize, 0, &from, &fromlen);
		if (v6 != -1)
			break;
		v7 = WSAGetLastError();
		if (v7 != 10035 && v7 != 10054)
		{
			v8 = NET_ErrorString();
			Com_PrintError(10, "NET_GetPacket: %s\n", v8);
		}
	LABEL_12:
		++v4;
		v5 += 4;
		if (v4 >= socketpool_size)
			return 0;
		v2 = v16;
	}
	cl_socketpool_lastpacket_time = Sys_Milliseconds();
	if (from.sa_family == 2)
	{
		v9 = (int)v14;
		v10 = *(unsigned int*)&from.sa_data[2];
		v11 = *(unsigned short*)from.sa_data;
		v14->type = NA_IP;
		*(unsigned int*)v9 = v10;
		*(unsigned short*)(v9 + 4) = v11;
	}
	v12 = (int)v16;
	result = 1;
	poolsockets[v4].ready = 1;
	*(unsigned int*)(v12 + 20) = v6;
	return result;
}

void NET_Config(int enableNetworking)
{
	int v1; // esi
	int v2; // ecx
	signed int v3; // edi
	signed int v4; // eax

	v1 = NET_GetDvars();
	if (Dvar_GetBool(net_noudp))
		v2 = 0;
	else
		v2 = enableNetworking;
	if (v2 == networkingEnabled)
	{
		if (!v1 || !v2)
			return;
		v3 = 1;
		goto LABEL_12;
	}
	if (v2)
	{
		v4 = 0;
		v3 = 1;
	}
	else
	{
		v4 = 1;
		v3 = 0;
	}
	networkingEnabled = v2;
	if (v4)
	{
	LABEL_12:
		if (ip_socket && ip_socket != -1)
		{
			closesocket(ip_socket);
			ip_socket = 0;
		}
		if (socks_socket && socks_socket != -1)
		{
			closesocket(socks_socket);
			socks_socket = 0;
		}
	}
	if (v3)
	{
		if (!Dvar_GetBool(net_noudp))
			NET_OpenIP();
	}
}

void NET_Restart()
{
	NET_Config(networkingEnabled);
}

int NET_IsWinsockReady()
{
	return winsockInitialized;
}

void NET_Init()
{
	int v0; // eax

	ip = _Dvar_RegisterString("net_ip", "localhost", 0x20u, "Network IP Address");
	if (!winsockInitialized)
	{
		v0 = WSAStartup(0x101u, &winsockdata);
		if (v0)
		{
			Com_PrintWarning(10, "WARNING: Winsock initialization failed, returned %d\n", v0);
			return;
		}
		winsockInitialized = 1;
		Com_Printf(10, "Winsock Initialized\n");
	}
	SV_SysLog_Init();
	NET_GetDvars();
	NET_Config(1);
	SV_Query_Init();
	winDmInitServer();
#ifdef _DEBUG
	NET_InitDebug();
#endif
}

