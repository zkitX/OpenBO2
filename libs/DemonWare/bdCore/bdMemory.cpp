#include "pch.h"

void bdMemory::setAllocateFunc(void* (* const allocator)(const unsigned int))
{
	m_allocateFunc = allocator;
}

void bdMemory::setDeallocateFunc(void(* const deallocator)(void*))
{
	m_deallocateFunc = deallocator;
}

void bdMemory::setReallocateFunc(void* (* const reallocator)(void*, const unsigned int))
{
	m_reallocateFunc = reallocator;
}

void bdMemory::setAlignedAllocateFunc(void* (* const allocator)(const unsigned int, const unsigned int))
{
	m_alignedAllocateFunc = allocator;
}

void bdMemory::setAlignedDeallocateFunc(void(* const deallocator)(void*))
{
	m_alignedDeallocateFunc = deallocator;
}

void bdMemory::setAlignedReallocateFunc(void* (* const reallocator)(void*, const unsigned int, const unsigned int))
{
	m_alignedReallocateFunc = reallocator;
}

void* (*bdMemory::getAllocateFunc())(const unsigned int)
{
	return m_allocateFunc;
}

void(*bdMemory::getDeallocateFunc())(void*)
{
	return m_deallocateFunc;
}

void* (*bdMemory::getReallocateFunc())(void*, const unsigned int)
{
	return m_reallocateFunc;
}

void* bdMemory::allocate(const unsigned int size)
{
	void* result; // eax
	void* v2; // esi

	result = 0;
	if (bdMemory::m_allocateFunc)
	{
		bdMutex::lock(&g_MemoryThreadLock);
		v2 = bdMemory::m_allocateFunc(size);
		bdMutex::unlock(&g_MemoryThreadLock);
		if (!v2)
			DebugBreak();
		result = v2;
	}
	return result;
}

void bdMemory::deallocate(void* p)
{
	if (bdMemory::m_deallocateFunc)
	{
		bdMutex::lock(&g_MemoryThreadLock);
		bdMemory::m_deallocateFunc(p);
		bdMutex::unlock(&g_MemoryThreadLock);
	}
}

void* bdMemory::reallocate(void* p, const unsigned int size)
{
	void* result; // eax
	void* v3; // esi

	result = 0;
	if (bdMemory::m_reallocateFunc)
	{
		bdMutex::lock(&g_MemoryThreadLock);
		v3 = bdMemory::m_reallocateFunc(p, size);
		bdMutex::unlock(&g_MemoryThreadLock);
		if (!v3)
			DebugBreak();
		result = v3;
	}
	return result;
}
