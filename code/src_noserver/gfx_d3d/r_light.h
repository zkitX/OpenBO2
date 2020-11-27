#pragma once
#include "r_shared.h"

struct GfxLightScale
{
	float diffuseScale;
	float specularScale;
};

struct GfxLightingInfo
{
	char primaryLightIndex;
	char reflectionProbeIndex;
	unsigned __int16 lightingHandle;
};

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

struct GfxShadowGeometry
{
	unsigned __int16 surfaceCount;
	unsigned __int16 smodelCount;
	unsigned __int16* sortedSurfIndex;
	unsigned __int16* smodelIndex;
};

#pragma region GfxLightGrid
struct GfxCompressedLightGridCoeffs
{
	unsigned __int16 coeffs[9][3];
};
struct GfxCompressedLightGridColors
{
	char rgb[56][3];
};
struct GfxLightGridRow
{
	unsigned __int16 colStart;
	unsigned __int16 colCount;
	unsigned __int16 zStart;
	unsigned __int16 zCount;
	unsigned int firstEntry;
};
struct GfxLightGridHeader
{
	unsigned __int16 mins[3];
	unsigned __int16 maxs[3];
	unsigned int rowAxis;
	unsigned int colAxis;
	unsigned __int16 rowDataStart[8192];
};
struct GfxLightGridEntry
{
	unsigned __int16 colorsIndex;
	char primaryLightIndex;
	char visibility;
};
struct GfxLightGrid
{
	unsigned int sunPrimaryLightIndex;
	unsigned __int16 mins[3];
	unsigned __int16 maxs[3];
	float offset;
	unsigned int rowAxis;
	unsigned int colAxis;
	unsigned __int16* rowDataStart;
	unsigned int rawRowDataSize;
	char* rawRowData;
	unsigned int entryCount;
	GfxLightGridEntry* entries;
	unsigned int colorCount;
	GfxCompressedLightGridColors* colors;
	unsigned int coeffCount;
	GfxCompressedLightGridCoeffs* coeffs;
	unsigned int skyGridVolumeCount;
	GfxSkyGridVolume* skyGridVolumes;
};
#pragma endregion

#pragma region GfxLightingSH
struct GfxLightingSH
{
	vec4_t V0;
	vec4_t V1;
	vec4_t V2;
};
struct GfxLightingSHQuantized
{
	unsigned __int16 V0[4];
	unsigned __int16 V1[4];
	unsigned __int16 V2[4];
};
struct GfxLightingSHAndVis
{
	GfxLightingSH sh;
	float vis;
};
#pragma endregion

#pragma region LightRegion
struct GfxLightRegionAxis
{
	vec3_t dir;
	float midPoint;
	float halfSize;
};
struct GfxLightRegionHull
{
	float kdopMidPoint[9];
	float kdopHalfSize[9];
	unsigned int axisCount;
	GfxLightRegionAxis* axis;
};
struct GfxLightRegion
{
	unsigned int hullCount;
	GfxLightRegionHull* hulls;
};
#pragma endregion

#pragma region HeroLight
struct GfxHeroLight
{
	char type;
	char unused[3];
	vec3_t color;
	vec3_t dir;
	vec3_t origin;
	float radius;
	float cosHalfFovOuter;
	float cosHalfFovInner;
	int exponent;
};

struct GfxHeroLightTree
{
	vec3_t mins;
	vec3_t maxs;
	int leftNode;
	int rightNode;
};
#pragma endregion
