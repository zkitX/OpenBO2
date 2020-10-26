#include "pch.h"
#include "bdMallocMemory.h"

bdMallocMemory::bdMemoryChainElement* bdMallocMemory::recordMemory(bdMemoryChainElement* link, const unsigned int size, const bool aligned)
{
    if (!link)
        return 0;
    bdMutex::lock(&bdMallocMemory::m_mutex);
    link->m_magic = 0xBDBD;
    link->m_size = size;
    link->m_aligned = aligned;
    link->m_next = m_memoryChain;
    link->m_prev = 0;
    if (m_memoryChain)
        m_memoryChain->m_prev = link;
    m_allocatedBytes += size;
    ++m_numAllocations;
    m_memoryChain = link;
    bdMutex::unlock(&bdMallocMemory::m_mutex);
    return link + 1;
}

void bdMallocMemory::eraseMemory(bdMemoryChainElement* link)
{
    bdMallocMemory::bdMemoryChainElement* v1; // eax
    bdMallocMemory::bdMemoryChainElement* v2; // eax

    bdMutex::lock(&bdMallocMemory::m_mutex);
    if (link->m_magic != 0xBDBD)
    {
        bdMutex::unlock(&bdMallocMemory::m_mutex);
        bdLogMessage(
            bdLogMessageType::BD_LOG_ERROR,
            "err/",
            "mallocmemory",
            __FILE__,
            __FUNCTION__,
            __LINE__,
            " BD_MEMORY_MAGIC is incorrect.");
        bdMutex::lock(&bdMallocMemory::m_mutex);
    }
    v1 = link->m_prev;
    if (v1)
        v1->m_next = link->m_next;
    else
        bdMallocMemory::m_memoryChain = link->m_next;
    v2 = link->m_next;
    if (v2)
        v2->m_prev = link->m_prev;
    bdMallocMemory::m_allocatedBytes -= link->m_size;
    --bdMallocMemory::m_numAllocations;
    bdMutex::unlock(&bdMallocMemory::m_mutex);
}

bdMallocMemory::bdMemoryChainElement* bdMallocMemory::allocate(const unsigned int size)
{
    bdMallocMemory::bdMemoryChainElement* v1; // eax

    v1 = (bdMallocMemory::bdMemoryChainElement*)bdAlignedOffsetMalloc(size + 20, 8u, 0x14u);
    return bdMallocMemory::recordMemory(v1, size, 0);
}

bdMallocMemory::bdMemoryChainElement* bdMallocMemory::reallocate(void* p, const unsigned int size)
{
    unsigned int v2; // edi
    char* v3; // eax

    if (!p)
        return bdMallocMemory::allocate(size);
    v2 = *((_DWORD*)p - 4) + 20;
    bdMallocMemory::eraseMemory((bdMallocMemory::bdMemoryChainElement*)p - 1);
    v3 = bdAlignedOffsetRealloc((char*)p - 20, v2, size + 20, 8u, 0x14u);
    return bdMallocMemory::recordMemory((bdMallocMemory::bdMemoryChainElement*)v3, size, 0);
}

bdMallocMemory::bdMemoryChainElement* bdMallocMemory::alignedAllocate(const unsigned int size, const unsigned int align)
{
    bdMallocMemory::bdMemoryChainElement* v2; // eax

    v2 = (bdMallocMemory::bdMemoryChainElement*)bdAlignedOffsetMalloc(size + 20, align, 0x14u);
    return bdMallocMemory::recordMemory(v2, size, 1);
}

void bdMallocMemory::deallocate(void* p)
{
    if (p)
    {
        bdMallocMemory::eraseMemory((bdMallocMemory::bdMemoryChainElement*)p - 1);
        bdAlignedOffsetFree((char*)p - 20);
    }
}

bdMallocMemory::bdMemoryChainElement* bdMallocMemory::alignedReallocate(void* p, const unsigned int size, const unsigned int align)
{
    unsigned int v3; // edi
    char* v4; // eax

    if (!p)
        return bdMallocMemory::alignedAllocate(size, align);
    v3 = *((_DWORD*)p - 4) + 20;
    bdMallocMemory::eraseMemory((bdMallocMemory::bdMemoryChainElement*)p - 1);
    v4 = bdAlignedOffsetRealloc((char*)p - 20, v3, size + 20, align, 0x14u);
    return bdMallocMemory::recordMemory((bdMallocMemory::bdMemoryChainElement*)v4, size, 1);
}
