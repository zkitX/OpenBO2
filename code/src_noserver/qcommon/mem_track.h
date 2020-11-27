#pragma once

enum EMemTrack
{
	TRACK_DEBUG = 0x0,
	TRACK_HUNK = 0x1,
	TRACK_BINARIES = 0x2,
	TRACK_MISC_SWAP = 0x3,
	TRACK_DELIMITER1 = 0x4,
	TRACK_AI = 0x5,
	TRACK_AI_NODES = 0x6,
	TRACK_SCRIPT = 0x7,
	TRACK_SCRIPT_DEBUG = 0x8,
	TRACK_FX = 0x9,
	TRACK_GLASS = 0xA,
	TRACK_NETWORK_ENTITY = 0xB,
	TRACK_MISC = 0xC,
	TRACK_FASTFILE = 0xD,
	TRACK_ANIMATION = 0xE,
	TRACK_ANIMCACHE = 0xF,
	TRACK_WORLD_GLOBALS = 0x10,
	TRACK_SOUND_GLOBALS = 0x11,
	TRACK_CLIENT_ANIMSCRIPT = 0x12,
	TRACK_SOUND = 0x13,
	TRACK_DELIMITER2 = 0x14,
	TRACK_RENDERER_GLOBALS = 0x15,
	TRACK_RENDERER_IMAGES = 0x16,
	TRACK_RENDERER_WORLD = 0x17,
	TRACK_RENDERER_MODELS = 0x18,
	TRACK_RENDERER_MISC = 0x19,
	TRACK_RENDERER_SIEGE = 0x1A,
	TRACK_CINEMATICS = 0x1B,
	TRACK_DELIMITER3 = 0x1C,
	TRACK_COLLISION_MISC = 0x1D,
	TRACK_COLLISION_BRUSH = 0x1E,
	TRACK_COLLISION_MODEL_TRI = 0x1F,
	TRACK_COLLISION_TERRAIN = 0x20,
	TRACK_PHYSICS = 0x21,
	TRACK_MAP_ENTS = 0x22,
	TRACK_TEMP = 0x23,
	TRACK_DELIMITER4 = 0x24,
	TRACK_LOCALIZATION = 0x25,
	TRACK_FLAME = 0x26,
	TRACK_UI = 0x27,
	TRACK_TL = 0x28,
	TRACK_ZMEM = 0x29,
	TRACK_FIREMANAGER = 0x2A,
	TRACK_PROFILE = 0x2B,
	TRACK_CLIENT = 0x2C,
	TRACK_RECORDER = 0x2D,
	TRACK_RSTREAM = 0x2E,
	TRACK_RENDERER_STREAMBUFFER = 0x2F,
	TRACK_RENDERER_STREAMBUFFER_EXTRA = 0x30,
	TRACK_GEOSTREAM = 0x31,
	TRACK_DDL = 0x32,
	TRACK_ONLINE = 0x33,
	TRACK_EMBLEM = 0x34,
	TRACK_VEHICLEDEF = 0x35,
	TRACK_THREAD_LOCAL = 0x36,
	TRACK_MINSPEC_IMAGES = 0x37,
	TRACK_DELIMITER5 = 0x38,
	TRACK_NONE = 0x39,
	TRACK_COUNT = 0x3A,
};

struct meminfo_t
{
	int total[2];
	int nonSwapTotal;
	int nonSwapMinSpecTotal;
	int typeTotal[58][2];
};


struct mem_track_t
{
	char name[128];
	const char* filename;
	int size[2];
	int pos;
	char type;
	char usageType;
	int count;
};

struct mem_track_node_s
{
	mem_track_t data;
	int project;
	mem_track_node_s* prev;
	mem_track_node_s* next;
};

struct TempMemInfo
{
	int permanent;
	int high;
	int highExtra;
	int hunkSize;
	int low;
	mem_track_t data;
};

meminfo_t g_info;
meminfo_t g_virtualMemInfo;
TempMemInfo g_mallocMemInfoArray[1500];
int g_mallocMemInfoCount;
int g_malloc_mem_high;
int g_malloc_mem_size;
mem_track_t g_staticsMemTrack[2048];
mem_track_node_s* g_ZMallocMemTrackList;

bool inited_0;
const char aInternal[] = "internal";

TempMemInfo* GetTempMemInfo(int permanent, const char* name, int type, int usageType, TempMemInfo* tempMemInfoArray, int* tempMemInfoCount, bool add_if_missing);

void track_init();
void track_physical_alloc(int size, const char* name, int type, int location);
void track_z_alloc(int size, const char* name, int type, void* pos, int project, int overhead);
void track_z_free(int type, void* pos, int overhead);

class mem_track
{
};

