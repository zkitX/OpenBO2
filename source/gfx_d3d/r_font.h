#pragma once

#include "r_material.h"

struct Glyph
{
	unsigned __int16 letter;
	char x0;
	char y0;
	char dx;
	char pixelWidth;
	char pixelHeight;
	float s0;
	float t0;
	float s1;
	float t1;
};

struct KerningPairs
{
	unsigned __int16 wFirst;
	unsigned __int16 wSecond;
	int iKernAmount;
};

struct Font_s
{
	const char* fontName;
	int pixelHeight;
	int isScalingAllowed;
	int glyphCount;
	int kerningPairsCount;
	Material* material;
	Material* glowMaterial;
	Glyph* glyphs;
	KerningPairs* kerningPairs;
};

class r_font
{
};

