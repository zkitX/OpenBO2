#include "timing.h"
#include <Windows.h>

void InitTiming()
{
	LARGE_INTEGER Frequency;

	Sleep(0);
	QueryPerformanceFrequency(&Frequency);
	msecPerRawTimerTick = 1.0 / (double)Frequency.QuadPart * 1000.0;
	Sleep(0);
	QueryPerformanceFrequency(&Frequency);
	usecPerRawTimerTick = 1.0 / (double)Frequency.QuadPart * 1000.0 * 1000.0;
}
