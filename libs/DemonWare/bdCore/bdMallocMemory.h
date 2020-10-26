#pragma once
class bdMallocMemory
{
public:
	struct bdMemoryChainElement {
		unsigned __int16 m_magic;
		unsigned int m_size;
		bool m_aligned;
		bdMemoryChainElement* m_prev;
		bdMemoryChainElement* m_next;
	};

	static bdMutex m_mutex;
	static bdMemoryChainElement* m_memoryChain;
	static unsigned int m_allocatedBytes;
	static unsigned int m_numAllocations;

	static bdMemoryChainElement* recordMemory(bdMemoryChainElement* link, const unsigned int size, const bool aligned);
	static void eraseMemory(bdMemoryChainElement* link);
	static bdMemoryChainElement* allocate(const unsigned int size);
	static bdMemoryChainElement* reallocate(void* p, const unsigned int size);
	static bdMemoryChainElement* alignedAllocate(const unsigned int size, const unsigned int align);
	static void deallocate(void* p);
	static bdMemoryChainElement* alignedReallocate(void* p, const unsigned int size, const unsigned int align);
};

