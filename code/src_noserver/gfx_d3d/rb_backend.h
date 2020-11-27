#pragma once
#include <qcommon/com_clients.h>

struct GfxSaveScreenParam
{
	float s0;
	float t0;
	float ds;
	float dt;
	LocalClientNum_t screenTimerId;
	char mode;
};

struct GfxBlendSaveScreenBlurredParam
{
	int fadeMsec;
	float s0;
	float t0;
	float ds;
	float dt;
	int screenTimerId;
	char enabled;
};

struct GfxBlendSaveScreenFlashedParam
{
	float intensityWhiteout;
	float intensityScreengrab;
	float s0;
	float t0;
	float ds;
	float dt;
	int screenTimerId;
	char enabled;
};
