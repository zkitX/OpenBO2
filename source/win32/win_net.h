#pragma once

#include <universal/dvar.h>

enum netadrtype_t
{
	NA_BOT = 0x0,
	NA_BAD = 0x1,
	NA_LOOPBACK = 0x2,
	NA_BROADCAST = 0x3,
	NA_IP = 0x4,
};

enum netsrc_t
{
	NS_NULL = 0xFFFFFFFF,
	NS_CLIENT1 = 0x0,
	NS_CLIENT2 = 0x1,
	NS_CLIENT3 = 0x2,
	NS_CLIENT4 = 0x3,
	NS_SERVER = 0x4,
	NS_MAXCLIENTS = 0x4,
	NS_PACKET = 0x5,
};

struct socketpool_t
{
	int sendtime;
	unsigned int m_sock;
	int ready;
	int sendcount;
};

struct msg_t
{
	int overflowed;
	int readOnly;
	char* data;
	char* splitData;
	int maxsize;
	int cursize;
	int splitSize;
	int readcount;
	int bit;
	int lastEntityRef;
	int flush;
	netsrc_t targetLocalNetID;
};

struct netadr_t {
	union {
		char ip[4];
		unsigned int inaddr;
	};
	unsigned __int16 port;
	netadrtype_t type;
	netsrc_t localNetID;
	unsigned __int16 serverID;
};

SOCKET ip_socket;
const dvar_t* shim_addr;
const dvar_t* shim_port;
const dvar_t* port;
const dvar_t* broadcastport;
const dvar_t* ip;

const dvar_t* net_noudp;
const dvar_t* net_socksEnabled;
const dvar_t* net_socksServer;
const dvar_t* net_socksPort;
const dvar_t* net_socksUsername;
const dvar_t* net_socksPassword;

socketpool_t poolsockets[50];
int cl_socketpool_lastpacket_time;
int cl_nat_overflow_detected;

unsigned __int16 s_serverID;
unsigned int s_shiminaddr;
unsigned __int16 s_shimport;

int numIP;
char localIP[16][4];

int networkingEnabled;
SOCKET socks_socket;

int winsockInitialized;

WSAData winsockdata;

void NET_Sleep(unsigned int timeInMs);
const char* NET_ErrorString();
hostent* Sys_StringToSockaddr(char const* s, sockaddr* sadr);
void NetadrToSockadr(netadr_t* a, sockaddr* s);
void SockadrToNetadr(sockaddr* s, netadr_t* a);
int Sys_StringToAdr(char const* s, netadr_t* a);
int Sys_GetPacket(netadr_t* net_from, msg_t* net_message);
bool Sys_SendPacket(netsrc_t sock, int length, const void* data, netadr_t to);
void Sys_ShowIP();
SOCKET NET_IPSocket(const char* net_interface, int port);
int NET_GetLocalAddress();
void NET_OpenIP();
int NET_GetDvars();
void Sys_CheckForNATOverflow();
int Sys_SocketPool_GetPacket(netadr_t* net_from, msg_t* net_message);
void NET_Config(int enableNetworking);
void NET_Restart();
int NET_IsWinsockReady();
void NET_Init();