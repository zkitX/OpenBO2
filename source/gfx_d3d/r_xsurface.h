#pragma once

#include <d3d11.h>

#include "r_utils.h"

struct XSurfaceVertexInfo
{
	__int16 vertCount[4];
	unsigned __int16* vertsBlend;
	float* tensionData;
};

struct XSurfaceCollisionAabb
{
	unsigned __int16 mins[3];
	unsigned __int16 maxs[3];
};

struct XSurfaceCollisionNode
{
	XSurfaceCollisionAabb aabb;
	unsigned __int16 childBeginIndex;
	unsigned __int16 childCount;
};

struct XSurfaceCollisionLeaf
{
	unsigned __int16 triangleBeginIndex;
};

struct XSurfaceCollisionTree
{
	vec3_t trans;
	vec3_t scale;
	unsigned int nodeCount;
	XSurfaceCollisionNode* nodes;
	unsigned int leafCount;
	XSurfaceCollisionLeaf* leafs;
};

struct XRigidVertList
{
	unsigned __int16 boneOffset;
	unsigned __int16 vertCount;
	unsigned __int16 triOffset;
	unsigned __int16 triCount;
	XSurfaceCollisionTree* collisionTree;
};

struct XSurface
{
	char tileMode;
	char vertListCount;
	unsigned __int16 flags;
	unsigned __int16 vertCount;
	unsigned __int16 triCount;
	unsigned __int16 baseVertIndex;
	unsigned __int16* triIndices;
	XSurfaceVertexInfo vertInfo;
	GfxPackedVertex* verts0;
	ID3D11Buffer* vb0;
	union {
		XRigidVertList* vertList;
		XRigidVertList* vertListEA;
	};
	ID3D11Buffer* indexBuffer;
	int partBits[5];
};

class r_xsurface
{
};

