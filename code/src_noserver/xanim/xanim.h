#pragma once

#include <gfx_d3d/r_utils.h>

union XAnimDynamicIndices
{
	char _1[1];
	unsigned __int16 _2[1];
};

struct XAnimDeltaPartQuatDataFrames
{
	__int16(*frames)[4];
	XAnimDynamicIndices indices;
};

union XAnimDeltaPartQuatData
{
	XAnimDeltaPartQuatDataFrames frames;
	__int16 frame0[4];
};

struct XAnimDeltaPartQuat
{
	unsigned __int16 size;
	XAnimDeltaPartQuatData u;
};

struct XAnimDeltaPartQuatDataFrames2
{
	__int16(*frames)[2];
	XAnimDynamicIndices indices;
};

union XAnimDeltaPartQuatData2
{
	XAnimDeltaPartQuatDataFrames2 frames;
	__int16 frame0[2];
};

struct XAnimDeltaPartQuat2
{
	unsigned __int16 size;
	XAnimDeltaPartQuatData2 u;
};

union XAnimDynamicFrames
{
	char(*_1)[3];
	unsigned __int16(*_2)[3];
};

struct XAnimParam
{
	char strName[16];
	float fValue;
};

struct XAnimParent
{
	unsigned __int16 flags;
	unsigned __int16 children;
};

struct XAnimPartTransFrames
{
	vec3_t mins;
	vec3_t size;
	XAnimDynamicFrames frames;
	XAnimDynamicIndices indices;
};

union XAnimPartTransData
{
	XAnimPartTransFrames frames;
	vec3_t frame0;
};

struct XAnimPartTrans
{
	unsigned __int16 size;
	char smallTrans;
	XAnimPartTransData u;
};

struct XAnimDeltaPart
{
	XAnimPartTrans* trans;
	XAnimDeltaPartQuat2* quat2;
	XAnimDeltaPartQuat* quat;
};

union XAnimIndices
{
	char* _1;
	unsigned __int16* _2;
	void* data;
};

struct XAnimNotifyInfo
{
	unsigned __int16 name;
	float time;
};

struct XAnimParts
{
	const char* name;
	unsigned __int16 dataByteCount;
	unsigned __int16 dataShortCount;
	unsigned __int16 dataIntCount;
	unsigned __int16 randomDataByteCount;
	unsigned __int16 randomDataIntCount;
	unsigned __int16 numframes;
	bool bLoop;
	bool bDelta;
	bool bDelta3D;
	bool bLeftHandGripIK;
	unsigned int streamedFileSize;
	char boneCount[10];
	char notifyCount;
	char assetType;
	bool isDefault;
	unsigned int randomDataShortCount;
	unsigned int indexCount;
	float framerate;
	float frequency;
	float primedLength;
	float loopEntryTime;
	unsigned __int16* names;
	char* dataByte;
	__int16* dataShort;
	int* dataInt;
	__int16* randomDataShort;
	char* randomDataByte;
	int* randomDataInt;
	XAnimIndices indices;
	XAnimNotifyInfo* notify;
	XAnimDeltaPart* deltaPart;
};

struct XAnimEntry
{
	unsigned __int16 bCreated;
	unsigned __int16 numAnims;
	unsigned __int16 firstParamIxPlusOne;
	unsigned __int16 paramCount;
	unsigned __int16 parent;
	union {
		XAnimParts* parts;
		XAnimParent animParent;
	};
};

struct XAnim_s
{
	const char* debugName;
	unsigned int size;
	unsigned int paramSize;
	XAnimParam* params;
	const char** debugAnimNames;
	bool* wasLoggedIfMissing;
	XAnimEntry entries[1];
};

void XAnimFree(XAnimParts* parts);
void XAnimFreeList(XAnim_s* anims);