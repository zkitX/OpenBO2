#include "bdRandom.h"

bdRandom::bdRandom()
{
	unsigned int timestamp;

	timestamp = bdPlatformTiming::getHiResTimeStamp();
	bdRandom::setSeed(timestamp);
}

signed int bdRandom::nextUInt()
{
	signed int result;

	result = bdRandom::a * (this->m_val % bdRandom::q) - bdRandom::r * (this->m_val / bdRandom::q);
	if (result <= 0)
		result += bdRandom::r;
	this->m_val = result;
	return result;
}

void bdRandom::nextUBytes(unsigned char* in, const int length) {
	int i;

	for (i = 0; i < length; ++i)
		in[i] = bdRandom::nextUInt();
}

void bdRandom::setSeed(const unsigned int seed) {
	this->m_val = seed;
	if (!seed)
	{
		//bdLogMessage(
		//	BD_LOG_WARNING,
		//	"warn/",
		//	"random",
		//	__FILE__,
		//	__FUNCTION__,
		//	0x40u,
		//	"Shouldn't use 0 for seed. 12,195,257 used instead.");
		this->m_val = 12195257;
	}
}