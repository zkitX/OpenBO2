#pragma once
#include "r_shared.h"

struct GfxLightCorona
{
	vec3_t origin;
	float radius;
	vec3_t color;
	float intensity;
};

struct Corona
{
	bool inUse;
	GfxLightCorona* info;
	int timeSinceSpawned;
	float currentVisibility;
	float targetVisibility;
	int hitNum;
	bool wasEverVisible;
	bool queryIssued[5];
	ID3D11Query* query[5];
	ID3D11Query* queryMax[5];
};