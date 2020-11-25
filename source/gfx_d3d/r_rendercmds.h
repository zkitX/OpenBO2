#pragma once
#include "r_buffers.h"
#include "r_debug.h"
#include "r_init.h"
#include "r_meshdata.h"
#include "r_rendertarget.h"
#include "r_scene.h"
#include "r_shade.h"
#include "r_wind.h"
#include "rb_light.h"
#include "rb_state.h"
#include "rb_stats.h"
#include "rb_sunshadow.h"
#include "rb_tess.h"

#include <d3d11.h>
#include <universal/com_math.h>
#include <universal/com_vector.h>
#include <universal/com_workercmds.h>

struct GfxCmdHeader
{
	unsigned __int16 byteCount;
	char id;
	char ui3d;
};

struct GfxCmdBuf
{
	ID3D11DeviceContext* device;
	ID3D11CommandList** commandList;
	unsigned __int16 viewIndex;
	unsigned __int16 deviceIndex;
};

struct GfxCmdArray
{
	char* cmds;
	int usedTotal;
	int usedCritical;
	GfxCmdHeader* lastCmd;
	int byteSize;
	int warnSize;
	int check;
};

struct GfxCmdBufInput
{
	vec4_t consts[211];
	const GfxImage* codeImages[55];
	char codeImageSamplerStates[55];
	const GfxBackEndData* data;
};

struct GfxBackEndPrimitiveData
{
	int hasSunDirChanged;
};

struct GfxSpotShadow
{
	GfxViewParms shadowViewParms;
	GfxMatrix lookupMatrix;
	char shadowableLightIndex;
	char flags;
	char pad[2];
	const GfxLight* light;
	float fade;
	GfxCodeImageRenderTarget spotShadowRenderTarget;
	char renderTargetId;
	GfxViewport viewport;
	GfxImage* image;
	vec4_t pixelAdjust;
	int clearScreen;
};

class GfxBackEndData
{
	char surfsBuffer[1];
	FxCodeMeshData codeMeshes[1];
	unsigned int primDrawSurfsBuf[65536];
	char lodData[4][16384];
	GfxViewParms viewParms[9];
	char primaryLightTechType[16][256];
	vec4_t codeMeshArgs[256];
	GfxParticleCloud clouds[256];
	GfxDrawSurf drawSurfs[1];
	GfxMeshData* codeMeshPtr;
	GfxMeshData markMesh;
	GfxMeshData glassMesh;
	GfxBackEndPrimitiveData prim;
	char shadowableLightHasShadowMap[255];
	unsigned int frameCount;
	int drawSurfCount;
	volatile int surfPos;
	volatile int gfxEntCount;
	GfxEntity gfxEnts[256];
	volatile int shaderConstantSetCount;
	ShaderConstantSet shaderConstantSets[128];
	volatile int textureOverrideCount;
	GfxTextureOverride textureOverrides[1024];
	volatile int cloudCount;
	volatile int codeMeshCount;
	volatile int codeMeshArgsCount;
	volatile int markMeshCount;
	FxMarkMeshData markMeshes[1];
	GfxVertexBufferState* skinnedCacheVb;
	volatile int skinnedCacheVertsAllocated;
	GfxIndexBufferState* dynamicIndexBuffer;
	GfxVertexBufferState* dynamicVertexBuffer;
	ID3D11Query* endFence;
	ID3D11Query* endFrameFence;
	char* tempSkinBuf;
	volatile int tempSkinPos;
	int tempSkinSize;
	int viewParmCount;
	GfxFog fogSettings;
	GfxCmdArray* commands;
	unsigned int viewInfoIndex;
	unsigned int viewInfoCount;
	GfxViewInfo* viewInfo;
	GfxWorkerData workerData[4];
	GfxUI3DBackend rbUI3D;
	GfxQRCodeBackend rbQRCode;
	const void* cmds;
	const void* compositingCmds;
	GfxLight sunLight;
	int hasApproxSunDirChanged;
	volatile int primDrawSurfPos;
	unsigned int streamerExecedFrame;
	unsigned int frameLastTransferId;
	jqBatchGroup glassGenerateVertsBatchGroup;
	DebugGlobals debugGlobals;
	unsigned int dualPlaySplitScreenOn;
	unsigned int drawType;
	float eyeOffset;
	int hideMatureContent;
	int splitscreen;
	GfxLight shadowableLights[255];
	unsigned int shadowableLightCount;
	unsigned int emissiveSpotLightIndex;
	GfxLight emissiveSpotLight;
	unsigned int emissiveSpotLightCount;
	int emissiveSpotLightShadowableLightIndex;
	GfxSunShadow sunShadow;
	unsigned int spotShadowCount;
	GfxSpotShadow spotShadows[4];
	WindState gfxWindState;
	GfxFrameStats frameStats;
};
