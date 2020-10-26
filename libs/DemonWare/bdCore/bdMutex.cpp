#include <pch.h>

bdMutex::bdMutex()
{
	this->m_handle = bdPlatformMutex::createMutex();
}

bdMutex::~bdMutex()
{
	bdPlatformMutex::destroy(&this->m_handle);
}

void bdMutex::unlock(bdMutex* instance)
{
	bdPlatformMutex::unlock(&instance->m_handle);
}

void bdMutex::lock(bdMutex* instance)
{
	bdPlatformMutex::lock(&instance->m_handle);
}
