#include "pch.h"
#include "bdChunk.h"

bdChunk::bdChunk(bdChunkTypes type)
{
}

bdChunk::~bdChunk()
{
}

unsigned int bdChunk::getSerializedSize()
{
    return this->serialize(0, 0);
}

bool bdChunk::isControl()
{
    return this->m_type != 2;
}

bdChunkTypes bdChunk::getType(const char* const data, const unsigned int size)
{
    return bdChunkTypes();
}

unsigned int bdChunk::serialize(char* data, const unsigned int size)
{
    return 0;
}

char bdChunk::deserialize(const char* const data, const unsigned int size, unsigned int* offset)
{
    return 0;
}
