#pragma once

enum GfxDrawSceneMethod
{
	GFX_DRAW_SCENE_NONE = 0x0,
	GFX_DRAW_SCENE_FULLBRIGHT = 0x1,
	GFX_DRAW_SCENE_DEBUGSHADER = 0x2,
	GFX_DRAW_SCENE_STANDARD = 0x3,
};

struct GfxDrawMethod
{
	GfxDrawSceneMethod drawScene;
	char baseTechType;
	char emissiveTechType;
	char litTechType[16][11];
};

GfxDrawMethod gfxDrawMethod;