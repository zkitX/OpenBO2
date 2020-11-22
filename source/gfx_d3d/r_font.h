#pragma once

#include <qcommon/com_clients.h>
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

const char MYRANDOMCHARS[63] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890" };
const char MYRANDOMNUMCHARS[10] = { "123456789" };

const Glyph* R_GetCharacterGlyph(Font_s*, unsigned int);
int R_GetKerningAmount(Font_s*, int, int);
unsigned int R_FontGetRandomLetter(Font_s*, int);
unsigned int R_FontGetRandomNumberCharacter(Font_s*, int);
Font_s* R_RegisterFont_LoadObj(char const*, int);
Font_s* R_RegisterFont_FastFile(char const*, int);
Font_s* R_RegisterFont(char const*, int);
float R_NormalizedTextScale(Font_s*, float);
void GetButtonProperties(LocalClientNum_t, char const*, Font_s*, int*, int*);
int R_TextWidth(LocalClientNum_t, char const*, int, Font_s*);
int R_TextHeight(Font_s*);
char* R_TextLineWrapPosition(char const*, int, int, Font_s*, float);
int R_ConsoleTextWidth(const char*, int, int, int, Font_s*);