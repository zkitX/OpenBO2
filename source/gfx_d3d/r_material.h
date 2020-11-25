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

enum MaterialVertexDeclType
{
	VERTDECL_GENERIC = 0x0,
	VERTDECL_PACKED = 0x1,
	VERTDECL_PACKED_LMAP_VC = 0x2,
	VERTDECL_WORLD = 0x3,
	VERTDECL_PACKED_WORLD = 0x4,
	VERTDECL_PACKED_WORLD_T1N0 = 0x5,
	VERTDECL_PACKED_WORLD_T1N1 = 0x6,
	VERTDECL_PACKED_WORLD_T2N0 = 0x7,
	VERTDECL_PACKED_WORLD_T2N1 = 0x8,
	VERTDECL_PACKED_WORLD_T2N2 = 0x9,
	VERTDECL_PACKED_WORLD_T3N0 = 0xA,
	VERTDECL_PACKED_WORLD_T3N1 = 0xB,
	VERTDECL_PACKED_WORLD_T3N2 = 0xC,
	VERTDECL_PACKED_WORLD_T4N0 = 0xD,
	VERTDECL_PACKED_WORLD_T4N1 = 0xE,
	VERTDECL_PACKED_WORLD_T4N2 = 0xF,
	VERTDECL_POS_TEX = 0x10,
	VERTDECL_PARTICLECLOUD = 0x11,
	VERTDECL_POS_COLOR = 0x12,
	VERTDECL_POS_FP16TEX = 0x13,
	VERTDECL_COUNT = 0x14,
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

struct MaterialMemory
{
	Material* material;
	int memory;
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

struct VertUsage
{
	int index;
	VertUsage* next;
};

struct MaterialUsage
{
	int memory;
	VertUsage* verts;
	Material* material;
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