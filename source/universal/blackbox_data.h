#pragma once
#include "blackbox.h"

class blackbox_data
{
public:
	// _________________________________________________________________________________
	// Structure declarations
	// _________________________________________________________________________________
	struct DefinitionMap
	{
		unsigned int nameHash;
		unsigned int formatHash;
		int parameterCount;
		int definitionId;
		char parameters[64];
	};
	// _________________________________________________________________________________
	// Variable declarations
	// _________________________________________________________________________________

	// _________________________________________________________________________________
	// Function declarations
	// _________________________________________________________________________________
	int BB_BeginDefinition(const char* name);
	int BB_CacheString(const char* str, int len);
	void BB_ClearStringCache();
	DefinitionMap* BB_GetDefinitionMap(unsigned int nameHash, unsigned int formatHash);
	void BB_InitStringCache();
	void BB_MsgInitPPP(blackbox::bb_msg_t* msg, void* pppHashTable, int size);
	int BB_ParseAndCacheFormatString(blackbox::bb_msg_t* msg, const char* name, const char* fmt, int* parameterCount, const char** parameters);
	void BB_RewriteDefinitions(blackbox::bb_msg_t* msg);

private:
	// _________________________________________________________________________________
	// Variable declarations
	// _________________________________________________________________________________
	unsigned int s_stringCollisions;
	unsigned int s_stringHashTable[1024];
	unsigned int s_stringHitBytes;
	unsigned int s_stringHits;
	unsigned int s_stringMisses;
};

