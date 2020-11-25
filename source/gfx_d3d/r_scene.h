#pragma once
#include "r_light.h"
#include "r_meshdata.h"
#include "r_qrcode.h"
#include "r_rendercmds.h"
#include "r_ui3d.h"
#include "rb_postfx.h"

#include <qcommon/com_clients.h>
#include <universal/com_math.h>

enum ShadowType
{
	SHADOW_NONE = 0x0,
	SHADOW_MAP = 0x1,
};

enum OcclusionReportsMode
{
	OCCLUSION_REPORTS_DISABLED = 0x0,
	OCCLUSION_REPORTS_RECORDING = 0x1,
	OCCLUSION_REPORTS_TESTING = 0x2,
};

struct GfxWorldVertexData0
{
	char* data;
	ID3D11Buffer* vb;
};

struct GfxWorldVertexData1
{
	char* data;
	ID3D11Buffer* vb;
};

struct GfxVolumePlane
{
	vec4_t plane;
};

struct Occluder
{
	unsigned int flags;
	char name[16];
	vec3_t points[4];
};

struct GfxOutdoorBounds
{
	vec3_t bounds[2];
};

struct GfxSceneDynModel
{
	XModelDrawInfo info;
	unsigned __int16 dynEntId;
	char primaryLightIndex;
	char reflectionProbeIndex;
};

struct GfxExposureVolume
{
	unsigned int control;
	float exposure;
	float luminanceIncreaseScale;
	float luminanceDecreaseScale;
	float featherRange;
	float featherAdjust;
};

struct GfxLutVolume
{
	vec3_t mins;
	unsigned int control;
	vec3_t maxs;
	float lutTransitionTime;
	unsigned int lutIndex;
};

#pragma region DpvsPlanes
struct GfxWorldDpvsPlanes
{
	int cellCount;
	cplane_s* planes;
	unsigned __int16* nodes;
	unsigned int* sceneEntCellBits;
};

struct DpvsPlane
{
	vec4_t coeffs;
	char side[3];
	char pad;
};

struct DpvsPlanes
{
	const DpvsPlane* planes;
	int count;
};
#pragma endregion

#pragma region Brush
struct BModelDrawInfo
{
	unsigned __int16 surfId;
};

struct GfxSceneDynBrush
{
	BModelDrawInfo info;
	unsigned __int16 dynEntId;
};

struct GfxSceneGlassBrush
{
	GfxPlacement placement;
	const GfxBrushModel* bmodel;
	Material* altStreamingMaterial;
	BModelDrawInfo info;
	char reflectionProbeIndex;
};

struct GfxSceneBrush
{
	BModelDrawInfo info;
	unsigned __int16 entnum;
	const GfxBrushModel* bmodel;
	GfxPlacement placement;
	const ShaderConstantSet* brushConstantSet;
	char reflectionProbeIndex;
};

#pragma endregion

struct GfxViewRenderControl
{
	char mainSceneMSAA;
	char mainScene;
	char mainSceneFinal;
	char mainSceneSaved;
	char extraCamSamplerRenderTarget;
	char ui3d;
	unsigned int renderingMode;
	unsigned int additionalPostFX;
};

struct GfxEntity
{
	unsigned int renderFxFlags;
	float materialTime;
	float destructibleBurnAmount;
	int textureOverrideIndex;
};

struct GfxLodRamp
{
	float scale;
	float bias;
	float appliedInvScale;
};

struct GfxSceneDef
{
	int time;
	float floatTime;
	vec3_t viewOffset;
};

#pragma region FOG
struct GfxFog
{
	int startTime;
	int finishTime;
	vec4_t color;
	float fogStart;
	float density;
	float heightDensity;
	float baseHeight;
	vec4_t sunFogColor;
	vec3_t sunFogDir;
	float sunFogStartAng;
	float sunFogEndAng;
	float maxDensity;
};

struct GfxWorldFogModifierVolume
{
	unsigned int control;
	unsigned __int16 minX;
	unsigned __int16 minY;
	unsigned __int16 minZ;
	unsigned __int16 maxX;
	unsigned __int16 maxY;
	unsigned __int16 maxZ;
	unsigned int controlEx;
	float transitionTime;
	float depthScale;
	float heightScale;
	vec4_t colorAdjust;
};

struct GfxWorldFog
{
	float baseDist;
	float halfDist;
	float baseHeight;
	float halfHeight;
	float sunFogPitch;
	float sunFogYaw;
	float sunFogInner;
	float sunFogOuter;
	vec3_t fogColor;
	float fogOpacity;
	vec3_t sunFogColor;
	float sunFogOpacity;
};

