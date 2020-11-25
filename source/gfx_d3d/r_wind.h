#pragma once

#include <d3d11.h>
#include <universal/com_math.h>

struct WindState
{
	vec3_t windDirection;
	float windSpringAngle[16];
	vec3_t windRustleAngle[16];
	float speed;
};
