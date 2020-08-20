#pragma once

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

union vec3_t
{
	struct {
		float x;
		float y;
		float z;
	};
	float v[3];
};

union vec2_t
{
	float v[2];
	struct {
		float x;
		float y;
	};
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

