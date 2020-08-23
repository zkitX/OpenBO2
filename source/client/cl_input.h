#pragma once

#include "../cgame/cg_main.h"

struct clientMouse_t
{
	int mouseDx[2];
	int mouseDy[2];
	int mouseIndex;
} mouseIn;

int CL_MouseEvent(LocalClientNum_t localClientNum, int x, int y, int dx, int dy);