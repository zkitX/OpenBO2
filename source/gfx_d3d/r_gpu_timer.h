#pragma once

#include <d3d11.h>
#include <d3d9.h>
#include <universal/dvar.h>

#include "r_font.h"

enum PerfTimerDisplayMode
{
	PERF_TIMER_MODE_TOP = 0x0,
	PERF_TIMER_MODE_ALL = 0x1,
	PERF_TIMER_MODE_ACTIVE = 0x2,
	PERF_TIMER_MODE_MAJOR = 0x3,
	PERF_TIMER_MODE_MINOR = 0x4,
	PERF_TIMER_MODE_UNKNOWN = 0x5,
};

enum PerfTimerDisplayType
{
	PERF_TIMER_TYPE_STANDARD = 0x0,
	PERF_TIMER_TYPE_IN_PLACE = 0x1,
	PERF_TIMER_TYPE_COM_ONLY = 0x2,
	PERF_TIMER_TYPE_UNKNOWN = 0x3,
};

enum GPUTimerId
{
	GPU_TIMER_UI3D = 0x0,
	GPU_TIMER_LIT_SONAR = 0x1,
	GPU_TIMER_SHADOW_TOTAL = 0x2,
	GPU_TIMER_SUN_SHADOW_TOTAL = 0x3,
	GPU_TIMER_SPOT_SHADOW_TOTAL = 0x4,
	GPU_TIMER_SUN_SHADOW_0 = 0x5,
	GPU_TIMER_SUN_SHADOW_1 = 0x6,
	GPU_TIMER_SPOT_SHADOW_0 = 0x7,
	GPU_TIMER_SPOT_SHADOW_1 = 0x8,
	GPU_TIMER_SPOT_SHADOW_2 = 0x9,
	GPU_TIMER_SPOT_SHADOW_3 = 0xA,
	GPU_TIMER_MAINPASS_TOTAL = 0xB,
	GPU_TIMER_CLEAR_SCREEN = 0xC,
	GPU_TIMER_DEPTH_HACK = 0xD,
	GPU_TIMER_DEPTH_PREPASS = 0xE,
	GPU_TIMER_ZCULL_RELOAD = 0xF,
	GPU_TIMER_LIT_BSP = 0x10,
	GPU_TIMER_LIT_SMODEL = 0x11,
	GPU_TIMER_LIT = 0x12,
	GPU_TIMER_LIGHTMAP = 0x13,
	GPU_TIMER_EMISSIVE_OPAQUE = 0x14,
	GPU_TIMER_LIT_QUASI_OPAQUE = 0x15,
	GPU_TIMER_RESOLVE_SCENE = 0x16,
	GPU_TIMER_LIT_POST_RESOLVE = 0x17,
	GPU_TIMER_LIGHTMAP_POST_RESOLVE = 0x18,
	GPU_TIMER_TRANS = 0x19,
	GPU_TIMER_SUN = 0x1A,
	GPU_TIMER_EMISSIVE_TOTAL = 0x1B,
	GPU_TIMER_EMISSIVE_FX = 0x1C,
	GPU_TIMER_TRANS_POST_FX = 0x1D,
	GPU_TIMER_VIEWMODEL_FX = 0x1E,
	GPU_TIMER_VIEWMODEL_TRANS = 0x1F,
	GPU_TIMER_SUPERFLARE = 0x20,
	GPU_TIMER_EXTRACAM = 0x21,
	GPU_TIMER_FXAA = 0x22,
	GPU_TIMER_BLOOM = 0x23,
	GPU_TIMER_DOF = 0x24,
	GPU_TIMER_UPSCALE = 0x25,
	GPU_TIMER_2D = 0x26,
	GPU_TIMER_2D_TOTAL = 0x27,
	GPU_TIMER_COUNT = 0x28,
};

enum GPUTimerDisplayMode
{
	GPU_TIMER_DISPLAY_MODE_DISABLED = 0x0,
	GPU_TIMER_DISPLAY_MODE_ACTIVE_IN_PLACE_SIMPLE = 0x1,
	GPU_TIMER_DISPLAY_MODE_ACTIVE_IN_PLACE = 0x2,
	GPU_TIMER_DISPLAY_MODE_ALL_IN_PLACE = 0x3,
	GPU_TIMER_DISPLAY_MODE_TOP = 0x4,
	GPU_TIMER_DISPLAY_MODE_ALL = 0x5,
	GPU_TIMER_DISPLAY_MODE_ACTIVE = 0x6,
	GPU_TIMER_DISPLAY_MODE_MAJOR = 0x7,
	GPU_TIMER_DISPLAY_MODE_MINOR = 0x8,
	GPU_TIMER_DISPLAY_MODE_MAJOR_IN_PLACE = 0x9,
	GPU_TIMER_DISPLAY_MODE_MINOR_IN_PLACE = 0xA,
	GPU_TIMER_DISPLAY_MODE_ALL_COM_ONLY = 0xB,
	GPU_TIMER_DISPLAY_MODE_ACTIVE_COM_ONLY = 0xC,
	GPU_TIMER_DISPLAY_MODE_COLLECT_ONLY = 0xD,
	GPU_TIMER_DISPLAY_MODE_COUNT = 0xE,
};

