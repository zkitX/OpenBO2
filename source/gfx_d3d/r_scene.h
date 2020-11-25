#pragma once

#include <universal/com_math.h>

struct GfxLodRamp
{
	float scale;
	float bias;
	float appliedInvScale;
};

struct ExtraCamLodInfo
{
	vec3_t pos[4];
	unsigned int posValidBits;
};

struct GfxLodParms
{
	vec3_t origin;
	GfxLodRamp ramp[2];
	GfxLodRamp rampExtracam[2];
	bool valid;
	ExtraCamLodInfo extracamLod;
};
