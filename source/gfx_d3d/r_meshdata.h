#pragma once
#include "r_buffers.h"
#include "r_light.h"

#include <d3d11.h>

struct FxMarkMeshData
{
	unsigned int triCount;
	unsigned __int16* indices;
	unsigned __int16 modelIndex;
	char modelTypeAndSurf;
	char pad0;
};

struct GfxMeshData
{
	GfxMeshData* thisPtr;
	unsigned int indexCount;
	unsigned int totalIndexCount;
	ID3D11Buffer* ib;
	unsigned __int16* indices;
	GfxVertexBufferState vb;
	unsigned int vertSize;
};

struct FxCodeMeshData
{
	unsigned int triCount;
	unsigned __int16* indices;
	unsigned __int16 argOffset;
	unsigned __int16 argCount;
	GfxLightingInfo lightingInfo;
};

struct GfxQuadMeshData
{
	float x;
	float y;
	float width;
	float height;
	GfxMeshData meshData;
};