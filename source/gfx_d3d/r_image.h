#pragma once
#include "../defs.h"

#include <d3d11.h>

struct CardMemory
{
	int platform[2];
};

struct GfxImage
{
	GfxTexture texture;
	char mapType;
	char semantic;
	char category;
	bool delayLoadPixels;
	Picmip picmip;
	bool noPicmip;
	char track;
	CardMemory cardMemory;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int16 depth;
	char levelCount;
	char streaming;
	unsigned int baseSize;
	char* pixels;
	GfxStreamedPartInfo streamedParts[1];
	char streamedPartCount;
	unsigned int loadedSize;
	char skippedMipLevels;
	const char* name;
	unsigned int hash;
};

struct GfxImageLoadDef
{
	char levelCount;
	char flags;
	int format;
	int resourceSize;
	char data[1];
};

struct GfxStreamedPartInfo
{
	unsigned int levelCountAndSize;
	unsigned int hash;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int32 offset : 32;
	unsigned __int32 size : 28;
	unsigned __int32 ipakIndex : 4;
	unsigned __int32 adjacentLeft : 15;
	unsigned __int32 adjacentRight : 15;
	unsigned __int32 compressed : 1;
	unsigned __int32 valid : 1;
};

union GfxTexture
{
	ID3D11ShaderResourceView* basemap;
	GfxImageLoadDef* loadDef;
};

struct Picmip
{
	char platform[2];
};

class r_image
{
};

