#pragma once
#include <Xinput.h>
#include <cstdint>

#include <qcommon/com_clients.h>
#include <universal/dvar.h>

enum ControllerIndex_t
{
	INVALID_CONTROLLER_PORT = 0xFFFFFFFF,
	CONTROLLER_INDEX_FIRST = 0x0,
	CONTROLLER_INDEX_0 = 0x0,
	CONTROLLER_INDEX_COUNT = 0x1,
};

enum GamePadButton
{
	GPAD_NONE = 0x0,
	GPAD_UP = 0x10000001,
	GPAD_DOWN = 0x10000002,
	GPAD_LEFT = 0x10000004,
	GPAD_RIGHT = 0x10000008,
	GPAD_START = 0x10000010,
	GPAD_BACK = 0x10000020,
	GPAD_L3 = 0x10000040,
	GPAD_R3 = 0x10000080,
	GPAD_A = 0x10001000,
	GPAD_B = 0x10002000,
	GPAD_X = 0x10004000,
	GPAD_Y = 0x10008000,
	GPAD_L_SHLDR = 0x10000100,
	GPAD_R_SHLDR = 0x10000200,
	GPAD_L_TRIG = 0x20000000,
	GPAD_R_TRIG = 0x20000001,
};

enum GamePadButtonEvent
{
	GPAD_BUTTON_RELEASED = 0x0,
	GPAD_BUTTON_PRESSED = 0x1,
	GPAD_BUTTON_UPDATE = 0x2,
};

enum GamePadStick
{
	GPAD_INVALID = 0x0,
	GPAD_LX = 0x40000000,
	GPAD_LY = 0x40000001,
	GPAD_RX = 0x40000002,
	GPAD_RY = 0x40000003,
};

enum GamePadStickDir
{
	GPAD_STICK_POS = 0x0,
	GPAD_STICK_NEG = 0x1,
	GPAD_STICK_DIRCOUNT = 0x2,
};

struct GamePad
{
	bool enabled;
	bool keyboardEnabled;
	__int16 digitals;
	__int16 lastDigitals;
	float analogs[2];
	float lastAnalogs[2];
	float sticks[4];
	float lastSticks[4];
	bool stickDown[4][2];
	bool stickDownLast[4][2];
	float lowRumble;
	float highRumble;
	struct { 
		_XINPUT_VIBRATION Rumble;
	} feedback;
	_XINPUT_CAPABILITIES caps;
	_XINPUT_CAPABILITIES keyboardCaps;
};

struct StickToCodeMap_t {
	GamePadStick padStick;
	GamePadStickDir padStickDir;
	int code;
};

struct ButtonToCodeMap_t {
	GamePadButton padButton;
	int code;
};

const uint32_t _mask__AbsFloat_ = 0x7FFFFFFF;
const uint32_t _mask__NegFloat_ = 0x80000000;

const dvar_t* gpad_button_deadzone;
GamePad s_gamePads[1];

void GPad_UpdateDigitals(ControllerIndex_t portIndex, const _XINPUT_GAMEPAD* xpad);
void GPad_UpdateAnalogs(ControllerIndex_t portIndex, const _XINPUT_GAMEPAD* xpad);
void GPad_ConvertStickToFloat(float* outX, float* outY, __int16 inX, __int16 inY);
void GPad_UpdateSticksDown(GamePad* gPad);
void GPad_UpdateSticks(ControllerIndex_t portIndex, const _XINPUT_GAMEPAD* xpad);
void GPad_StopRumbles(ControllerIndex_t portIndex);
bool GPad_InUse(LocalClientNum_t localClientNum);
bool GPad_IsActive(ControllerIndex_t portIndex);
double GPad_GetButton(ControllerIndex_t portIndex, GamePadButton button);
bool GPad_ButtonRequiresUpdates(ControllerIndex_t portIndex, GamePadButton button);
signed int GPad_IsButtonPressed(ControllerIndex_t portIndex, GamePadButton button);
signed int GPad_IsButtonReleased(ControllerIndex_t portIndex, GamePadButton button);
double GPad_GetStick(ControllerIndex_t portIndex, GamePadStick stick);
void GPad_SetLowRumble(ControllerIndex_t portIndex, float rumble);
void GPad_SetHighRumble(ControllerIndex_t portIndex, float rumble);
bool GPad_GetStickChangedToPressedState(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir, bool pressedState);
bool GPad_IsStickPressed(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir);
bool GPad_IsStickReleased(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir);
void GPad_RefreshAll();
void GPad_UpdateAll();