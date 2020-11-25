#pragma once
#include "r_material.h"
#include "r_scene.h"
#include "rb_stats.h"

#include <d3d11.h>

enum GfxDepthRangeType
{
	GFX_DEPTH_RANGE_SCENE = 0x0,
	GFX_DEPTH_RANGE_VIEWMODEL = 0x40000000,
	GFX_DEPTH_RANGE_FULL = 0xFFFFFFFF,
};

struct GfxCmdBufPrimState
{
	union {
		ID3D11DeviceContext* device;
		ID3D11DeviceContext* localDevice;
	};
	ID3D11Buffer* indexBuffer;
	MaterialVertexDeclType vertDeclType;
	struct { 
		unsigned int strides[3];
		ID3D11Buffer* vbs[3];
		unsigned int offsets[3];
	} streams;
	ID3D11InputLayout* vertexDecl;
	__m128 constantsBuffer[262];
	char* constants[4];
	bool constantsDirty[4];
	GfxFrameStats frameStats;
};

struct GfxCmdBufState
{
	char refSamplerState[16];
	unsigned int samplerState[16];
	const GfxTexture* samplerTexture[16];
	GfxCmdBufPrimState prim;
	const Material* material;
	char techType;
	const MaterialTechnique* technique;
	union {
		const MaterialPass* localPass;
		const MaterialPass* pass;
	};
	unsigned int passIndex;
	GfxDepthRangeType depthRangeType;
	float depthRangeNear;
	float depthRangeFar;
	unsigned __int64 vertexShaderConstStateBuffer[1048];
	unsigned __int64* vertexShaderConstState[4];
	unsigned __int64 pixelShaderConstStateBuffer[1048];
	unsigned __int64* pixelShaderConstState[4];
	char alphaRef;
	ID3D11BlendState* blendState;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11RasterizerState* rasterizerState;
	const MaterialPixelShader* pixelShader;
	const MaterialVertexShader* vertexShader;
	unsigned int pixPrimarySortKey;
	const Material* pixMaterial;
	const MaterialTechnique* pixTechnique;
	int pixCombine;
	GfxViewport viewport;
	GfxViewport scissor;
	int scissorEnabled;
	char renderTargetId;
	int stateOverride;
	const GfxBackEndData* backEndData;
};

GfxCmdBufState gfxCmdBufState;