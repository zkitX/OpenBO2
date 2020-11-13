#pragma once
#include <bdCore/bdSecurityID.h>
#include <bdCore/bdSecurityKey.h>

class bdSecurityKeyMap
{
public:
	char initKey(bdSecurityID* id, bdSecurityKey* key);
	bool contains(const bdSecurityID* id);
	bool get(const bdSecurityID* id, bdSecurityKey* key);
	bool unregisterKey(const bdSecurityID* id);
	bdSecurityKeyMap();
	void clear();
	bool registerKey(const bdSecurityID* id, const bdSecurityKey* key);
	~bdSecurityKeyMap();
};

