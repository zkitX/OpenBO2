#include "blackbox.h"
#include "../defs.h"
#include "../qcommon/cmd.h"

#include <ctime>

void BB_Alert(const char* filename, int line, const char* type, const char* msg)
{
	const char* v4; // esi
	unsigned int v5; // edi
	AddressInfo_s* v6; // eax
	int v7; // ebx
	int v8; // esi
	int v9; // eax
	int v10; // eax
	ControllerIndex_t v11; // esi
	unsigned int v12; // eax
	BBMsgHeader* v13; // ecx
	int addressCount; // [esp+14h] [ebp-808h]
	char stacktrace[2048]; // [esp+18h] [ebp-804h]

	v4 = msg;
	stacktrace[0] = 0;
	v5 = 0;
	v6 = StackTrace_GetAddressInfo(&addressCount);
	if (addressCount <= 0)
		goto LABEL_14;
	v7 = 0;
	if (addressCount <= 0)
		goto LABEL_13;
	v8 = (int)v6->bestLineFilename;
	while (v5 < 0x800)
	{
		v9 = *(unsigned int*)(v8 - 200);
		if (v9)
		{
			if (*(unsigned char*)v8)
			{
				v10 = _snprintf(
					&stacktrace[v5],
					2048 - v5,
					"0x%08x#%d#%s#%s#%s\n",
					v9,
					*(unsigned int*)(v8 + 68),
					v8 - 132,
					v8,
					v8 - 196);
			LABEL_10:
				v5 += v10;
				goto LABEL_11;
			}
			if (*(unsigned char*)(v8 - 132))
			{
				v10 = _snprintf(&stacktrace[v5], 2048 - v5, "0x%08x##%s#%s#%s\n", v9, v8 - 132, v8 - 68, v8 - 196);
				goto LABEL_10;
			}
		}
	LABEL_11:
		++v7;
		v8 += 272;
		if (v7 >= addressCount)
			break;
	}
	v4 = msg;
LABEL_13:
	stacktrace[2047] = 0;
LABEL_14:
	if (!I_strncmp(v4, "\x15", 1))
		++v4;
	BB_Print(
		INVALID_CONTROLLER_PORT,
		"alerts",
		"line %d filename %s type %s stacktrace %s message %s",
		line,
		filename,
		type,
		stacktrace,
		v4);
	if (s_blackboxInitialized)
	{
		v11 = Com_LocalClient_GetControllerIndex(0);
		Sys_EnterCriticalSection(CRITSECT_BLACKBOX);
		if (!s_blackboxIsInteresting)
			goto LABEL_27;
		v12 = Sys_Milliseconds();
		v13 = bb_msg_header;
		bb_msg_header->sendTime = v12;
		if (bb_msg.overflow)
			v13->flags |= 1u;
		if (bb_msg.pppHashTable)
			bb_msg.data[bb_msg.pppBitOffset] = bb_msg.pppBits << (8 - LOBYTE(bb_msg.pppBitCount));
		if (dwRecordEvent(v11, DW_EVENT_BLACKBOX, bb_msg.data, bb_msg.cursize))
			LABEL_27:
		BB_SetupMsg();
		Sys_LeaveCriticalSection(CRITSECT_BLACKBOX);
	}
}

char BB_CheckThrottle(const char* name)
{
	return 0;
}

void BB_Disable_f()
{
}

void BB_Enable_f()
{
}

unsigned int BB_HashString(const char* str, int len)
{
	return 0;
}

