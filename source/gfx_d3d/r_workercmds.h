#pragma once
#include "r_rendercmds.h"

#include <d3d11.h>
#include <universal/com_workercmds.h>

class GfxWorkerData
{
	int cmdBufValid[33];
	GfxCmdBuf cmdBuf[33];
	jqBatchGroup cmdBufBatchGroup[33];
};