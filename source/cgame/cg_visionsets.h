#pragma once
#include <gfx_d3d/r_utils.h>

enum visionSetLerpStyle_t
{
	VISIONSETLERP_UNDEFINED = 0x0,
	VISIONSETLERP_NONE = 0x1,
	VISIONSETLERP_TO_LINEAR = 0x2,
	VISIONSETLERP_TO_SMOOTH = 0x3,
	VISIONSETLERP_BACKFORTH_LINEAR = 0x4,
	VISIONSETLERP_BACKFORTH_SMOOTH = 0x5,
	VISIONSETLERP_CUSTOM_VALUE = 0x6,
};

struct visionSetVars_t
{
	bool filmEnable;
	vec4_t visBloomLevelsRGBYInB;
	vec4_t visBloomLevelsRGBYInG;
	vec4_t visBloomLevelsRGBYInW;
	vec4_t visBloomLevelsRGBYOutB;
	vec4_t visBloomLevelsRGBYOutW;
	vec4_t visBloomRGBHi;
	vec4_t visBloomRGBLo;
	vec4_t visBloomYHi;
	vec4_t visBloomYLo;
	vec4_t visColorRangeS;
	vec4_t visColorRangeE;
	vec4_t visColorShadowMatrixR;
	vec4_t visColorShadowMatrixG;
	vec4_t visColorShadowMatrixB;
	vec4_t visColorMidtoneMatrixR;
	vec4_t visColorMidtoneMatrixG;
	vec4_t visColorMidtoneMatrixB;
	vec4_t visColorHilightMatrixR;
	vec4_t visColorHilightMatrixG;
	vec4_t visColorHilightMatrixB;
	vec4_t visColorFinalGamma;
	vec4_t visColorFinalSaturation;
	vec4_t visColorFinalBlend;
	float filmLut;
	bool reviveEnable;
	float reviveEdgeColorTemp;
	float reviveEdgeSaturation;
	vec3_t reviveEdgeScale;
	vec3_t reviveEdgeContrast;
	vec3_t reviveEdgeOffset;
	float reviveEdgeMaskAdjust;
	float reviveEdgeAmount;
	bool charPrimaryUseTweaks;
	float charPrimaryDiffuseScale;
	float charPrimarySpecularScale;
	float masterRingmod;
	float reverbRingmod;
	float hiFilter;
	float lowFilter;
	vec4_t sunFlareTint;
	float postEmissiveBrightening;
	char name[64];
};

struct visionSetLerpData_t
{
	int timeStart;
	int timeDuration;
	float lerpFraction;
	visionSetLerpStyle_t style;
};

class cg_visionsets
{
};

