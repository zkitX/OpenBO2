#pragma once
#include "r_buffers.h"

#include <d3d11.h>

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
