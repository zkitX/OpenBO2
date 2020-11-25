#pragma once
#include "r_shared.h"

struct GfxWorldSun
{
	unsigned int control;
	vec3_t angles;
	vec4_t ambientColor;
	vec4_t sunCd;
	vec4_t sunCs;
	vec4_t skyColor;
	float exposure;
};

struct sunflare_t
{
	bool hasValidData;
	Material* spriteMaterial;
	Material* flareMaterial;
	float spriteSize;
	float flareMinSize;
	float flareMinDot;
	float flareMaxSize;
	float flareMaxDot;
	float flareMaxAlpha;
	int flareFadeInTime;
	int flareFadeOutTime;
	float blindMinDot;
	float blindMaxDot;
	float blindMaxDarken;
	int blindFadeInTime;
	int blindFadeOutTime;
	float glareMinDot;
	float glareMaxDot;
	float glareMaxLighten;
	int glareFadeInTime;
	int glareFadeOutTime;
	vec3_t sunFxPosition;
};

struct SunLightParseParams
{
	char name[64];
	GfxWorldSun initWorldSun[1];
	float fogTransitionTime;
	GfxWorldFog initWorldFog[1];
};

struct GfxSkyDynamicIntensity
{
	float angle0;
	float angle1;
	float factor0;
	float factor1;
};

struct GfxSkyGridVolume
{
	vec3_t mins;
	vec3_t maxs;
	vec3_t lightingOrigin;
	unsigned __int16 colorsIndex;
	char primaryLightIndex;
	char visibility;
};

void R_RegisterSunDvars();
