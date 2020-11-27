#include "pch.h"
#include "bdTrulyRandom.h"

void bdTrulyRandomImpl::getRandomUByte8(char* in, const unsigned int length)
{
    bdGetRandomUChar8(in, length);
}

unsigned int bdTrulyRandomImpl::getRandomUInt()
{
    char randomBytes[4];

    getRandomUByte8(randomBytes, 4);
    return (unsigned int)randomBytes;
}
