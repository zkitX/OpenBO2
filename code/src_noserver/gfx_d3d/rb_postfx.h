#pragma once
#include "r_material.h"
#include "rb_backend.h"

#include <universal/com_math.h>

#pragma region FXTypes
struct GfxDepthOfField
{
	float viewModelStart;
	float viewModelEnd;
	float nearStart;
	float nearEnd;
	float farStart;
	float farEnd;
	float nearBlur;
	float farBlur;
	bool useAltMaterial;
};

struct GfxFilm
{
	bool enabled;
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
	__int16 lutA;
	__int16 lutB;
	__int16 lutC;
	__int16 lutD;
	vec4_t lutBlendWeights;
	float filmLut;
	vec4_t sunFlareTint;
};

struct GfxCompositeFx
{
	vec2_t distortionScale;
	float blurRadius;
	float distortionMagnitude;
	float frameRate;
	int lastUpdate;
	int frame;
	int startMSec;
	int currentTime;
	int duration;
	bool enabled;
	bool scriptEnabled;
};

struct GfxReviveFx
{
	bool enabled;
	float reviveEdgeColorTemp;
	float reviveEdgeSaturation;
	vec3_t reviveEdgeScale;
	vec3_t reviveEdgeContrast;
	vec3_t reviveEdgeOffset;
	float reviveEdgeMaskAdjust;
	float reviveEdgeAmount;
};

struct GfxDoubleVision
{
	vec3_t direction;
	float motionBlurMagnitude;
	float deltaPerMS;
	float cur;
	float targ;
};

struct GfxGenericFilter
{
	bool passEnabled[8][4];
	Material* passMaterial[8][4];
	int passTarget[8][4];
	int passSampler0[8][4];
	int passSampler1[8][4];
	float passParam[8][4][16];
	int passQuads[8][4];
	int bitFlag[8];
};

struct GfxPoison
{
	float curAmountTarget;
	float curAmount;
};

struct GfxSaveScreenFx
{
	GfxSaveScreenParam saveScreenParam;
	GfxBlendSaveScreenBlurredParam blendBlurredParam;
	GfxBlendSaveScreenFlashedParam blendFlashedParam;
};

struct GfxBloom
{
	vec4_t visBloomLevelsRGBYInB;
	vec4_t visBloomLevelsRGBYInG;
	vec4_t visBloomLevelsRGBYInW;
	vec4_t visBloomLevelsRGBYOutB;
	vec4_t visBloomLevelsRGBYOutW;
	vec4_t visBloomRGBHi;
	vec4_t visBloomRGBLo;
	vec4_t visBloomYHi;
	vec4_t visBloomYLo;
};
#pragma endregion

struct GfxPostFxInfo
{
	GfxDepthOfField dof;
	GfxFilm film;
	GfxCompositeFx flameFx;
	GfxReviveFx reviveFx;
	GfxCompositeFx waterSheetingFx;
	GfxDoubleVision doubleVision;
	GfxGenericFilter genericFilter;
	GfxPoison poisonFx;
	GfxCompositeFx electrifiedFx;
	GfxCompositeFx transportedFx;
	GfxSaveScreenFx saveScreenFx;
	GfxBloom bloom;
	float bloomExpansionXpos;
	float bloomExpansionYpos;
	float blurRadius;
	float exposureValue;
	float exposureRemap;
	bool useColorControl;
	float colorSaturation;
	float colorTemp;
	float colorHue;
	vec3_t colorScale;
};