struct PerfTimer
{
	unsigned int beginTime;
	unsigned int totalTime;
	PerfTimer* parent;
	PerfTimer* nextSibling;
	bool hasChild;
	bool isActive;
	bool hide;
	char bucket;
};

struct PerfTimerFrame
{
	unsigned int beginTime;
	unsigned int totalTime;
	PerfTimer* timers[80];
};

struct PerfTimerFrameHistory
{
	unsigned int currentFrame;
	unsigned int frameCount;
	PerfTimerFrame frames[64];
};

struct PerfTimerStyle
{
	Font_s* font;
	float fontWidth;
	float fontHeight;
	PerfTimerDisplayMode displayMode;
	PerfTimerDisplayType displayType;
	float(* convertTime)(unsigned int);
	unsigned int timerCount;
	unsigned int timerNameCharCount;
	const char** timerNames;
	const GfxColor* timerColors;
	float totalBarY;
	float budgetMs;
	float budgetPixels;
	unsigned int bucketCount;
	const char** bucketNames;
};

struct GPUTimeStamp
{
	ID3D11Query* disjoint;
	ID3D11Query* beginQuery;
	ID3D11Query* endQuery;
	GPUTimeStamp* next;
};

struct GPUTimer : PerfTimer
{
	GPUTimeStamp* firstTimeStamp;
	GPUTimeStamp* currentTimeStamp;
	int adjustTime;
};

struct GPUTimerFrame
{
	unsigned int frameIndex;
	unsigned int beginTime;
	unsigned int totalTime;
	bool isCurrent;
	GPUTimer timers[40];
};

const char* g_gpuTimerNames[40] = { "UI3D", "sonar", "ShadowTotal", "SunShadowTotal", "SpotShadowTotal", "SunShadow0", "SunShadow1", "SpotShadow0", "SpotShadow1", "SpotShadow2", "SpotShadow3", "Mainpass Total", "ClearScreen", "Depth Hack", "DepthPrepass", "ZCull Reload", "Lit BSP", "Lit SModel", "Lit", "Lightmap", "Emissive Opaque", "LitQuasiOpaque", "ResolveScene", "LitPostResolve", "LightmapPostResolve", "Trans", "Sun+corona", "Emissive Total", "Emissive FX", "Trans post FX", "ViewModelFX", "ViewModelTrans", "Superflare", "ExtraCam", "FXAA", "Bloom", "Dof", "Upscale", "2D", "2D Toral" };

const char* g_gpuTimerSimpleNames[40] = { "UI3D", "sonar", "ShadowTotal", "SunShadowTotal", "SpotShadowTotal", "Dynamic sun shadows", "SunShadow1", "Dynamic primary shadows", "SpotShadow1", "SpotShadow2", "SpotShadow3", "Mainpass Total", "ClearScreen", "View Model", "DepthPrepass", "ZCull Reload", "Lit BSP Surfaces (In Dyn Shadow)", "Lit Static Models (In Dyn Shadow)", "Lit Other (In Dyn Shadow)", "Lightmap", "Emissive Opaque", "LitQuasiOpaque (eg foliage)", "ResolveScene", "Scene Reflection (In Dyn Shadow)", "Scene Reflection", "Transparent", "Sun Flare", "Emissive Total", "Particle FX", "Trans post FX", "ViewModelFX", "ViewModelTrans", "Superflare", "ExtraCam", "FXAA", "Bloom", "Dof", "Upscale", "2D", "Bloom,ADS,2D,HUD" };

int g_gpuTimerNamesMaxLength;
int g_gpuTimerSimpleNamesMaxLength;

const dvar_t* r_gpuTimers;
const dvar_t* r_vsync;

PerfTimerDisplayMode* s_gpuPerfMode;
PerfTimerDisplayType* s_gpuPerfType;
PerfTimerFrameHistory s_gpuTimerFrameHistory;
PerfTimerStyle s_gpuTimerStyle;

GPUTimer* s_currentTimer;
GPUTimeStamp* s_firstFreeTimeStamp;
GPUTimerDisplayMode s_displayMode;
GPUTimerFrame s_timerFrames[64];

unsigned int LastGoodElapsed[40];
unsigned int LastGoodTime;

int s_callbackIndex;
unsigned int s_frameIndex;
int s_lastReadIndex;
int s_readIndex;
volatile unsigned int s_totalTime;
int s_writeIndex;

void R_GPU_CalcDerivedTimers(GPUTimerFrame*);
void R_GPU_CalcTimers(GPUTimerFrame*);
void R_GPU_FreeInterveningTimeStamps(int, int);
void R_GPU_EndFrameCallback(void);
void R_GPU_BeginFrame(void);
void R_GPU_EndFrame(void);
void R_GPU_BeginTimer(GPUTimerId);
void R_GPU_EndTimer(void);
void R_GPU_DrawTimers(void);