void BB_Init()
{
	#ifdef _DEBUG
	if (s_blackboxInitialized
		&& !(unsigned __int8)Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(s_blackboxInitialized == 0)",
			&pBlock))
	{
		__debugbreak();
	}
	#endif
	Cmd_AddCommandInternal("bbstart", BB_Start_f, &BB_Start_f_VAR);
	Cmd_AddCommandInternal("bbsend", BB_Send_f, &BB_Send_f_VAR);
	Cmd_AddCommandInternal("bbenable", BB_Enable_f, &BB_Enable_f_VAR);
	Cmd_AddCommandInternal("bbdisable", BB_Disable_f, &BB_Disable_f_VAR);
	Cmd_AddCommandInternal("bbthrottle", BB_Throttle_f, &BB_Throttle_f_VAR);
	blackboxBandwidthLimited = _Dvar_RegisterBool(
		"blackboxBandwidthLimited",
		0,
		0x20000u,
		"When true, limits blackbox sending so that it only occurs when not in game");
	blackboxHighVolumeProbability = _Dvar_RegisterFloat(
		"blackboxHighVolumeProbability",
		0.0,
		0.0,
		1.0,
		0x20000u,
		"Probability of sending high volume blackbox data");
	s_connectionsHash = BB_HashString("connections", 11);
	s_sessionsHash = BB_HashString("sessions", 8);
}

void BB_InitDefinitions()
{
}

void BB_InitStringCache()
{
	memset((char*)s_stringHashTable, 0, 0x1000u);
	s_stringHits = 0;
	s_stringMisses = 0;
	s_stringCollisions = 0;
	s_stringHitBytes = 0;
}

void BB_MsgInit(bb_msg_t* msg, void* data, int maxsize)
{
}

void BB_Notify(bool daily, const char* recipient, const char* msg, bool immediate)
{
}

char BB_ParseParameter(const char* string, int len)
{
	return 0;
}

void BB_Print(ControllerIndex_t controllerIndex, const char* name, const char* fmt, ...)
{
}

int BB_RegisterHighWaterMark(const char* name)
{
	return 0;
}

void BB_ResetHighWaterMark(const char* name)
{
}

void BB_ResetHighWaterMarks()
{
}

void BB_Send(ControllerIndex_t controllerIndex, bool force, bool bandwidthLimited)
{
}

void BB_Send_f()
{
}

void BB_SetHighWaterMark(int idx, unsigned int value)
{
}

void BB_SetThrottle(const char* name, int throttle)
{
}

void BB_SetupMsg()
{
}

void BB_Start()
{
	int v0; // edi
	int v1; // esi
	int v2; // ebx

	if (s_blackboxInitialized
		&& !(unsigned __int8)Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(s_blackboxInitialized == 0)",
			&pBlock))
	{
		__debugbreak();
	}
	v0 = _time64(0);
	Sys_EnterCriticalSection(CRITSECT_BLACKBOX);
	BB_InitDefinitions();
	BB_InitStringCache();
	s_bbmac = 0i64;
	v1 = rand();
	v2 = rand();
	Com_PrintWarning(v1, 10, "Could not determine MAC address, using random default\n");
	LODWORD(s_bbid) = v2;
	HIDWORD(s_bbid) = 4 * v0 ^ v1;
	s_blackboxInitialized = 1;
	BB_SetupMsg();
	Sys_LeaveCriticalSection(CRITSECT_BLACKBOX);
}

void BB_Start_f()
{
	float prob; // ST10_4

	if (!s_blackboxInitialized)
	{
		prob = random();
		if (Dvar_GetFloat(blackboxHighVolumeProbability) <= prob)
			Cmd_ExecuteSingleCommand((LocalClientNum_t)0, (ControllerIndex_t)0, "exec blackbox_low.cfg", 0);
		else
			Cmd_ExecuteSingleCommand((LocalClientNum_t)0, (ControllerIndex_t)0, "exec blackbox.cfg", 0);
		BB_Start();
	}
}

void BB_Throttle_f()
{
}

void BB_WriteFormatParameters(bb_msg_t* msg, int parameterCount, const char* parameters, char* args)
{
}

void BB_WriteInstanceData(ControllerIndex_t controllerIndex)
{
}

void BB_WriteInt8(bb_msg_t* msg, int c)
{
}

void BB_WriteString(bb_msg_t* msg, const char* str, int len)
{
}

void BB_WriteVarUInt32(bb_msg_t* msg, unsigned int c)
{
}

void BB_WriteVarUInt64(unsigned long long c)
{
}

double random()
{
	return (double)rand() * 0.000030517578;
}
