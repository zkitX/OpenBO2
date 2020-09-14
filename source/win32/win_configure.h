#pragma once

#include <Windows.h>

struct SysInfo
{
	long double cpuGHz;
	long double configureGHz;
	int logicalCpuCount;
	int physicalCpuCount;
	int sysMB;
	char gpuDescription[512];
	char cpuVendor[13];
	char cpuName[49];
};

const char* unknownVideoCard = "Unknown Video Card";

const double DOUBLE_1_0 = 1.0;
const double DOUBLE_1_75 = 1.75;
const double DOUBLE_2_0 = 2.0;

int Sys_SystemMemoryMB();
void Sys_DetectVideoCard(int descLimit, char* description);
void Sys_CopyCpuidString(unsigned int maxLen, char* dest, const char* source);
void Sys_DetectCpuVendorAndName(char* vendor, char* name);
void Sys_GetPhysicalCpuCount(SysInfo* sysInfo);
long double Sys_BenchmarkGHz();
void Sys_SetAutoConfigureGHz(SysInfo* sysInfo);