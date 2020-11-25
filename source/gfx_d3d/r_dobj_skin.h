#pragma once
#include "r_shared.h"

struct half
{
	unsigned __int16 v;
};

struct half2 {
	union {
		struct {
			half x;
			half y;
		};
		unsigned int v;
	};
};

struct half4
{
	union {
		struct {
			half x;
			half y;
			half z;
			half w;
		};
		unsigned __int64 v;
	};
};

struct SSkinShaders
{
	char* pixelShaderAlign;
	char* pixelShader;
	char* vertexShader;
	int pixelShaderSize;
	int vertexShaderSize;
};

struct SSkinVert
{
	half4 pos_bone;
	PackedUnitVec normal;
	half2 uv;
};

struct SSkinModel
{
	int numVerts;
	int numTris;
	SSkinVert* verts;
	unsigned __int16* tris;
};

struct SSkinAnim
{
	int bones;
	int width;
	int frames;
	float* data;
};

struct SSkinInstance
{
	union {
		vec4_t mat;
		float disk_mat[16];
	};
	SSkinShaders* shaders;
	SSkinModel* model;
	SSkinAnim* anim;
	SSkinVert* instVerts;
	float frame;
	int pad1;
	int pad2;
	int pad3;
};
