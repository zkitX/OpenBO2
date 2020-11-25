#pragma once
#include "r_material.h"
#include "r_scene.h"
#include "rb_stats.h"

#include <d3d11.h>
#include <universal/com_math.h>

struct GfxGammaRamp
{
	unsigned __int16 entries[256];
};

struct GfxConfiguration
{
	unsigned int maxClientViews;
	unsigned int entCount;
	unsigned int entnumNone;
	unsigned int entnumOrdinaryEnd;
	int threadContextCount;
	int critSectCount;
};

struct trViewStatistics_t
{
	int c_indexes;
	int c_fxIndexes;
	int c_viewIndexes;
	int c_shadowIndexes;
	int c_vertexes;
	int c_batches;
};

struct Image_MemUsage
{
	int total;
	int lightmap;
	int minspec;
};

struct vidConfig_t
{
	unsigned int sceneWidth;
	unsigned int sceneHeight;
	unsigned int displayWidth;
	unsigned int displayHeight;
	unsigned __int16 outputDisplayWidth;
	unsigned __int16 outputDisplayHeight;
	unsigned int displayFrequency;
	bool isToolMode;
	int isWideScreen;
	int isHiDef;
	int isFullscreen;
	float aspectRatioWindow;
	float aspectRatioScenePixel;
	float aspectRatioDisplayPixel;
	unsigned int maxTextureSize;
	unsigned int maxTextureMaps;
	bool deviceSupportsGamma;
};

struct GfxGlobals
{
	bool startedRenderThread;
	bool isMultiplayer;
	bool isZombie;
	volatile int endFrameFence;
	bool isRenderingRemoteUpdate;
	volatile int screenUpdateNotify;
	volatile int remoteScreenUpdateNesting;
	volatile int remoteScreenUpdateInGame;
	volatile int remoteScreenUpdateLUI;
	char remoteScreenLastSceneResolveTarget;
	int backEndFrameCount;
	char frameBuffer;
};

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

struct GfxMatrix
{
	vec4_t m[4];
};

struct TxaaCtxDX
{
	unsigned int pad[256];
};

struct GfxClientFog
{
	GfxFog settings[6];
	int index;
};

struct DxCmdBuf
{
	int isInUse[33];
	ID3D11DeviceContext* context[33];
	ID3D11CommandList* list[33];
};

struct trStatistics_t
{
	trViewStatistics_t* views;
	Image_MemUsage c_imageUsage;
};

struct GfxWindowTarget
{
	HWND__* hwnd;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* view;
	int width;
	int height;
};

struct GfxViewParms
{
	GfxMatrix viewMatrix;
	GfxMatrix projectionMatrix;
	GfxMatrix viewProjectionMatrix;
	GfxMatrix inverseViewProjectionMatrix;
	vec4_t origin;
	vec3_t axis[3];
	float depthHackNearClip;
	float zNear;
	float zFar;
	int bspCellIndex;
};

struct GfxLodParms
{
	vec3_t origin;
	GfxLodRamp ramp[2];
	GfxLodRamp rampExtracam[2];
	bool valid;
	ExtraCamLodInfo extracamLod;
};

