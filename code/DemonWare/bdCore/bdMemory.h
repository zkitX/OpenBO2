#pragma once
class bdMemory
{
private:
	static void*(*m_allocateFunc)(const unsigned int);
	static void(*m_deallocateFunc)(void*);
	static void*(*m_reallocateFunc)(void*, const unsigned int);
	static void*(*m_alignedAllocateFunc)(const unsigned int, const unsigned int);
	static void(*m_alignedDeallocateFunc)(void*);
	static void*(*m_alignedReallocateFunc)(void*, const unsigned int, const unsigned int);
	static bdMutex g_MemoryThreadLock;
public:
	static void setAllocateFunc(void* (* const allocator)(const unsigned int));
	static void setDeallocateFunc(void(* const deallocator)(void*));
	static void setReallocateFunc(void* (* const reallocator)(void*, const unsigned int));
	static void setAlignedAllocateFunc(void* (* const allocator)(const unsigned int, const unsigned int));
	static void setAlignedDeallocateFunc(void(* const deallocator)(void*));
	static void setAlignedReallocateFunc(void* (* const reallocator)(void*, const unsigned int, const unsigned int));
	void* (*getAllocateFunc())(const unsigned int);
	void(*getDeallocateFunc())(void*);
	void* (*getReallocateFunc())(void*, const unsigned int);
	static void* allocate(const unsigned int size);
	static void deallocate(void* p);
	static void* reallocate(void* p, const unsigned int size);
};