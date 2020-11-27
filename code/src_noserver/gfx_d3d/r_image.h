#pragma once
#include "r_material.h"

#include <d3d11.h>
#include <universal/com_math.h>

struct CardMemory
{
	int platform[2];
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
	unsigned short width;
	unsigned short height;
	unsigned int offset : 32;
	unsigned int size : 28;
	unsigned int ipakIndex : 4;
	unsigned int adjacentLeft : 15;
	unsigned int adjacentRight : 15;
	unsigned int compressed : 1;
	unsigned int valid : 1;
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
	unsigned short width;
	unsigned short height;
	unsigned short depth;
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

struct GfxTextureOverride
{
	__int16 prev;
	char type;
	unsigned int dobjModelMask;
	const Material* mat;
	union {
		struct {
			const GfxImage* img1;
			const GfxImage* img2;
		} image;
		struct {
			const Material* mat1;
			const Material* mat2;
		} material;
		struct {
			unsigned int constIdx;
			vec4_t value;
		} shaderConst;
	};
};
