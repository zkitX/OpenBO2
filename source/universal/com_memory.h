#pragma once

struct MemoryFile
{
	char* buffer;
	int bufferSize;
	int bytesUsed;
	char* careerStatsBuffer;
	int cacheBufferUsed;
	int segmentIndex;
	int segmentStart;
	int nonZeroCount;
	int zeroCount;
	bool errorOnOverflow;
	bool memoryOverflow;
	bool compress_enabled;
	bool rle_enabled;
	bool is_writing;
	int cacheBufferAvail;
	char cacheBuffer[32760];
};

void Com_Memset(void* dest, const int val, int count);

void* Z_Malloc(int size, const char* name, int type);
char* Z_MallocGarbage(int size, const char* name, int type);

