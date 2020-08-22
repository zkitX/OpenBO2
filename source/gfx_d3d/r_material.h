#pragma once
#include "r_drawsurf.h"
#include "r_image.h"
#include "r_shade.h"
#include "r_utils.h"

struct Material
{
	MaterialInfo info;
	char stateBitsEntry[36];
	char textureCount;
	char constantCount;
	char stateBitsCount;
	char stateFlags;
	char cameraRegion;
	char probeMipBits;
	union {
		MaterialTechniqueSet* localTechniqueSet;
		MaterialTechniqueSet* techniqueSet;
	};
	MaterialTextureDef* textureTable;
	union {
		MaterialConstantDef* localConstantTable;
		MaterialConstantDef* constantTable;
	};
	GfxStateBits* stateBitsTable;
	Material* thermalMaterial;
};

struct MaterialArgumentCodeConst
{
	unsigned __int16 index;
	char firstRow;
	char rowCount;
};

union MaterialArgumentDef
{
	const float* literalConst;
	MaterialArgumentCodeConst codeConst;
	unsigned int codeSampler;
	unsigned int nameHash;
};

union MaterialArgumentLocation
{
	unsigned __int16 offset;
	struct {
		char textureIndex;
		char samplerIndex;
	};
};

struct MaterialConstantDef
{
	unsigned int nameHash;
	char name[12];
	vec4_t literal;
};

struct MaterialInfo
{
	const char* name;
	unsigned int gameFlags;
	char pad;
	char sortKey;
	char textureAtlasRowCount;
	char textureAtlasColumnCount;
	GfxDrawSurf drawSurf;
	unsigned int surfaceTypeBits;
	unsigned int layeredSurfaceTypes;
	unsigned __int16 hashIndex;
	int surfaceFlags;
	int contents;
};

struct MaterialPass
{
	MaterialVertexDeclaration* vertexDecl;
	MaterialVertexShader* vertexShader;
	union {
		MaterialPixelShader* pixelShader;
		MaterialPixelShader* localPixelShader;
	};
	char perPrimArgCount;
	char perObjArgCount;
	char stableArgCount;
	char customSamplerFlags;
	char precompiledIndex;
	char materialType;
	union {
		MaterialShaderArgument* localArgs;
		MaterialShaderArgument* args;
	};
};

struct MaterialPixelShader
{
	const char* name;
	MaterialPixelShaderProgram prog;
};

struct MaterialPixelShaderProgram
{
	ID3D11PixelShader* ps;
	GfxPixelShaderLoadDef loadDef;
};

struct MaterialShaderArgument
{
	unsigned __int16 type;
	MaterialArgumentLocation location;
	unsigned __int16 size;
	unsigned __int16 buffer;
	MaterialArgumentDef u;
};

struct MaterialStreamRouting
{
	char source;
	char dest;
};

struct MaterialTechnique
{
	const char* name;
	unsigned __int16 flags;
	unsigned __int16 passCount;
	MaterialPass passArray[1];
};

struct MaterialTechniqueSet
{
	const char* name;
	char worldVertFormat;
	MaterialTechnique* techniques[36];
};

struct MaterialTextureDef
{
	unsigned int nameHash;
	char nameStart;
	char nameEnd;
	char samplerState;
	char semantic;
	char isMatureContent;
	char pad[3];
	GfxImage* image;
};

struct MaterialVertexDeclaration
{
	char streamCount;
	bool hasOptionalSource;
	bool isLoaded;
	MaterialVertexStreamRouting routing;
};

struct MaterialVertexShader
{
	const char* name;
	MaterialVertexShaderProgram prog;
};

struct MaterialVertexShaderProgram
{
	ID3D11VertexShader* vs;
	GfxVertexShaderLoadDef loadDef;
};

struct MaterialVertexStreamRouting
{
	MaterialStreamRouting data[16];
	ID3D11InputLayout* decl[20];
};

class r_material
{
};

