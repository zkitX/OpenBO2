#pragma once
#include "r_model.h"
#include "r_utils.h"

#include <d3d11.h>
#include <universal/com_math.h>

struct GfxParticleCloud
{
	GfxScaledPlacement placement;
	vec3_t velDirWorld;
	GfxColor color;
	vec2_t radius;
	unsigned __int16 count;
	unsigned __int16 offset;
	char gaussian;
	char pad[3];
};