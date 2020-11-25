#pragma once
#include "r_shared.h"

struct GfxPortal;

struct GfxWaterBuffer
{
	unsigned int bufferSize;
	vec4_t* buffer;
};

struct GfxPortalWritable
{
	bool isQueued;
	bool isAncestor;
	char recursionDepth;
	char hullPointCount;
	vec2_t* hullPoints;
	GfxPortal* queuedParent;
};

struct GfxPortal
{
	GfxPortalWritable writable;
	DpvsPlane plane;
	GfxCell* cell;
	vec3_t* vertices;
	char vertexCount;
	vec3_t hullAxis[2];
	vec3_t bounds[2];
};

struct GfxAabbTree
{
	vec3_t mins;
	vec3_t maxs;
	unsigned __int16 childCount;
	unsigned __int16 surfaceCount;
	unsigned __int16 startSurfIndex;
	unsigned __int16 smodelIndexCount;
	unsigned __int16* smodelIndexes;
	int childrenOffset;
};

struct GfxStreamingAabbTree
{
	vec4_t mins;
	vec4_t maxs;
	float maxStreamingDistance;
	unsigned __int16 firstItem;
	unsigned __int16 itemCount;
	unsigned __int16 firstChild;
	unsigned __int16 childCount;
	unsigned __int16 smodelCount;
	unsigned __int16 surfaceCount;
};

struct GfxCell
{
	vec3_t mins;
	vec3_t maxs;
	int aabbTreeCount;
	GfxAabbTree* aabbTree;
	int portalCount;
	GfxPortal* portals;
	char reflectionProbeCount;
	char* reflectionProbes;
};

struct GfxWorldStreamInfo
{
	int aabbTreeCount;
	GfxStreamingAabbTree* aabbTrees;
	int leafRefCount;
	int* leafRefs;
};

struct GfxWorld
{
	const char* name;
	const char* baseName;
	int planeCount;
	int nodeCount;
	int surfaceCount;
	GfxWorldStreamInfo streamInfo;
	const char* skyBoxModel;
	SunLightParseParams sunParse;
	GfxLight* sunLight;
	unsigned int sunPrimaryLightIndex;
	unsigned int primaryLightCount;
	unsigned int coronaCount;
	GfxLightCorona* coronas;
	unsigned int shadowMapVolumeCount;
	GfxShadowMapVolume* shadowMapVolumes;
	unsigned int shadowMapVolumePlaneCount;
	GfxVolumePlane* shadowMapVolumePlanes;
	unsigned int exposureVolumeCount;
	GfxExposureVolume* exposureVolumes;
	unsigned int exposureVolumePlaneCount;
	GfxVolumePlane* exposureVolumePlanes;
	unsigned int worldFogVolumeCount;
	GfxWorldFogVolume* worldFogVolumes;
	unsigned int worldFogVolumePlaneCount;
	GfxVolumePlane* worldFogVolumePlanes;
	unsigned int worldFogModifierVolumeCount;
	GfxWorldFogModifierVolume* worldFogModifierVolumes;
	unsigned int worldFogModifierVolumePlaneCount;
	GfxVolumePlane* worldFogModifierVolumePlanes;
	unsigned int lutVolumeCount;
	GfxLutVolume* lutVolumes;
	unsigned int lutVolumePlaneCount;
	GfxVolumePlane* lutVolumePlanes;
	GfxSkyDynamicIntensity skyDynIntensity;
	GfxWorldDpvsPlanes dpvsPlanes;
	int cellBitsCount;
	GfxCell* cells;
	GfxWorldDraw draw;
	GfxLightGrid lightGrid;
	int modelCount;
	GfxBrushModel* models;
	vec3_t mins;
	vec3_t maxs;
	unsigned int checksum;
	int materialMemoryCount;
	MaterialMemory* materialMemory;
	sunflare_t sun;
	vec4_t outdoorLookupMatrix[4];
	GfxImage* outdoorImage;
	unsigned int* cellCasterBits;
	GfxSceneDynModel* sceneDynModel;
	GfxSceneDynBrush* sceneDynBrush;
	unsigned int* primaryLightEntityShadowVis;
	unsigned int* primaryLightDynEntShadowVis[2];
	unsigned int numSiegeSkinInsts;
	SSkinInstance* siegeSkinInsts;
	GfxShadowGeometry* shadowGeom;
	GfxLightRegion* lightRegion;
	GfxWorldDpvsStatic dpvs;
	GfxWorldDpvsDynamic dpvsDyn;
	float waterDirection;
	GfxWaterBuffer waterBuffers[2];
	const Material* waterMaterial;
	const Material* coronaMaterial;
	const Material* ropeMaterial;
	const Material* lutMaterial;
	unsigned int numOccluders;
	Occluder* occluders;
	unsigned int numOutdoorBounds;
	GfxOutdoorBounds* outdoorBounds;
	unsigned int heroLightCount;
	unsigned int heroLightTreeCount;
	GfxHeroLight* heroLights;
	GfxHeroLightTree* heroLightTree;
	unsigned int lightingFlags;
	int lightingQuality;
};