struct GfxWorldFogVolume
{
	vec3_t mins;
	unsigned int control;
	vec3_t maxs;
	float fogTransitionTime;
	unsigned int controlEx;
	GfxWorldFog volumeWorldFog[1];
};

struct GfxClientFog
{
	GfxFog settings[6];
	int index;
};
#pragma endregion

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

struct GfxDrawSurfListInfo
{
	const GfxDrawSurf* drawSurfs;
	unsigned int drawSurfCount;
	char baseTechType;
	const GfxViewInfo* viewInfo;
	vec4_t viewOrigin;
	OcclusionReportsMode occlusionReportsMode;
	int cameraView;
	int disableAllShadowTech;
	int needResolve;
};

struct GfxWorldDraw
{
	unsigned int reflectionProbeCount;
	union {
		GfxReflectionProbe* localReflectionProbes;
		GfxReflectionProbe* reflectionProbes;
	};
	union {
		GfxTexture* localReflectionProbeTextures;
		GfxTexture* reflectionProbeTextures;
	};
	int lightmapCount;
	union {
		GfxTexture* localLightmapPrimaryTextures;
		GfxTexture* lightmapPrimaryTextures;
	};
	union {
		GfxTexture* localLightmapPrimaryTextures;
		GfxTexture* lightmapPrimaryTextures;
	};
	union {
		GfxTexture* localLightmapSecondaryTextures;
		GfxTexture* lightmapSecondaryTextures;
	};
	unsigned int vertexCount;
	unsigned int vertexDataSize0;
	GfxWorldVertexData0 vd0;
	unsigned int vertexDataSize1;
	GfxWorldVertexData1 vd1;
	int indexCount;
	unsigned __int16* indices;
	ID3D11Buffer* indexBuffer;
};

struct GfxWorldDpvsStatic
{
	unsigned int smodelCount;
	unsigned int staticSurfaceCount;
	unsigned int litSurfsBegin;
	unsigned int litSurfsEnd;
	unsigned int litTransSurfsBegin;
	unsigned int litTransSurfsEnd;
	unsigned int emissiveOpaqueSurfsBegin;
	unsigned int emissiveOpaqueSurfsEnd;
	unsigned int emissiveTransSurfsBegin;
	unsigned int emissiveTransSurfsEnd;
	unsigned int smodelVisDataCount;
	unsigned int surfaceVisDataCount;
	char* smodelVisData[3];
	char* surfaceVisData[3];
	char* smodelVisDataCameraSaved;
	char* surfaceVisDataCameraSaved;
	unsigned __int16* sortedSurfIndex;
	GfxStaticModelInst* smodelInsts;
	GfxSurface* surfaces;
	GfxStaticModelDrawInst* smodelDrawInsts;
	GfxDrawSurf* surfaceMaterials;
	char* surfaceCastsSunShadow;
	char* surfaceCastsShadow;
	char* smodelCastsShadow;
	volatile int usageCount;
};

struct GfxWorldDpvsDynamic
{
	unsigned int dynEntClientWordCount[2];
	unsigned int dynEntClientCount[2];
	unsigned int* dynEntCellBits[2];
	char* dynEntVisData[2][3];
	volatile int usageCount;
};

struct GfxViewInfo
{
	GfxViewParms viewParms;
	GfxViewport sceneViewport;
	GfxViewport displayViewport;
	GfxViewport scissorViewport;
	GfxSceneDef sceneDef;
	ShadowType dynamicShadowType;
	LocalClientNum_t localClientNum;
	int viewInfoIndex;
	int isRenderingFullScreen;
	int playerTeleported;
	GfxLight visibleLights[4];
	int visibleLightCount;
	vec4_t frustumPlanes[4];
	GfxLightScale charPrimaryLightScale;
	GfxPostFxInfo* postFxInfo;
	const void* cmds;
	float sunVisibility;
	float adsZScale;
	GfxQuadMeshData* fullSceneViewMesh;
	GfxDrawSurfListInfo* drawList;
	GfxCmdBufInput* input;
	char hdrRenderingMode;
	GfxViewRenderControl sceneComposition;
	GfxUI3DBackend rbUI3D;
	GfxQRCodeBackend rbQRCode;
	bool isExtraCamera;
	int noLodCullOut;
	float lodScaleRigid;
	float lodScaleSkinned;
	float postEmissiveBrightening;
	GfxViewParms weaponViewParms;
	bool sonarRenderTargetInUse;
	float sonarBlur;
	GfxViewParms sonarViewParms;
	bool sonarUseThermalMaterial;
	bool sonarOverrideBlend;
	bool drawInfrared;
	bool drawPostBlur;
	bool drawWaypointsPostBloom;
	int splitscreenBlurEdges;
	bool needResolveOpaque;
	__int16 lutScriptIndex;
	bool drawDarkness;
};
