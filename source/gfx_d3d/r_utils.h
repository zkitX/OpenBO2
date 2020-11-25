#pragma once

#include <d3d11.h>
#include <universal/com_math.h>

struct GfxMatrix
{
	vec4_t m[4];
};

union GfxColor
{
	unsigned int packed;
	char array[4];
};

union PackedTexCoords
{
	unsigned int packed;
};

union PackedUnitVec
{
	unsigned int packed;
};

struct GfxPackedVertex
{
	vec3_t xyz;
	float binormalSign;
	GfxColor color;
	PackedTexCoords texCoord;
	PackedUnitVec normal;
	PackedUnitVec tangent;
};

struct GfxStateBits
{
	unsigned int loadBits[2];
	ID3D11BlendState* blendState;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11RasterizerState* rasterizerState;
};