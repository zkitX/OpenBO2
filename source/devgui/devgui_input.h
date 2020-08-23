#pragma once

#include "../win32/win_gamepad.h"

enum DevGuiInputButton
{
	INPUT_UP = 0x0,
	INPUT_DOWN = 0x1,
	INPUT_LEFT = 0x2,
	INPUT_RIGHT = 0x3,
	INPUT_ACCEPT = 0x4,
	INPUT_REJECT = 0x5,
	INPUT_BIND = 0x6,
	INPUT_GRAPH_EDIT = 0x7,
	INPUT_GRAPH_ADD = 0x8,
	INPUT_GRAPH_REMOVE = 0x9,
	INPUT_GRAPH_SAVE = 0xA,
	INPUT_LINK = 0xB,
	INPUT_RESET = 0xC,
	INPUT_COUNT = 0xD,
};

enum DevGuiInputAxis
{
	SCROLL_XAXIS = 0x0,
	SCROLL_YAXIS = 0x1,
	SCROLL_AXIS_COUNT = 0x2,
};

enum DevGuiInputState
{
	SCROLL_NONE = 0x0,
	SCROLL_PRESSED = 0x1,
	SCROLL_STALLED = 0x2,
	SCROLL_HELD = 0x3,
};

struct DevGuiInput
{
	ControllerIndex_t selectedGamePadIndex;
	ControllerIndex_t gamePadIndex;
	float mousePos[2];
	bool buttonDown[13];
	bool prevButtonDown[13];
	float scrollScale;
	DevGuiInputState digitalStates[2];
	float digitalAxis[2];
	float digitalTimes[2];
	DevGuiInputState analogStates[2];
	float analogAxis[2];
	float analogTimes[2];
	__int16 menuScroll[2];
	float menuScrollTime[2];
	float digitalSliderTime;
	float analogSliderTime;
	float sliderScrollTime;
	float sliderScrollMaxTimeStep;
};

DevGuiInput s_input;

float i_fsign(float val);
bool DevGui_IsButtonDown(DevGuiInputButton button);
void DevGui_MouseEvent(int dx, int dy);
void DevGui_InputInit();