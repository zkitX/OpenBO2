#pragma once

union GfxDrawSurf
{
	GfxDrawSurfFields fields;
	unsigned __int64 packed;
};

struct GfxDrawSurfFields
{
	unsigned __int64 objectId : 16;
	unsigned __int64 customIndex : 9;
	unsigned __int64 reflectionProbeIndex : 5;
	unsigned __int64 dlightMask : 2;
	unsigned __int64 materialSortedIndex : 12;
	unsigned __int64 primaryLightIndex : 8;
	unsigned __int64 surfType : 4;
	unsigned __int64 prepass : 2;
	unsigned __int64 primarySortKey : 6;
};

class r_drawsurf
{
};

