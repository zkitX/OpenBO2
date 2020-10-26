#include "pch.h"

void bdCore::init(const bool defaultMemoryFunctions)
{
	if (m_initialized)
	{
		bdLogMessage(
			bdLogMessageType::BD_LOG_WARNING,
			"warn/",
			"core",
			__FILE__,
			__FUNCTION__,
			__LINE__,
			"init() has been called twice without an intermediate quit()");
	}
	else
	{
		if (defaultMemoryFunctions)
		{
			bdMemory::setAllocateFunc((void* (__cdecl* const)(const unsigned int))bdMallocMemory::allocate);
			bdMemory::setAlignedAllocateFunc((void* (__cdecl* const)(const unsigned int, const unsigned int))bdMallocMemory::alignedAllocate);
			bdMemory::setDeallocateFunc(bdMallocMemory::deallocate);
			bdMemory::setAlignedDeallocateFunc(bdMallocMemory::deallocate);
			bdMemory::setReallocateFunc((void* (__cdecl* const)(void*, const unsigned int))bdMallocMemory::reallocate);
			bdMemory::setAlignedReallocateFunc((void* (__cdecl* const)(void*, const unsigned int, const unsigned int))bdMallocMemory::alignedReallocate);
		}
	}
	m_initialized = true;
}