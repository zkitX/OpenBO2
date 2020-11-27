#pragma once

struct GfxCodeImageRenderTargetFields
{
	unsigned __int64 renderTargetId : 8;
	unsigned __int64 enable : 1;
	unsigned __int64 fbufferTexture : 3;
	unsigned __int64 filtering : 20;
};

union GfxCodeImageRenderTarget
{
	GfxCodeImageRenderTargetFields fields;
	unsigned int packed;
};
