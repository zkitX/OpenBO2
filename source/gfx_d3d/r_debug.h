#pragma once

#include <universal/com_math.h>

struct GfxDebugPlume
{
	vec3_t origin;
	vec4_t color;
	int score;
	int startTime;
	int duration;
};

struct GfxDebugPoly
{
	vec4_t faceColor;
	bool faceDepthTest;
	vec4_t edgeColor;
	bool edgeDepthTest;
	int firstVert;
	int vertCount;
};

struct trDebugLine_t
{
	vec3_t start;
	vec3_t end;
	vec4_t color;
	int depthTest;
};

struct trDebugSphere_t
{
	vec3_t center;
	float radius;
	int sideCount;
	vec4_t color;
	int depthTest;
};

struct trDebugString_t
{
	vec3_t xyz;
	vec4_t color;
	float scale;
	char text[96];
};

struct PolySet
{
	vec3_t* verts;
	int vertCount;
	int vertLimit;
	GfxDebugPoly* polys;
	int polyCount;
	int polyLimit;
};

struct DebugGlobals
{
	PolySet polySet;
	trDebugString_t* strings;
	int stringCount;
	int stringLimit;
	trDebugString_t* externStrings;
	int externStringCount;
	int externMaxStringCount;
	trDebugLine_t* lines;
	int lineCount;
	int lineLimit;
	trDebugLine_t* externLines;
	int externLineCount;
	int externMaxLineCount;
	GfxDebugPlume* plumes;
	int plumeCount;
	int plumeLimit;
	trDebugSphere_t* spheres;
	int sphereCount;
	int sphereLimit;
	trDebugSphere_t* externSpheres;
	int externSphereCount;
	int externMaxSphereCount;
};
