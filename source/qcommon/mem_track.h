#pragma once

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

