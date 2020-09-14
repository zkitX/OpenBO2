#include "win_main.h"
#include "timing.h"

#include <qcommon/threads.h>

void Sys_FindInfo()
{
	sys_info.logicalCpuCount = Sys_GetCpuCount();
	sys_info.cpuGHz = 1.0 / (((double)1i64 - (double)0i64) * msecPerRawTimerTick * 1000000.0);
	sys_info.sysMB = Sys_SystemMemoryMB();
	Sys_DetectVideoCard(512, sys_info.gpuDescription);
	Sys_DetectCpuVendorAndName(sys_info.cpuVendor, sys_info.cpuName);
	Sys_SetAutoConfigureGHz(&sys_info);
}
