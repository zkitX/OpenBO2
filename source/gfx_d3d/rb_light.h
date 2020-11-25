#pragma once
#include "r_image.h"

#include <d3d11.h>
#include <universal/com_math.h>

struct GfxLightImage
{
	GfxImage* image;
	char samplerState;
};

struct GfxLightDef
{
	const char* name;
	GfxLightImage attenuation;
	int lmapLookupStart;
};

struct GfxLight
{
	char type;
	char canUseShadowMap;
	char shadowmapVolume;
	__int16 cullDist;
	vec3_t color;
	vec3_t dir;
	vec3_t origin;
	float radius;
	float cosHalfFovOuter;
	float cosHalfFovInner;
	int exponent;
	unsigned int spotShadowIndex;
	float dAttenuation;
	float roundness;
	vec3_t angles;
	float spotShadowHiDistance;
	vec4_t diffuseColor;
	vec4_t shadowColor;
	vec4_t falloff;
	vec4_t aAbB;
	vec4_t cookieControl0;
	vec4_t cookieControl1;
	vec4_t cookieControl2;
	float44 viewMatrix;
	float44 projMatrix;
	GfxLightDef* def;
};
