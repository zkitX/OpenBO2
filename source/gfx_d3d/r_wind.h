#pragma once
#include "r_shared.h"

#include <d3d11.h>
#include <universal/com_math.h>

struct WindState
{
	vec3_t windDirection;
	float windSpringAngle[16];
	vec3_t windRustleAngle[16];
	float speed;
};

WindState frontendWind;

void R_SetWindDirection(vec3_t* dir, float speed);
void R_SetVariantWindSpringAngle(unsigned int index, float angle);
void R_SetVariantWindRustleAngle(unsigned int, vec3_t*);
void R_SetWindShaderConstants(GfxCmdBufInput*);
void R_CopyWindStateToBackEnd();
