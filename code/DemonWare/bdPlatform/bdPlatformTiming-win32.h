#pragma once
class bdPlatformTiming
{
public:
	static void sleep(unsigned int);
	static _LARGE_INTEGER getHiResTimeStamp(void);
	static float getElapsedTime(unsigned __int64, unsigned __int64);
	static unsigned int getLoResTimeStamp(void);
	static unsigned int getLoResElapsedTime(unsigned int, unsigned int);
};

