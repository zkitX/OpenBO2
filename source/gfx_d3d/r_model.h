#pragma once

#include <universal/com_math.h>

struct GfxPlacement
{
	vec4_t quat;
	vec3_t origin;
};

struct GfxScaledPlacement
{
	GfxPlacement base;
	float scale;
};