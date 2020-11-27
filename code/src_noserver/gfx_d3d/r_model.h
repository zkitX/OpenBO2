#pragma once

#include "r_shared.h"

struct PhysPreset
{
	const char* name;
	int flags;
	float mass;
	float bounce;
	float friction;
	float bulletForceScale;
	float explosiveForceScale;
	const char* sndAliasPrefix;
	float piecesSpreadFraction;
	float piecesUpwardVelocity;
	int canFloat;
	float gravityScale;
	vec3_t centerOfMassOffset;
	vec3_t buoyancyBoxMin;
	vec3_t buoyancyBoxMax;
};

struct GfxPlacement
{
	vec4_t quat;
	vec3_t origin;
};

struct GfxScaledPlacement
{
	GfxPlacement base;
	float scale;
};

union XModelDrawInfo
{
	struct {
		char state;
		char lod;
		unsigned __int16 surfId;
	};
	unsigned int packed;
};

struct GfxBrushModelWritable
{
	vec3_t mins;
	float padding1;
	vec3_t maxs;
	float padding2;
};

struct GfxBrushModel
{
	GfxBrushModelWritable writable;
	vec3_t bounds[2];
	unsigned int surfaceCount;
	unsigned int startSurfIndex;
};

#pragma region XModel
struct XBoneInfo
{
	vec3_t bounds[2];
	vec3_t offset;
	float radiusSquared;
	char collmap;
};
struct XModelLodInfo
{
	float dist;
	unsigned __int16 numsurfs;
	unsigned __int16 surfIndex;
	int partBits[5];
};
struct XModelCollTri_s
{
	vec4_t plane;
	vec4_t svec;
	vec4_t tvec;
};
struct XModelCollSurf_s
{
	XModelCollTri_s* collTris;
	int numCollTris;
	vec3_t mins;
	vec3_t maxs;
	int boneIdx;
	int contents;
	int surfFlags;
};

struct XModel
{
	const char* name;
	char numBones;
	char numRootBones;
	char numsurfs;
	char lodRampType;
	union {
		unsigned __int16* localBoneNames;
		unsigned __int16* boneNames;
	};
	union {
		char* localParentList;
		char* parentList;
	};
	union {
		__int16* localQuats;
		__int16* quats;
	};
	union {
		float* localTrans;
		float* trans;
	};
	char* partClassification;
	DObjAnimMat* baseMat;
	XSurface* surfs;
	Material** materialHandles;
	XModelLodInfo lodInfo[4];
	XModelCollSurf_s* collSurfs;
	int numCollSurfs;
	int contents;
	union {
		XBoneInfo* localBoneInfo;
		XBoneInfo* boneInfo;
	};
	float radius;
	vec3_t mins;
	vec3_t maxs;
	__int16 numLods;
	__int16 collLod;
	float* himipInvSqRadii;
	int memUsage;
	int flags;
	bool bad;
	PhysPreset* physPreset;
	char numCollmaps;
	Collmap* collmaps;
	PhysConstraints* physConstraints;
	vec3_t lightingOriginOffset;
	float lightingOriginRange;
};

#pragma endregion


struct GfxStaticModelInst
{
	vec3_t mins;
	vec3_t maxs;
	vec3_t lightingOrigin;
};

struct GfxStaticModelLmapVertexInfo
{
	unsigned int* lmapVertexColors;
	ID3D11Buffer* lmapVertexColorsVB;
	unsigned __int16 numLmapVertexColors;
};

struct GfxStaticModelDrawInst
{
	float cullDist;
	GfxPackedPlacement placement;
	XModel* model;
	int flags;
	float invScaleSq;
	unsigned __int16 lightingHandle;
	unsigned __int16 colorsIndex;
	GfxLightingSHQuantized lightingSH;
	char primaryLightIndex;
	char visibility;
	char reflectionProbeIndex;
	unsigned int smid;
	GfxStaticModelLmapVertexInfo lmapVertexInfo[4];
};
