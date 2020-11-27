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

struct SunFlareDynamic
{
	float flareIntensity;
	float sunVisibilityAdjust;
	float currentBlind;
	float currentGlare;
	int lastTime;
	float lastVisibility;
	float lastDot;
	unsigned __int16 occlusionQueryHandle;
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

#pragma region DVARS
const dvar_t* r_sunglare_max_lighten;
const dvar_t* r_sunsprite_shader;
const dvar_t* r_sunsprite_size;
const dvar_t* r_sunflare_min_size;
const dvar_t* r_sunblind_min_angle;
const dvar_t* r_sunblind_fadeout;
const dvar_t* r_sun_fx_position;
const dvar_t* r_sunglare_fadein;
const dvar_t* r_sunglare_fadeout;
const dvar_t* r_sunflare_fadein;
const dvar_t* r_sunblind_fadein;
const dvar_t* r_sunflare_fadeout;
const dvar_t* r_sunblind_max_darken;
const dvar_t* r_sunflare_max_angle;
const dvar_t* r_sunflare_max_alpha;
const dvar_t* r_sunblind_max_angle;
const dvar_t* r_sunglare_max_angle;
const dvar_t* r_sunglare_min_angle;
const dvar_t* r_sunflare_min_angle;
const dvar_t* r_sunflare_max_size;
const dvar_t* r_sunflare_shader;
#pragma endregion

const char* sunDvarNames[]{
	"r_sunsprite_shader",
	"r_sunsprite_size",
	"r_sunflare_shader",
	"r_sunflare_min_size",
	"r_sunflare_min_angle",
	"r_sunflare_max_size",
	"r_sunflare_max_angle",
	"r_sunflare_max_alpha",
	"r_sunflare_fadein",
	"r_sunflare_fadeout",
	"r_sunblind_min_angle",
	"r_sunblind_max_angle",
	"r_sunblind_max_darken",
	"r_sunblind_fadein",
	"r_sunblind_fadeout",
	"r_sunglare_min_angle",
	"r_sunglare_max_angle",
	"r_sunglare_max_lighten",
	"r_sunglare_fadein",
	"r_sunglare_fadeout",
	"r_sun_fx_position"
};

SunFlareDynamic sunFlareArray[4];

void R_RegisterSunDvars();
void R_SetSunFromDvars(sunflare_t* sun);
void R_LoadSunThroughDvars(char const*, sunflare_t*);
void R_FlushSun();
