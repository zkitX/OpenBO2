#pragma once
#include "r_drawsurf.h"
#include "r_image.h"
#include "r_shade.h"
#include "r_utils.h"

struct MaterialTechniqueSet;
struct MaterialTextureDef;
struct MaterialConstantDef;
struct MaterialVertexDeclaration;
struct MaterialVertexShader;
struct MaterialPixelShader;
struct MaterialShaderArgument;

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
	unsigned short hashIndex;
	int surfaceFlags;
	int contents;
};

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
	unsigned short index;
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
	unsigned short offset;
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

struct MaterialPixelShaderProgram
{
	ID3D11PixelShader* ps;
	GfxPixelShaderLoadDef loadDef;
};


struct MaterialPixelShader
{
	const char* name;
	MaterialPixelShaderProgram prog;
};

struct MaterialShaderArgument
{
	unsigned short type;
	MaterialArgumentLocation location;
	unsigned short size;
	unsigned short buffer;
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
	unsigned short flags;
	unsigned short passCount;
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

struct MaterialVertexStreamRouting
{
	MaterialStreamRouting data[16];
	ID3D11InputLayout* decl[20];
};

struct MaterialVertexDeclaration
{
	char streamCount;
	bool hasOptionalSource;
	bool isLoaded;
	MaterialVertexStreamRouting routing;
};

struct MaterialVertexShaderProgram
{
	ID3D11VertexShader* vs;
	GfxVertexShaderLoadDef loadDef;
};

struct MaterialVertexShader
{
	const char* name;
	MaterialVertexShaderProgram prog;
};

class r_material
{
};
