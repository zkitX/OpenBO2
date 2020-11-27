#include "pch.h"
#include "bdSecurityKeyMap.h"

#include <bdCore/bdTrulyRandom.h>

char bdSecurityKeyMap::initKey(bdSecurityID* id, bdSecurityKey* key)
{
    bdTrulyRandomImpl* random;

    random-> 
}

bool bdSecurityKeyMap::contains(const bdSecurityID* id)
{
    return false;
}

bool bdSecurityKeyMap::get(const bdSecurityID* id, bdSecurityKey* key)
{
    return false;
}

bool bdSecurityKeyMap::unregisterKey(const bdSecurityID* id)
{
    return false;
}

bdSecurityKeyMap::bdSecurityKeyMap()
{
}

void bdSecurityKeyMap::clear()
{
}

bool bdSecurityKeyMap::registerKey(const bdSecurityID* id, const bdSecurityKey* key)
{
    return false;
}

bdSecurityKeyMap::~bdSecurityKeyMap()
{
}