struct DxGlobals
{
	HINSTANCE__* hinst;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11Buffer* constBuffer[4];
	unsigned int vendorId;
	bool adapterNativeIsValid;
	int adapterNativeWidth;
	int adapterNativeHeight;
	int adapterFullscreenWidth;
	int adapterFullscreenHeight;
	bool nvInitialized;
	bool nvStereoActivated;
	void* nvStereoHandle;
	TxaaCtxDX txaaCtx;
	int adapterIndex;
	bool vidRestart;
	bool needResizeBuffers;
	bool doResizeBuffers;
	bool ignoreResize;
	bool needsShaderWarming;
	int lastMaterialWarmed;
	DXGI_FORMAT depthStencilFormat;
	DxCmdBuf cmdBuf[4];
	unsigned int monitorCount;
	unsigned int displayModeCount[8];
	DXGI_MODE_DESC displayModes[8][512];
	const char* resolutionNameTable[513];
	char modeText[10240];
	const char* monitorResolutionNameTable[8][513];
	char monitorModeText[81920];
	ID3D11Query* fencePool[8];
	unsigned int nextFence;
	int gpuCount;
	void(__cdecl* resolutionCallback)(bool);
	unsigned int multiSampleType;
	unsigned int multiSampleQuality;
	unsigned __int16 sunShadowPartition;
	unsigned __int16 spotShadowPartition;
	ID3D11DepthStencilView* singleSampleDepthStencilSurface;
	int targetWindowIndex;
	int windowCount;
	GfxWindowTarget windows[1];
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* backBuffer;
	ID3D11Query* flushGpuQuery;
	int linearMippedAnisotropy;
	int anisotropyFor2x;
	int anisotropyFor4x;
	int mipFilterMode;
	unsigned int mipBias;
};

struct r_globals_t
{
	GfxViewParms identityViewParms;
	bool inFrame;
	bool registered;
	bool forbidDelayLoadImages;
	bool ignorePrecacheErrors;
	vec3_t viewOrg;
	vec3_t viewDir;
	unsigned int frontEndFrameCount;
	int totalImageMemory;
	Material* materialHashTable[4992];
	MaterialUsage materialUsage[4992];
	GfxClientFog clientFogs[4];
	GfxColor color_axis;
	GfxColor color_allies;
	trStatistics_t* globalstats;
	trViewStatistics_t* stats;
	GfxFrameStats frameStats;
	GfxLodParms lodParms;
	GfxLodParms correctedLodParms[4];
	bool useSunLightOverride;
	bool useSunDirOverride;
	bool useSunDirLerp;
	vec3_t diffuseSunLightOverride;
	vec3_t specularSunLightOverride;
	vec3_t sunDirOverride;
	vec3_t sunDirOverrideTarget;
	int sunDirLerpBeginTime;
	int sunDirLerpEndTime;
	const GfxViewParms* debugViewParms;
	int endTime;
	bool distortion;
	bool drawWorld;
	bool drawSModels;
	bool drawXModels;
	bool drawBModels;
	const char* codeImageNames[55];
	unsigned int viewInfoCount;
	int sunShadowFull;
	float sunShadowmapScale;
	float sunShadowmapScaleNum;
	unsigned int sunShadowSize;
	float sunShadowPartitionRatio;
	int drawSunShadow;
	bool isNotRenderingFullScreen;
	bool enablePlayerShadowFlag;
	bool gammaModified;
	float gamma;
	float skinnedVertexCacheUsed[4];
};

DxGlobals dx;
vidConfig_t vidConfig;
GfxConfiguration gfxCfg;
GfxGlobals r_glob;
r_globals_t rg;

bool g_allocateMinimalResources;
int g_disableRendering;

bool R_Is3DOn();
const char* R_ErrorDescription(int hr);
bool R_IsNullRenderer();
char* R_GetFontPathFromName(const char* fontName);
void R_SetBrightness(float brightness);
void R_SetColorMappings();
void R_GammaCorrect(char* buffer, int bufSize);
void SetGfxConfig(const GfxConfiguration* config);
void R_MakeDedicated(const GfxConfiguration* config);
void R_SetVidRestart();
void R_AllocateMinimalResources();
void R_SetIsMultiplayer(bool isMp);
void R_SetIsZombie(bool isZM);
void R_ShutdownStreams();
void R_Shutdown(int destroyWindow);
void R_UnloadWorld();
void R_BeginRegistration(vidConfig_t* vidConfigOut, GfxViewParms* a2);
void R_EndRegistration(void);
void R_TrackStatistics(trStatistics_t* stats);
void R_ConfigureRenderer(const GfxConfiguration* config);
bool R_StereoActivated();
