#pragma once
#include "r_shared.h"

struct GfxReflectionProbeStreamPart
{
	vec3_t mins;
	float himipRadiusInvSq;
	vec3_t maxs;
};

struct GfxReflectionProbeVolumeData
{
	vec4_t volumePlanes[6];
};

struct GfxReflectionProbe
{
	vec3_t origin;
	GfxLightingSH lightingSH;
	GfxImage* reflectionImage;
	GfxReflectionProbeVolumeData* probeVolumes;
	unsigned int probeVolumeCount;
	float mipLodBias;
};