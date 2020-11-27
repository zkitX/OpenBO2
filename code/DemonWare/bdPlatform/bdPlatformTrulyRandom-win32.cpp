#include "pch.h"

s_randomInit::s_randomInit()
{
	CryptAcquireContextA(&s_provider, 0, 0, 1u, 0xF0000000);
}

s_randomInit::~s_randomInit()
{
	CryptReleaseContext(s_provider, 0);
}

void bdGetRandomUChar8(char* in, const unsigned int length)
{
	CryptGenRandom(s_provider, length, (BYTE*)in);
}