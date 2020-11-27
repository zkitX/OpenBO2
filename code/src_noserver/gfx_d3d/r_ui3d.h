#pragma once
#include "r_state.h"
#include "r_scene.h"

#include <universal/com_math.h>

struct GfxUI3DBackend
{
	GfxViewport viewport[6];
	vec4_t uvSetup[6];
	int renderCmdCount[6];
	int renderCmdThatDrawsSomethingCount[6];
	int totalRenderCmds;
	int totalRenderCmdsThatDrawsSomething;
	float blurRadius;
};
