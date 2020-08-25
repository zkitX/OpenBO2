#pragma once

#include <d3d11.h>

union vec2_t
{
	float v[2];
	struct {
		float x;
		float y;
	};
};

union vec3_t
{
	struct {
		float x;
		float y;
		float z;
	};
	float v[3];
};

union vec4_t
{
	float v[4];
	struct {
		float x;
		float y;
		float z;
		float w;
	};
	struct {
		float r;
		float g;
		float b;
		float a;
	};
};

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

struct GfxStateBits
{
	unsigned int loadBits[2];
	ID3D11BlendState* blendState;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11RasterizerState* rasterizerState;
};