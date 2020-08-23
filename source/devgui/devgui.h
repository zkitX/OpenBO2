#pragma once

#include "../universal/dvar.h"

enum DevEventType
{
	EVENT_ACTIVATE = 0x0,
	EVENT_DEACTIVATE = 0x1,
	EVENT_ACCEPT = 0x2,
	EVENT_UPDATE = 0x3,
	EVENT_DRAW = 0x4,
	EVENT_SAVE = 0x5,
};

struct DevGraph
{
	vec2_t* knots;
	int* knotCount;
	int knotCountMax;
	int selectedKnot;
	void(__cdecl* eventCallback)(DevGraph*, DevEventType, LocalClientNum_t);
	void(__cdecl* textCallback)(DevGraph*, const float, const float, char*, const int);
	void* data;
	bool disableEditingEndPoints;
};

union DevMenuChild
{
	const char* command;
	const dvar_t* dvar;
	DevGraph* graph;
	unsigned __int16 menu;
};

struct DevMenuItem
{
	char label[26];
	char childType;
	char childMenuMemory;
	__int16 sortKey;
	unsigned __int16 nextSibling;
	unsigned __int16 prevSibling;
	unsigned __int16 parent;
	DevMenuChild child;
} d_menu;

bool DevGuiActive;
unsigned __int16 handle;

bool DevGui_IsActive();
void DevGui_Toggle();