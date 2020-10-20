#pragma once

#include <Windows.h>

#include "bdPlatformThread-win32.h"
#include "bdPlatformSemaphore-win32.h"
#include "bdPlatformSocket.h"
#include "bdPlatformMutex-win32.h"
#include "bdLinkable.h"

#define __CFADD__(X,Y) (((unsigned int)(X)+(unsigned int)(Y)) >> 8 ? 1 : 0)

#define BYTEn(x, n)   (*((unsigned char*)&(x)+n))
#define DWORDn(x, n)  (*((*)&(x)+n))

#define LODWORD(x) DWORDn(x,LOW_IND(x,long long))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,long long))