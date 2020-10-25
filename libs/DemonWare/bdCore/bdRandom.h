#pragma once

class bdRandom
{
private:
	unsigned int m_val{};
	const int a = 0x41A7;
	const int m = 0x7FFFFFFF;
	const int q = 0x1F31D;
	const int r = 0x0B14;

	void setSeed(unsigned int);
	signed int nextUInt();
public:
	bdRandom();
	virtual ~bdRandom() = 0;
	void nextUBytes(unsigned char* const, int);
};