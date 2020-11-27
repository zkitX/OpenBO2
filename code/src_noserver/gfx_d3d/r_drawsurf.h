#pragma once

struct GfxDrawSurfFields
{
	unsigned long long objectId : 16;
	unsigned long long customIndex : 9;
	unsigned long long reflectionProbeIndex : 5;
	unsigned long long dlightMask : 2;
	unsigned long long materialSortedIndex : 12;
	unsigned long long primaryLightIndex : 8;
	unsigned long long surfType : 4;
	unsigned long long prepass : 2;
	unsigned long long primarySortKey : 6;
};

union GfxDrawSurf
{
	GfxDrawSurfFields fields;
	unsigned long long packed;
};

class r_drawsurf
{
};
