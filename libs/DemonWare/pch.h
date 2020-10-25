#pragma once

#ifndef PCH_H
#define PCH_H

#include <Windows.h>
#include <iostream>

typedef struct
{
	char* _ptr;
	int _cnt;
	char* _base;
	int _flag;
	int _file;
	int _charbuf;
	int _bufsiz;
	char* _tmpfname;
} iobuf;

iobuf _iob[20];

inline iobuf* __iob_func() {
	return _iob;
}

#include "bdPlatform/bdLinkable.h"
#include "bdPlatform/bdPlatformMutex-win32.h"
#include "bdPlatform/bdPlatformSemaphore-win32.h"
#include "bdPlatform/bdInAddr.h"
#include "bdPlatform/bdPlatformSocket.h"
#include "bdPlatform/bdPlatformString.h"
#include "bdPlatform/bdPlatformThread-win32.h"
#include "bdPlatform/bdPlatformTiming-win32.h"
#include "bdPlatform/bdPlatformTrulyRandom-win32.h"
#include "bdPlatform/bdPlatformLog.h"

#include "bdCore/bdMutex.h"
#include "bdCore/bdRandom.h"
#include "bdCore/bdRunnable.h"

#include <idadefs.h>

#endif //PCH_H