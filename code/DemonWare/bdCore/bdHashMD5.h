#pragma once
class bdHashMD5
{
private:
	ltc_hash_descriptor md5_desc;
public:
	bdHashMD5();
	bool hash(const char* data, const unsigned int dataSize, char* result, unsigned int* resultSize);
};
