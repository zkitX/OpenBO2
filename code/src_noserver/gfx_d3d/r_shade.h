#pragma once

#include <d3d11.h>
#include <universal/com_math.h>

struct ShaderConstantSet
{
	vec4_t value[7];
	char constantSource[7];
	char used;
};

struct GfxPixelShaderLoadDef
{
	char* program;
	unsigned int programSize;
};

struct GfxVertexShaderLoadDef
{
	char* program;
	unsigned int programSize;
};
