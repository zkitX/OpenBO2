#pragma once

enum LastInput_t
{
	LAST_INPUT_GAMEPAD = 0x0,
	LAST_INPUT_MOUSEMOVE = 0x1,
	LAST_INPUT_MOUSECLICK = 0x2,
	LAST_INPUT_KEYBOARD = 0x3,
};

enum LocalClientNum_t
{
	INVALID_LOCAL_CLIENT = 0xFFFFFFFF,
	LOCAL_CLIENT_FIRST = 0x0,
	LOCAL_CLIENT_0 = 0x0,
	ONLY_LOCAL_CLIENT = 0x0,
	LOCAL_CLIENT_COUNT = 0x1,
};

LocalClientNum_t primaryLocalClient;
long long qword_1DB4408[]; // UNKNOWN
int resetTime;

void Com_LocalClient_LastInput_Set(LocalClientNum_t localClientNum, LastInput_t currentInput);
LocalClientNum_t Com_LocalClients_GetPrimary();