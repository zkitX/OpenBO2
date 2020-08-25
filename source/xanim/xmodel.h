#pragma once

#include "dobj.h"

struct XModelRigidCullInfoContext
{
	int cullInfoPos;
	unsigned int cullInfoBufSize;
	char cullFlags;
	unsigned int size;
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

class xmodel
{
};

