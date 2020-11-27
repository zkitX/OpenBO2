#pragma once

HCRYPTPROV s_provider;

void bdGetRandomUChar8(char* in, const unsigned int length);

class s_randomInit {
	s_randomInit();
	~s_randomInit();
};