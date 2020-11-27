#pragma once

#include <gfx_d3d/r_utils.h>

struct PlayerVehicleState
{
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	vec3_t angVelocity;
	vec2_t tilt;
	vec2_t tiltVelocity;
	float targetHeightDelta;
	float lastGroundHeight;
	int entity;
	int flags;
	bool fullPhysics;
};


class cg_vehicle
{
};

