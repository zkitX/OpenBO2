#pragma once
namespace bdBytePacker
{
	bool removeBuffer(const void* src, const unsigned int srcSize, const unsigned int offset, unsigned int* newOffset, void* const dest, const unsigned int readSize);

};

