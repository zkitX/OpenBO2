#pragma once
#include "r_init.h"
#include "r_state.h"

#include <d3d11.h>
#include <universal/com_math.h>

struct GfxSunShadowProjection
{
	float viewMatrix[4][4];
	vec4_t switchPartition;
	vec4_t shadowmapScale;
};

struct GfxSunShadowBoundingPoly
{
	vec2_t snapDelta;
	int pointCount;
	vec2_t points[9];
	int pointIsNear[9];
};

struct GfxSunShadowPartition
{
	GfxViewParms shadowViewParms;
	int partitionIndex;
	GfxViewport viewport;
	GfxSunShadowBoundingPoly boundingPoly;
};

struct GfxSunShadow
{
	GfxMatrix lookupMatrix;
	GfxSunShadowProjection sunProj;
	int sunShadowCount;
	GfxSunShadowPartition partition[2];
};

struct GfxShadowMapVolume
{
	unsigned int control;
	unsigned int padding1;
	unsigned int padding2;
	unsigned int padding3;
};
