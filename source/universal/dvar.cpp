#include "dvar.h"

#include <defs.h>
#include <devgui/devgui.h>
#include <qcommon/common.h>
#include <qcommon/threads.h>
#include <universal/assertive.h>
#include <universal/com_math.h>
#include <universal/com_memory.h>
#include <win32/win_net.h>
#include <win32/win_shared.h>

const char* s_dvarTypeNames[13]{
	"bool",
	"float",
	"float2",
	"float3",
	"float4",
	"int",
	"enum",
	"string",
	"color",
	"int64",
	"linColorRGB",
	"colorXYZ"
};

void Dvar_SetInAutoExec(bool inAutoExec)
{
	s_isLoadingAutoExecGlobalFlag = inAutoExec;
}

bool Dvar_IsSystemActive()
{
	return s_isDvarSystemActive;
}

bool Dvar_IsValidName(const char* dvarName)
{
	const char* v1; // esi
	char v3; // bl

	v1 = dvarName;
	if (!dvarName)
		return false;
	if (!*dvarName)
		return true;
	while (1)
	{
		v3 = *v1;
		if (!isalnum(*v1) && v3 != 95)
			break;
		if (!*++v1)
			return true;
	}
	return false;
}

void Dvar_CopyString(const char* string, DvarValue* value)
{
	if (!string && !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
		__debugbreak();
	value->integer = (int)CopyString(string);
}

void Dvar_WeakCopyString(const char* string, DvarValue* value)
{
	if (!string && !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
		__debugbreak();
	value->integer = (int)string;
}

bool Dvar_ShouldFreeCurrentString(dvar_t* dvar)
{
	return dvar->current.integer && dvar->current.integer != dvar->latched.integer && dvar->current.integer != dvar->reset.integer;
}

bool Dvar_ShouldFreeLatchedString(dvar_t* dvar)
{
	return dvar->latched.integer && dvar->latched.integer != dvar->current.integer && dvar->latched.integer != dvar->reset.integer;
}

bool Dvar_ShouldFreeResetString(dvar_t* dvar)
{
	return dvar->reset.integer && dvar->reset.integer != dvar->current.integer && dvar->reset.integer != dvar->latched.integer;
}

void Dvar_FreeString(DvarValue* value)
{
	FreeString(value->string);
	value->integer = 0;
}

void Dvar_AssignCurrentStringValue(dvar_t* dvar, DvarValue* dest, const char* string)
{
	const char* v3; // esi
	const char* v4; // esi

	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	v3 = dvar->latched.string;
	if (v3 && (string == v3 || !strcmp(string, dvar->latched.string)))
	{
		dest->integer = (int)v3;
	}
	else
	{
		v4 = dvar->reset.string;
		if (v4 && (string == v4 || !strcmp(string, dvar->reset.string)))
		{
			dest->integer = (int)v4;
		}
		else
		{
			if (!string
				&& !Assert_MyHandler(
					__FILE__,
					__LINE__,
					0,
					"(string)",
					nullptr))
			{
				__debugbreak();
			}
			dest->integer = (int)CopyString(string);
		}
	}
}

void Dvar_AssignLatchedStringValue(dvar_t* dvar, DvarValue* dest, const char* string)
{
	const char* v3; // esi
	const char* v4; // esi

	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	v3 = dvar->current.string;
	if (v3 && (string == v3 || !strcmp(string, dvar->current.string)))
	{
		dest->integer = (int)v3;
	}
	else
	{
		v4 = dvar->reset.string;
		if (v4 && (string == v4 || !strcmp(string, dvar->reset.string)))
		{
			dest->integer = (int)v4;
		}
		else
		{
			if (!string
				&& !Assert_MyHandler(
					__FILE__,
					__LINE__,
					0,
					"(string)",
					nullptr))
			{
				__debugbreak();
			}
			dest->integer = (int)CopyString(string);
		}
	}
}

void Dvar_AssignResetStringValue(dvar_t* dvar, DvarValue* dest, const char* string)
{
	const char* v3; // esi
	const char* v4; // esi

	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	v3 = dvar->current.string;
	if (v3 && (string == v3 || !strcmp(string, dvar->current.string)))
	{
		dest->integer = (int)v3;
	}
	else
	{
		v4 = dvar->latched.string;
		if (v4 && (string == v4 || !strcmp(string, dvar->latched.string)))
		{
			dest->integer = (int)v4;
		}
		else
		{
			if (!string
				&& !Assert_MyHandler(
					__FILE__,
					__LINE__,
					0,
					"(string)",
					nullptr))
			{
				__debugbreak();
			}
			dest->integer = (int)CopyString(string);
		}
	}
}

char* Dvar_EnumToString(const dvar_t* dvar)
{
	int v1; // eax
	bool v2; // zf
	char* result; // eax

	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	if (!dvar->name
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvar->name)",
			nullptr))
	{
		__debugbreak();
	}
	if (dvar->type != 7
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((dvar->type == DVAR_TYPE_ENUM))",
			"(dvar->name) = %s",
			dvar->name))
	{
		__debugbreak();
	}
	if (!dvar->domain.integer.max
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((dvar->domain.enumeration.strings))",
			"(dvar->name) = %s",
			dvar->name))
	{
		__debugbreak();
	}
	v1 = dvar->current.integer;
	v2 = v1 == 0;
	if (v1 >= 0)
	{
		if (v1 < dvar->domain.enumeration.stringCount)
			goto LABEL_19;
		v2 = v1 == 0;
	}
	if (!v2
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((dvar->current.integer >= 0 && dvar->current.integer < dvar->domain.enumeration.stringCount || dvar->current.integer == 0))",
			"(dvar->current.integer) = %i",
			dvar->current.integer))
	{
		__debugbreak();
	}
LABEL_19:
	if (dvar->domain.enumeration.stringCount)
		result = *(char**)(dvar->domain.integer.max + 4 * dvar->current.integer);
	else
		result = (char*)nullptr;
	return result;
}

char* Dvar_IndexStringToEnumString(const dvar_t* dvar, const char* indexString)
{
	int v3; // edi
	int v4; // esi
	int v5; // eax

	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	if (!dvar->name
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvar->name)",
			nullptr))
	{
		__debugbreak();
	}
	if (dvar->type != 7
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((dvar->type == DVAR_TYPE_ENUM))",
			"(dvar->name) = %s",
			dvar->name))
	{
		__debugbreak();
	}
	if (!dvar->domain.integer.max
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((dvar->domain.enumeration.strings))",
			"(dvar->name) = %s",
			dvar->name))
	{
		__debugbreak();
	}
	if (!indexString
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((indexString))",
			"(dvar->name) = %s",
			dvar->name))
	{
		__debugbreak();
	}
	if (!dvar->domain.enumeration.stringCount)
		return (char*)nullptr;
	v3 = strlen(indexString);
	v4 = 0;
	if (v3 <= 0)
	{
		v5 = atoi(indexString);
		if (v5 >= 0 && v5 < dvar->domain.enumeration.stringCount)
			return *(char**)(dvar->domain.integer.max + 4 * v5);
	}
	else
	{
		while (isdigit(indexString[v4]))
		{
			if (++v4 >= v3) {
				v5 = atoi(indexString);
				if (v5 >= 0 && v5 < dvar->domain.enumeration.stringCount)
					return *(char**)(dvar->domain.integer.max + 4 * v5);
			}
		}
	}
	return nullptr;
}

char* Dvar_ValueToString(const dvar_t* dvar, DvarValue value)
{
	dvarType_t v2; // eax
	char* result; // eax
	bool v4; // zf
	const char* v5; // eax

	v2 = dvar->type;
	switch (v2)
	{
	case 1:
		result = (char*)"1";
		if (!value.enabled)
			result = (char*)"0";
		return result;
	case 2:
		return va("%g", value.value);
	case 3:
		return va("%g %g", value.value, value.vector.v[1]);
	case 4:
	case 11:
	case 12:
		return va("%g %g %g", value.value, value.vector.v[1], value.vector.v[2]);
	case 5:
		return va("%g %g %g %g", value.value, value.vector.v[1], value.vector.v[2], value.vector.v[3]);
	case 6:
		return va("%i", value.integer);
	case 7:
		v4 = value.integer == 0;
		if (value.integer < 0)
			goto LABEL_15;
		if (value.integer >= dvar->domain.enumeration.stringCount)
		{
			v4 = value.integer == 0;
		LABEL_15:
			if (!v4
				&& !Assert_MyHandler(
					__FILE__,
					__LINE__,
					0,
					"((value.integer >= 0 && value.integer < dvar->domain.enumeration.stringCount || value.integer == 0))",
					"(value.integer) = %i",
					value.integer))
			{
				__debugbreak();
			}
		}
		if (!dvar->domain.enumeration.stringCount)
			return nullptr;
		return *(char**)(dvar->domain.integer.max + 4 * value.integer);
	case 8:
		if (!value.integer
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"((value.string))",
				"(dvar->name) = %s",
				dvar->name))
		{
			__debugbreak();
		}
		return va("%s", value.string);
	case 9:
		return va(
			"%g %g %g %g",
			(value.enabled * 0.0039215689),
			(value.color[1] * 0.0039215689),
			(value.color[2] * 0.0039215689),
			(value.color[3] * 0.0039215689));
	case 10:
		return va("%lli", value.integer64);
	default:
		v5 = va("unhandled dvar type '%i'", v2);
		if (!Assert_MyHandler(__FILE__, __LINE__, 1, nullptr, v5))
			__debugbreak();
		return nullptr;
	}
}

bool Dvar_StringToBool(const char* string)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	return atoi(string) != 0;
}

int Dvar_StringToInt(const char* string)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	return atoi(string);
}

__int64 Dvar_StringToInt64(const char* string)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	return I_atoi64(string);
}

float Dvar_StringToFloat(const char* string)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	return atof(string);
}

void Dvar_StringToVec2(const char* string, vec2_t* vector)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	sscanf(string, "%g %g", vector, &vector->y);
}

void Dvar_StringToVec3(const char* string, vec3_t* vector)
{
	char* v2; // [esp-8h] [ebp-8h]
	char* v3; // [esp-4h] [ebp-4h]

	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	v3 = (char*)&vector->v[2];
	vector->x = 0.0;
	vector->z = 0.0;
	v2 = (char*)&vector->v[1];
	if (*string == 40)
		sscanf(string, "( %g, %g, %g )", vector, v2, v3);
	else
		sscanf(string, "%g %g %g", vector, v2, v3);
}

void Dvar_StringToVec4(const char* string, vec4_t* vector)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	*vector->v = 0;
	vector->b = 0;
	sscanf(string, "%g %g %g %g", vector, &vector->g, &vector->b, &vector->a);
}

int Dvar_StringToEnum(DvarLimits const*, const char*)
{
	return 0;
}

void Dvar_StringToColor(char* color, const char* string)
{
	float v2; // xmm1_4
	float v3; // xmm2_4
	float v4; // xmm0_4
	float v5; // xmm1_4
	bool v6; // cf
	float v7; // xmm1_4
	float v8; // xmm1_4
	int v9; // [esp+Ch] [ebp-18h]
	int v10; // [esp+Ch] [ebp-18h]
	int v11; // [esp+Ch] [ebp-18h]
	vec4_t colorVec; // [esp+10h] [ebp-14h]

	*colorVec.v = 0;
	colorVec.b = 0;
	sscanf(string, "%g %g %g %g", &colorVec, &colorVec.g, &colorVec.b, &colorVec.a);
	v2 = colorVec.v[0];
	v3 = 1.0;
	v4 = 0.0;
	if ((float)(colorVec.v[0] - 1.0) >= 0.0)
		v2 = 1.0;
	if (-v2 >= 0.0)
		v2 = 0.0;
	v9 = (int)((float)(v2 * 255.0) + 9.313225746154785e-10);
	v5 = colorVec.v[1];
	v6 = (float)(colorVec.v[1] - 1.0) < 0.0;
	*color = v9;
	if (!v6)
		v5 = 1.0;
	if (-v5 >= 0.0)
		v5 = 0.0;
	v10 = (int)((float)(v5 * 255.0) + 9.313225746154785e-10);
	v7 = colorVec.v[2];
	v6 = (float)(colorVec.v[2] - 1.0) < 0.0;
	color[1] = v10;
	if (!v6)
		v7 = 1.0;
	if (-v7 >= 0.0)
		v7 = 0.0;
	v11 = (int)((float)(v7 * 255.0) + 9.313225746154785e-10);
	v8 = colorVec.v[3];
	v6 = (float)(colorVec.v[3] - 1.0) < 0.0;
	color[2] = v11;
	if (v6)
		v3 = v8;
	if (-v3 < 0.0)
		v4 = *(float*)&v3;
	color[3] = (int)((float)(v4 * 255.0) + 9.313225746154785e-10);
}

DvarValue* Dvar_StringToValue(dvarType_t type, const char* string, DvarLimits domain)
{
	const char* v3; // edi
	dvarType_t v4; // esi
	DvarValue* result; // eax
	const char* v7; // eax

	v3 = string;
	v4 = type;
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
	{
		__debugbreak();
	}
	switch (v4)
	{
	case 1:
		domain.enumeration.stringCount = Dvar_StringToBool(v3);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 2:
		domain.enumeration.stringCount = Dvar_StringToFloat(v3);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 3:
		Dvar_StringToVec2(v3, (vec2_t*)domain.enumeration.stringCount);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 4:
	case 11:
	case 12:
		Dvar_StringToVec3(v3, (vec3_t*)domain.enumeration.stringCount);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 5:
		Dvar_StringToVec4(v3, (vec4_t*)domain.enumeration.stringCount);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 6:
		domain.enumeration.stringCount = Dvar_StringToInt(v3);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 7:
		domain.enumeration.stringCount = Dvar_StringToEnum((const DvarLimits*)((char*)&domain + 4), v3);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 8:
		domain.enumeration.stringCount = (int)v3;
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 9:
		Dvar_StringToColor((char*)domain.enumeration.stringCount, v3);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	case 10:
		domain.enumeration.stringCount = Dvar_StringToInt64(v3);
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	default:
		v7 = va("unhandled dvar type '%i'", v4);
		if (!Assert_MyHandler(__FILE__, __LINE__, 1, nullptr, v7))
			__debugbreak();
		domain.enumeration.stringCount = 0;
		result = (DvarValue*)domain.enumeration.stringCount;
		break;
	}
	return result;
}

char* Dvar_DisplayableValue(const dvar_t* dvar)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	return Dvar_ValueToString(dvar, dvar->current);
}

char* Dvar_DisplayableResetValue(const dvar_t* dvar)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	return Dvar_ValueToString(dvar, dvar->reset);
}

char* Dvar_DisplayableLatchedValue(const dvar_t* dvar)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	return Dvar_ValueToString(dvar, dvar->latched);
}

DvarValue Dvar_ClampValueToDomain(dvarType_t type, DvarValue value, DvarValue resetValue, DvarLimits domain)
{
	return DvarValue();
}

bool Dvar_ValueInDomain(dvarType_t type, DvarValue value, DvarLimits domain)
{
	bool result; // al
	bool v4; // zf
	int v5; // eax
	float v6; // xmm0_4
	int v7; // eax
	float v8; // xmm0_4
	int v9; // eax
	float v10; // xmm0_4
	const char* v11; // eax
	DvarValue v12; // [esp+0h] [ebp-14h]

	v12 = value;
	switch (type)
	{
	case 1:
		if (!v12.enabled
			&& v12.enabled
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(value.enabled == 1 || value.enabled == 0)",
				nullptr))
		{
			__debugbreak();
		}
		return 1;
	case 2:
		return domain.value.min <= v12.value && v12.value <= domain.value.max;
	case 3:
		v5 = 0;
		while (1)
		{
			v6 = *(&v12.value + v5);
			if (domain.value.min > v6 || v6 > domain.value.max)
				break;
			if (++v5 >= 2)
				return 1;
		}
		return 0;
	case 4:
	case 11:
	case 12:
		v7 = 0;
		while (1)
		{
			v8 = *(&v12.value + v7);
			if (domain.value.min > v8 || v8 > domain.value.max)
				break;
			if (++v7 >= 3)
				return 1;
		}
		return 0;
	case 5:
		v9 = 0;
		break;
	case 6:
		if (domain.enumeration.stringCount > domain.integer.max
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(domain.integer.min <= domain.integer.max)",
				nullptr))
		{
			__debugbreak();
		}
		if (v12.integer < domain.enumeration.stringCount)
			return 0;
		return v12.integer <= domain.integer.max;
	case 7:
		v4 = v12.integer == 0;
		if (v12.integer < 0)
			goto LABEL_24;
		if (v12.integer < domain.enumeration.stringCount)
			goto LABEL_47;
		v4 = v12.integer == 0;
	LABEL_24:
		if (v4)
			LABEL_47 :
			result = 1;
		else
			result = 0;
		return result;
	case 8:
	case 9:
		return 1;
	case 10:
		if (domain.integer64.min > domain.integer64.max
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(domain.integer64.min <= domain.integer64.max)",
				nullptr))
		{
			__debugbreak();
		}
		return v12.integer64 >= domain.integer64.min && v12.integer64 <= domain.integer64.max;
	default:
		v11 = va("unhandled dvar type '%i'", type);
		if (!Assert_MyHandler(__FILE__, __LINE__, 1, nullptr, v11))
			__debugbreak();
		return 0;
	}
	while (1)
	{
		v10 = *(&v12.value + v9);
		if (domain.value.min > v10 || v10 > domain.value.max)
			break;
		if (++v9 >= 4)
			return 1;
	}
	return 0;
}

void Dvar_VectorDomainToString(int components, char* outBuffer, int outBufferLen, DvarLimits domain)
{
	double v4; // xmm0_8

	if (domain.value.min == -3.4028235e38)
	{
		if (domain.value.max == 3.4028235e38)
			_snprintf(outBuffer, outBufferLen, "Domain is any %iD vector", components);
		else
			_snprintf(
				outBuffer,
				outBufferLen,
				"Domain is any %iD vector with components %g or smaller",
				components,
				domain.value.max);
	}
	else
	{
		v4 = domain.value.min;
		if (domain.value.max == 3.4028235e38)
			_snprintf(outBuffer, outBufferLen, "Domain is any %iD vector with components %g or bigger", components, v4);
		else
			_snprintf(
				outBuffer,
				outBufferLen,
				"Domain is any %iD vector with components from %g to %g",
				components,
				v4,
				domain.value.max);
	}
}

char* Dvar_DomainToString_Internal(char* outBuffer, int outBufferLen, dvarType_t type, DvarLimits domain, int* outLineCount)
{
	dvarType_t v5; // ebx
	int v6; // esi
	char* result; // eax
	double v8; // xmm0_8
	int v9; // eax
	int v10; // esi
	char* v11; // ebx
	int v12; // eax
	const char* v13; // eax
	char* outBufferEnd; // [esp+20h] [ebp+8h]

	v5 = type;
	v6 = outBufferLen;
	if (outBufferLen <= 0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(outBufferLen > 0)",
			nullptr))
	{
		__debugbreak();
	}
	outBufferEnd = &outBuffer[v6];
	if (outLineCount)
		*outLineCount = 0;
	switch (v5)
	{
	case 1:
		_snprintf(outBuffer, v6, "Domain is 0 or 1");
		*(outBufferEnd - 1) = 0;
		result = outBuffer;
		break;
	case 2:
		if (domain.value.min == -3.4028235e38)
		{
			if (domain.value.max == 3.4028235e38)
				_snprintf(outBuffer, v6, "Domain is any number");
			else
				_snprintf(outBuffer, v6, "Domain is any number %g or smaller", domain.value.max);
			*(outBufferEnd - 1) = 0;
			result = outBuffer;
		}
		else
		{
			v8 = domain.value.min;
			if (domain.value.max == 3.4028235e38)
				_snprintf(outBuffer, v6, "Domain is any number %g or bigger", v8);
			else
				_snprintf(outBuffer, v6, "Domain is any number from %g to %g", v8, domain.value.max);
			*(outBufferEnd - 1) = 0;
			result = outBuffer;
		}
		break;
	case 3:
		Dvar_VectorDomainToString(2, outBuffer, v6, domain);
		*(outBufferEnd - 1) = 0;
		result = outBuffer;
		break;
	case 4:
	case 11:
	case 12:
		Dvar_VectorDomainToString(3, outBuffer, v6, domain);
		*(outBufferEnd - 1) = 0;
		result = outBuffer;
		break;
	case 5:
		Dvar_VectorDomainToString(4, outBuffer, v6, domain);
		*(outBufferEnd - 1) = 0;
		result = outBuffer;
		break;
	case 6:
		if (domain.enumeration.stringCount == 0x80000000)
		{
			if (domain.integer.max == 0x7FFFFFFF)
				goto LABEL_15;
			_snprintf(outBuffer, v6, "Domain is any integer %i or smaller", domain.integer.max);
			*(outBufferEnd - 1) = 0;
			result = outBuffer;
		}
		else
		{
			if (domain.integer.max == 0x7FFFFFFF)
				_snprintf(outBuffer, v6, "Domain is any integer %i or bigger", domain.enumeration.stringCount);
			else
				_snprintf(
					outBuffer,
					v6,
					"Domain is any integer from %i to %i",
					domain.enumeration.stringCount,
					domain.integer.max);
			*(outBufferEnd - 1) = 0;
			result = outBuffer;
		}
		break;
	case 7:
		v9 = _snprintf(outBuffer, v6, "Domain is one of the following:");
		if (v9 < 0)
			goto LABEL_48;
		v10 = 0;
		v11 = &outBuffer[v9];
		if (domain.enumeration.stringCount <= 0)
			goto LABEL_48;
		do
		{
			v12 = _snprintf(v11, outBufferEnd - v11, "\n  %2i: %s", v10, *(const char**)(domain.integer.max + 4 * v10));
			if (v12 < 0)
				goto LABEL_48;
			if (outLineCount)
				++* outLineCount;
			++v10;
			v11 += v12;
		} while (v10 < domain.enumeration.stringCount);
		*(outBufferEnd - 1) = 0;
		result = outBuffer;
		break;
	case 8:
		_snprintf(outBuffer, v6, "Domain is any text");
		*(outBufferEnd - 1) = 0;
		result = outBuffer;
		break;
	case 9:
		_snprintf(outBuffer, v6, "Domain is any 4-component color, in RGBA format");
		*(outBufferEnd - 1) = 0;
		result = outBuffer;
		break;
	case 10:
		if (domain.enumeration.stringCount || domain.integer.max != 0x80000000)
		{
			if (domain.integer64.max == 0x7FFFFFFFFFFFFFFFi64)
				_snprintf(outBuffer, v6, "Domain is any integer %lli or bigger", domain.integer64.min);
			else
				_snprintf(
					outBuffer,
					v6,
					"Domain is any integer from %lli to %lli",
					domain.integer64.min,
					domain.integer64.max);
			*(outBufferEnd - 1) = 0;
			result = outBuffer;
		}
		else if (domain.integer64.max == 0x7FFFFFFFFFFFFFFFi64)
		{
		LABEL_15:
			_snprintf(outBuffer, v6, "Domain is any integer");
			*(outBufferEnd - 1) = 0;
			result = outBuffer;
		}
		else
		{
			_snprintf(outBuffer, v6, "Domain is any integer %lli or smaller", domain.integer64.max);
			*(outBufferEnd - 1) = 0;
			result = outBuffer;
		}
		break;
	default:
		v13 = va("unhandled dvar type '%i'", v5);
		if (!Assert_MyHandler(__FILE__, __LINE__, 1, nullptr, v13))
			__debugbreak();
		*outBuffer = 0;
	LABEL_48:
		*(outBufferEnd - 1) = 0;
		result = outBuffer;
		break;
	}
	return result;
}

char* Dvar_DomainToString_GetLines(dvarType_t type, DvarLimits domain, char* outBuffer, int outBufferLen, int* outLineCount)
{
	if (!outLineCount
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(outLineCount)",
			nullptr))
	{
		__debugbreak();
	}
	return Dvar_DomainToString_Internal(outBuffer, outBufferLen, type, domain, outLineCount);
}

void Dvar_PrintDomain(dvarType_t type, DvarLimits domain)
{
	char* domainString; // eax
	char domainBuffer[1024]; // [esp+0h] [ebp-404h]

	domainString = Dvar_DomainToString_Internal(domainBuffer, 1024, type, domain, 0);
	Com_Printf(10, "  %s\n", domainString);
}

bool Dvar_ValuesEqual(DvarValue val0, DvarValue val1)
{
	dvarType_t type; // ecx
	int result; // eax
	const char* v4; // eax
	DvarValue b; // [esp+0h] [ebp-24h]
	DvarValue a; // [esp+10h] [ebp-14h]

	a = val0;
	b = val1;
	switch (type)
	{
	case 1:
		result = a.enabled == b.enabled;
		break;
	case 2:
		if (a.value != b.value)
			goto LABEL_26;
		result = 1;
		break;
	case 3:
		if (a.value != b.value || a.vector.v[1] != b.vector.v[1])
			goto LABEL_26;
		result = 1;
		break;
	case 4:
	case 11:
	case 12:
		if (a.value != b.value || a.vector.v[1] != b.vector.v[1] || a.vector.v[2] != b.vector.v[2])
			goto LABEL_26;
		result = 1;
		break;
	case 5:
		result = Vec4Compare((const vec4_t*)&a, (const vec4_t*)&b);
		break;
	case 6:
	case 7:
		result = a.integer == b.integer;
		break;
	case 8:
		if (!a.integer
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(val0.string)",
				nullptr))
		{
			__debugbreak();
		}
		if (!b.integer
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(val1.string)",
				nullptr))
		{
			__debugbreak();
		}
		result = strcmp(a.string, b.string) == 0;
		break;
	case 9:
		result = a.integer == b.integer;
		break;
	case 10:
		if (a.integer64 != b.integer64)
			goto LABEL_26;
		result = 1;
		break;
	default:
		v4 = va("unhandled dvar type '%i'", type);
		if (!Assert_MyHandler(__FILE__, __LINE__, 1, nullptr, v4))
			__debugbreak();
	LABEL_26:
		result = 0;
		break;
	}
	return result;
}

void Dvar_SetLatchedValue(dvar_t*, DvarValue)
{
}

bool Dvar_HasLatchedValue(const dvar_t* dvar)
{
	return Dvar_ValuesEqual(dvar->current, dvar->latched) == 0;
}

dvarCallBack_t* findCallBackForDvar(const dvar_t* dvar)
{
	int v1; // ecx
	int v2; // esi
	dvarCallBack_t* result; // eax

	v1 = 0;
	if (s_nextFreeCallback <= 0)
		return 0;
	v2 = dvar->hash;
	for (result = s_dvarCallbackPool; result->dvar->hash != v2; ++result)
	{
		if (++v1 >= s_nextFreeCallback)
			return 0;
	}
	return result;
}

dvar_t* Dvar_FindMalleableVar(int dvarHash)
{
	dvar_t* v1; // esi
	dvar_t* result; // eax

	g_dvarCritSect.readCount += 1u;
	while (g_dvarCritSect.writeCount)
		NET_Sleep(0);
	v1 = s_dvarHashTable[dvarHash & 0x437];
	if (v1)
	{
		while (v1->hash != dvarHash)
		{
			v1 = v1->hashNext;
			if (!v1)
				goto LABEL_6;
		}
		if (g_dvarCritSect.readCount <= 0
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(critSect->readCount > 0)",
				nullptr))
		{
			__debugbreak();
		}
		g_dvarCritSect.readCount += 0xFFFFFFFF;
		result = v1;
	}
	else
	{
	LABEL_6:
		if (g_dvarCritSect.readCount <= 0
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(critSect->readCount > 0)",
				nullptr))
		{
			__debugbreak();
		}
		g_dvarCritSect.readCount += 0xFFFFFFFF;
		result = 0;
	}
	return result;
}

dvar_t* Dvar_FindMalleableVar(const char* dvarName)
{
	const char* v1; // edi
	char v3; // al
	int i; // esi

	v1 = dvarName;
	if (!dvarName)
		return Dvar_FindMalleableVar(0);
	v3 = *dvarName;
	for (i = 5381; *v1; v3 = *v1)
	{
		++v1;
		i = 33 * i + tolower(v3);
	}
	return Dvar_FindMalleableVar(i);
}

dvar_t* Dvar_FindVar(const char* dvarName)
{
	if (!dvarName)
	{
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvarName)",
			nullptr))
			__debugbreak();
		return 0;
	}
	if (!*dvarName)
		return 0;
	return Dvar_FindMalleableVar(dvarName);
}

dvar_t* Dvar_FindVar(int dvarHash)
{
	return Dvar_FindMalleableVar(dvarHash);
}

void Dvar_ClearModified(dvar_t* dvar)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	dvar->modified = 0;
}

void Dvar_SetModified(dvar_t* dvar)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	dvar->modified = 1;
}

bool Dvar_GetModified(const dvar_t* dvar)
{
	if (dvar)
		return dvar->modified;
	else
		return false;
}

int Dvar_GetInt(int dvarHash)
{
	dvar_t* dvar; // esi
	dvarType_t dvarType; // eax

	dvar = Dvar_FindMalleableVar(dvarHash);
	dvarType = dvar->type;
	if (dvar)
	{
		if (dvarType != 1
			&& dvarType != 6
			&& dvarType != 2
			&& dvarType != 7
			&& (dvarType != 8 || !(dvar->flags & 0x4000))
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"((dvar->type == DVAR_TYPE_BOOL || dvar->type == DVAR_TYPE_INT || dvar->type == DVAR_TYPE_FLOAT || dvar->type"
				" == DVAR_TYPE_ENUM || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14)))))",
				"(dvar->type) = %i",
				dvar->type))
		{
			__debugbreak();
		}
		switch (dvarType)
		{
		case 7:
			return dvar->current.integer;
		case 1:
			return dvar->current.enabled;
		case 2:
			return dvar->current.value;
		default:
			return Dvar_StringToInt(dvar->current.string);
		}
	}
}

unsigned int Dvar_GetUnsignedInt(const dvar_t* dvar)
{
	if (dvar)
		return dvar->current.unsignedInt;
	return 0;
}

float Dvar_GetFloat(int dvarHash)
{
	dvar_t* dvar;
	dvarType_t type;

	dvar = Dvar_FindMalleableVar(dvarHash);
	if (dvar)
	{
		type = dvar->type;
		if (type != 2
			&& type != 6
			&& (type != 8 || !(dvar->flags & 0x4000))
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"((dvar->type == DVAR_TYPE_FLOAT || dvar->type == DVAR_TYPE_INT || (dvar->type == DVAR_TYPE_STRING && (dvar->"
				"flags & (1 << 14)))))",
				"(dvar->type) = %i",
				dvar->type))
		{
			__debugbreak();
		}
	}
}

void Dvar_GetVec2(const dvar_t* dvar, vec2_t* result)
{
	if (dvar)
	{
		if (dvar->type != 3
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(dvar->type == DVAR_TYPE_FLOAT_2)",
				nullptr))
		{
			__debugbreak();
		}
		*result = dvar->current.integer64;
	}
	else
	{
		*result = vec2_origin;
	}
}

void Dvar_GetVec3(const dvar_t* dvar, vec3_t* result)
{
	dvarType_t type; // eax

	if (dvar)
	{
		type = dvar->type;
		if (type != 4
			&& type != 11
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(dvar->type == DVAR_TYPE_FLOAT_3 || dvar->type == DVAR_TYPE_LINEAR_COLOR_RGB)",
				nullptr))
		{
			__debugbreak();
		}
		result->x = dvar->current.value;
		result->y = dvar->current.vector.v[1];
		result->z = dvar->current.vector.v[2];
	}
	else
	{
		*result = vec3_origin;
	}
}

void Dvar_GetVec4(const dvar_t* dvar, vec4_t* result)
{
	if (dvar)
	{
		if (dvar->type != 5
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(dvar->type == DVAR_TYPE_FLOAT_4)",
				nullptr))
		{
			__debugbreak();
		}
		*(DvarValue*)result = dvar->current;
	}
	else
	{
		*result = vec4_origin;
	}
}

char* Dvar_GetString(const dvar_t* dvar)
{
	char* result; // eax
	dvarType_t v2; // eax

	if (!dvar)
		return nullptr;
	v2 = dvar->type;
	if (v2 != 8
		&& v2 != 7
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((dvar->type == DVAR_TYPE_STRING || dvar->type == DVAR_TYPE_ENUM))",
			"(dvar->type) = %i",
			dvar->type))
	{
		__debugbreak();
	}
	if (dvar->type == 7)
		result = Dvar_EnumToString(dvar);
	else
		result = (char*)dvar->current.integer;
	return result;
}

char* Dvar_GetVariantString(int dvarHash)
{
	dvar_t* v1; // eax
	char* result; // eax

	v1 = Dvar_FindMalleableVar(dvarHash);
	if (v1)
		result = Dvar_ValueToString(v1, v1->current);
	else
		result = nullptr;
	return result;
}

char* Dvar_GetVariantString(const dvar_t* dvar)
{
	char* result; // eax

	if (dvar)
		result = Dvar_ValueToString(dvar, dvar->current);
	else
		result = nullptr;
	return result;
}

void Dvar_GetUnpackedColor(const dvar_t* dvar, vec4_t* expandedColor)
{
	bool v2; // zf
	const char* v3; // eax
	int v4; // ecx
	int v5; // edx
	int v6; // eax
	char color[4]; // [esp+8h] [ebp-4h]

	v2 = dvar->type == 9;
	v3 = dvar->current.string;
	if (v2)
	{
		*color = (char)v3;
	}
	else
	{
		Dvar_StringToColor(color, v3);
		v3 = (const char*)color;
	}
	v4 = (unsigned __int8)color[3];
	expandedColor->v[0] = (float)(unsigned __int8)v3 * 0.0039215689;
	v5 = BYTE1(v3);
	v6 = (unsigned __int8)color[2];
	expandedColor->v[1] = (float)v5 * 0.0039215689;
	expandedColor->v[2] = (float)v6 * 0.0039215689;
	expandedColor->v[3] = (float)v4 * 0.0039215689;
}

void Dvar_GetColor(const dvar_t* dvar, char* color)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	if (dvar->type == 9)
	{
		*color = dvar->current.integer;
	}
	else
	{
		if (!Assert_MyHandler(
			__FILE__,
			1903,
			0,
			"((dvar->type == DVAR_TYPE_COLOR))",
			"(dvar->type) = %i",
			dvar->type))
			__debugbreak();
		*color = dvar->current.integer;
	}
}

float Dvar_GetColorRed(const dvar_t* dvar)
{
	vec4_t expandedColor;

	Dvar_GetUnpackedColor(dvar, &expandedColor);
	return expandedColor.v[0];
}

float Dvar_GetColorRed(int dvarHash)
{
	const dvar_t* v1; // eax
	vec4_t expandedColor; // [esp+4h] [ebp-14h]

	v1 = Dvar_FindMalleableVar(dvarHash);
	if (!v1)
		return 1.0;
	Dvar_GetUnpackedColor(v1, &expandedColor);
	return expandedColor.v[0];
}

float Dvar_GetColorGreen(const dvar_t* dvar)
{
	vec4_t expandedColor; // [esp+0h] [ebp-14h]

	Dvar_GetUnpackedColor(dvar, &expandedColor);
	return expandedColor.v[1];
}

float Dvar_GetColorGreen(int dvarHash)
{
	const dvar_t* v1; // eax
	vec4_t expandedColor; // [esp+4h] [ebp-14h]

	v1 = Dvar_FindMalleableVar(dvarHash);
	if (!v1)
		return 1.0;
	Dvar_GetUnpackedColor(v1, &expandedColor);
	return expandedColor.v[1];
}

float Dvar_GetColorBlue(const dvar_t* dvar)
{
	vec4_t expandedColor; // [esp+0h] [ebp-14h]

	Dvar_GetUnpackedColor(dvar, &expandedColor);
	return expandedColor.v[2];
}

float Dvar_GetColorBlue(int dvarHash)
{
	const dvar_t* v1; // eax
	vec4_t expandedColor; // [esp+4h] [ebp-14h]

	v1 = Dvar_FindMalleableVar(dvarHash);
	if (!v1)
		return 1.0;
	Dvar_GetUnpackedColor(v1, &expandedColor);
	return expandedColor.v[2];
}

float Dvar_GetColorAlpha(const dvar_t* dvar)
{
	vec4_t expandedColor; // [esp+0h] [ebp-14h]

	Dvar_GetUnpackedColor(dvar, &expandedColor);
	return expandedColor.v[3];
}

float Dvar_GetColorAlpha(int dvarHash)
{
	const dvar_t* v1; // eax
	vec4_t expandedColor; // [esp+4h] [ebp-14h]

	v1 = Dvar_FindMalleableVar(dvarHash);
	if (!v1)
		return 1.0;
	Dvar_GetUnpackedColor(v1, &expandedColor);
	return expandedColor.v[3];
}

bool Dvar_GetLatchedBool(const dvar_t* dvar)
{
	if (!dvar)
		return 0;
	if (dvar->type != 1
		&& !Assert_MyHandler(
			__FILE__,
			1974,
			0,
			"((dvar->type == DVAR_TYPE_BOOL))",
			"(dvar->type) = %i",
			dvar->type))
	{
		__debugbreak();
	}
	return dvar->latched.enabled;
}

int Dvar_GetLatchedInt(const dvar_t* dvar)
{
	dvarType_t v2; // eax

	if (!dvar)
		return 0;
	v2 = dvar->type;
	if (v2 != 6
		&& v2 != 7
		&& !Assert_MyHandler(
			__FILE__,
			1987,
			0,
			"((dvar->type == DVAR_TYPE_INT || dvar->type == DVAR_TYPE_ENUM))",
			"(dvar->type) = %i",
			dvar->type))
	{
		__debugbreak();
	}
	return dvar->latched.integer;
}

float Dvar_GetLatchedFloat(const dvar_t* dvar)
{
	if (!dvar)
		return 0.0;
	if (dvar->type != 2
		&& !Assert_MyHandler(
			__FILE__,
			2013,
			0,
			"((dvar->type == DVAR_TYPE_FLOAT))",
			"(dvar->type) = %i",
			dvar->type))
	{
		__debugbreak();
	}
	return dvar->latched.value;
}

void Dvar_GetLatchedVec2(const dvar_t* dvar, vec2_t* result)
{
	if (dvar)
	{
		if (dvar->type != 3
			&& !Assert_MyHandler(
				__FILE__,
				2029,
				0,
				"(dvar->type == DVAR_TYPE_FLOAT_2)",
				nullptr))
		{
			__debugbreak();
		}
		*result = dvar->latched.integer64;
	}
	else
	{
		*result = vec2_origin;
	}
}

void Dvar_GetLatchedVec3(const dvar_t* dvar, vec3_t* result)
{
	dvarType_t v2; // eax

	if (dvar)
	{
		v2 = dvar->type;
		if (v2 != 4
			&& v2 != 11
			&& v2 != 12
			&& !Assert_MyHandler(
				__FILE__,
				2044,
				0,
				"(dvar->type == DVAR_TYPE_FLOAT_3 || dvar->type == DVAR_TYPE_LINEAR_COLOR_RGB || dvar->type == DVAR_TYPE_COLOR_XYZ)",
				nullptr))
		{
			__debugbreak();
		}
		result->x = dvar->latched.value;
		result->y = dvar->latched.vector.v[1];
		result->z = dvar->latched.vector.v[2];
	}
	else
	{
		*result = vec3_origin;
	}
}

void Dvar_GetLatchedVec4(const dvar_t* dvar, vec4_t* result)
{
	if (dvar)
	{
		if (dvar->type != 5
			&& !Assert_MyHandler(
				__FILE__,
				2059,
				0,
				"(dvar->type == DVAR_TYPE_FLOAT_4)",
				nullptr))
		{
			__debugbreak();
		}
		*(DvarValue*)result = dvar->latched;
	}
	else
	{
		*result = vec4_origin;
	}
}

void Dvar_GetLatchedColor(const dvar_t* dvar, char* color)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	if (dvar->type == 9)
	{
		*color = dvar->latched.integer;
	}
	else
	{
		if (!Assert_MyHandler(
			__FILE__,
			2067,
			0,
			"((dvar->type == DVAR_TYPE_COLOR))",
			"(dvar->type) = %i",
			dvar->type))
			__debugbreak();
		*color = dvar->latched.integer;
	}
}

int Dvar_GetResetInt(const dvar_t* dvar)
{
	dvarType_t v2; // eax

	if (!dvar)
		return 0;
	v2 = dvar->type;
	if (v2 != 6
		&& v2 != 7
		&& !Assert_MyHandler(
			__FILE__,
			2080,
			0,
			"((dvar->type == DVAR_TYPE_INT || dvar->type == DVAR_TYPE_ENUM))",
			"(dvar->type) = %i",
			dvar->type))
	{
		__debugbreak();
	}
	return dvar->reset.integer;
}

const char* Dvar_GetResetString(const dvar_t* dvar)
{
	if (!dvar)
		return nullptr;
	if (dvar->type != 8
		&& !Assert_MyHandler(
			__FILE__,
			2106,
			0,
			"((dvar->type == DVAR_TYPE_STRING))",
			"(dvar->type) = %i",
			dvar->type))
	{
		__debugbreak();
	}
	return dvar->reset.string;
}

void Dvar_GetResetVec3(dvar_t const*, vec3_t&)
{
}

char** Dvar_GetDomainEnumStrings(dvar_t const*)
{
	return nullptr;
}

int Dvar_GetDomainEnumStringCount(dvar_t const*)
{
	return 0;
}

int Dvar_GetDomainIntMin(dvar_t const*)
{
	return 0;
}

int Dvar_GetDomainIntMax(dvar_t const*)
{
	return 0;
}

__int64 Dvar_GetDomainInt64Min(dvar_t const*)
{
	return 0;
}

__int64 Dvar_GetDomainInt64Max(dvar_t const*)
{
	return 0;
}

float Dvar_GetDomainFloatMin(dvar_t const*)
{
	return 0.0f;
}

float Dvar_GetDomainFloatMax(dvar_t const*)
{
	return 0.0f;
}

float Dvar_GetDomainVecMin(dvar_t const*)
{
	return 0.0f;
}

float Dvar_GetDomainVecMax(dvar_t const*)
{
	return 0.0f;
}

dvarType_t Dvar_GetType(dvar_t const* dvar)
{
	if (dvar)
		return dvar->type;
	else
		return DVAR_TYPE_COUNT;
}

DvarValue Dvar_GetCurrent(dvar_t const* dvar)
{
	DvarValue* newValue;

	if (dvar)
		return dvar->current;
	else
		return *newValue;
}

DvarLimits Dvar_GetDomain(dvar_t const* dvar)
{
	DvarLimits newLimit;

	if (dvar)
		return dvar->domain;
	else
		return newLimit;
}

const char* Dvar_GetDescription(dvar_t const* dvar)
{
	if (dvar)
		return dvar->description;
	else
		return nullptr;
}

unsigned int Dvar_GetFlags(dvar_t const* dvar)
{
	if (dvar)
		return dvar->flags;
	else
		return 0x84000u; // default flag
}

const char* Dvar_GetName(dvar_t const* dvar)
{
	if (dvar)
		return dvar->name;
	else
		return nullptr;
}

void Dvar_Shutdown(void)
{
}

void Dvar_PerformUnregistration(dvar_t* dvar)
{
	unsigned int v2; // eax
	char* v3; // esi
	const char* v4; // eax
	const char* v5; // ecx
	const char* v6; // esi
	const char* v7; // eax
	char* v8; // eax
	DvarValue resetString; // [esp+8h] [ebp-10h]

	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	v2 = dvar->flags;
	if (!(v2 & 0x4000))
	{
		dvar->flags = v2 | 0x4000;
		dvar->name = CopyString(dvar->name);
	}
	if (dvar->type != 8)
	{
		v3 = Dvar_DisplayableLatchedValue(dvar);
		if (!v3
			&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
		{
			__debugbreak();
		}
		v4 = CopyString(v3);
		v5 = dvar->latched.string;
		dvar->current.integer = (int)v4;
		if (v5 && v5 != v4 && v5 != dvar->reset.string)
		{
			FreeString(v5);
			dvar->latched.integer = 0;
		}
		v6 = dvar->current.string;
		dvar->latched.integer = 0;
		if (!v6
			&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", nullptr))
		{
			__debugbreak();
		}
		v7 = dvar->reset.string;
		dvar->latched.integer = (int)v6;
		if (v7 && v7 != dvar->current.string && v7 != v6)
		{
			FreeString(v7);
			dvar->reset.integer = 0;
		}
		dvar->reset.integer = 0;
		v8 = Dvar_DisplayableResetValue(dvar);
		Dvar_AssignResetStringValue(dvar, &resetString, v8);
		dvar->reset.integer = resetString.integer;
		dvar->type = DVAR_TYPE_STRING;
	}
}

void Dvar_UpdateResetValue(dvar_t* dvar, DvarValue value)
{
	const char* v3; // edx
	bool v4; // bl
	const char* oldString; // [esp+0h] [ebp-24h]
	DvarValue resetString; // [esp+10h] [ebp-14h]

	resetString = value;
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	switch (dvar->type)
	{
	case DVAR_TYPE_FLOAT_2:
		dvar->reset.integer64 = resetString.integer64;
		break;
	case DVAR_TYPE_FLOAT_3:
	case DVAR_TYPE_LINEAR_COLOR_RGB:
	case DVAR_TYPE_COLOR_XYZ:
		dvar->reset.integer64 = resetString.integer64;
		dvar->reset.vector.v[2] = resetString.vector.v[2];
		break;
	case DVAR_TYPE_FLOAT_4:
		dvar->reset = resetString;
		break;
	case DVAR_TYPE_STRING:
		if (dvar->reset.integer != resetString.integer)
		{
			v4 = Dvar_ShouldFreeResetString(dvar);
			if (v4)
				oldString = v3;
			Dvar_AssignResetStringValue(dvar, &resetString, resetString.string);
			dvar->reset.integer = resetString.integer;
			if (v4)
				FreeString(oldString);
		}
		break;
	default:
		dvar->reset = resetString;
		break;
	}
}

void Dvar_ChangeResetValue(dvar_t const* dvar, DvarValue value)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	if (!dvar->name
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvar->name)",
			nullptr))
	{
		__debugbreak();
	}
	if (!(dvar->flags & 0x200)
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((dvar->flags & (1 << 9)))",
			"(dvar->name) = %s",
			dvar->name))
	{
		__debugbreak();
	}
	Dvar_UpdateResetValue((dvar_t*)dvar, value);
}

void Dvar_UpdateValue(dvar_t* dvar, DvarValue value)
{
	const char* v2; // edx
	const char* v3; // esi
	int v4; // xmm0_4
	float v5; // xmm1_4
	int v6; // xmm0_4
	float v7; // xmm1_4
	float v8; // xmm2_4
	int v9; // xmm0_4
	float v10; // xmm1_4
	float v11; // xmm2_4
	float v12; // xmm3_4
	__int64 v13; // xmm0_8
	const char* v14; // xmm1_4
	const char* oldString; // [esp+0h] [ebp-28h]
	bool shouldFree; // [esp+13h] [ebp-15h]
	DvarValue currentString; // [esp+14h] [ebp-14h]

	currentString = value;
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	switch (dvar->type)
	{
	case 3:
		v4 = currentString.integer;
		v5 = currentString.vector.v[1];
		dvar->current.integer = currentString.integer;
		dvar->current.vector.v[1] = v5;
		dvar->latched.integer = v4;
		dvar->latched.vector.v[1] = v5;
		break;
	case 4:
	case 0xB:
	case 0xC:
		v6 = currentString.integer;
		v7 = currentString.vector.v[1];
		v8 = currentString.vector.v[2];
		dvar->current.integer = currentString.integer;
		dvar->current.vector.v[1] = v7;
		dvar->current.vector.v[2] = v8;
		dvar->latched.integer = v6;
		dvar->latched.vector.v[1] = v7;
		dvar->latched.vector.v[2] = v8;
		break;
	case 5:
		v9 = currentString.integer;
		v10 = currentString.vector.v[1];
		v11 = currentString.vector.v[2];
		v12 = currentString.vector.v[3];
		dvar->current.integer = currentString.integer;
		dvar->current.vector.v[1] = v10;
		dvar->current.vector.v[2] = v11;
		dvar->current.vector.v[3] = v12;
		dvar->latched.integer = v9;
		dvar->latched.vector.v[1] = v10;
		dvar->latched.vector.v[2] = v11;
		dvar->latched.vector.v[3] = v12;
		break;
	case 8:
		if (currentString.integer != dvar->current.integer)
		{
			shouldFree = Dvar_ShouldFreeCurrentString(dvar);
			if (shouldFree)
				oldString = currentString.string;
			Dvar_AssignCurrentStringValue(dvar, &currentString, currentString.string);
			dvar->current.integer = currentString.integer;
			if (Dvar_ShouldFreeLatchedString(dvar))
				Dvar_FreeString(&dvar->latched);
			v3 = dvar->current.string;
			dvar->latched.integer = 0;
			Dvar_WeakCopyString(v3, &dvar->latched);
			if (shouldFree)
				FreeString(oldString);
		}
		break;
	default:
		v13 = currentString.integer64;
		v14 = currentString.string;
		dvar->current.integer64 = currentString.integer64;
		dvar->latched.integer64 = v13;
		dvar->current.string = v14;
		dvar->latched.string = v14;
		break;
	}
}

void Dvar_MakeExplicitType(dvar_t* dvar, const char* dvarName, dvarType_t type, unsigned __int16 flags, DvarValue resetValue, DvarLimits domain)
{
	bool v6; // [esp+0h] [ebp-64h]
	DvarValue v7; // [esp+4h] [ebp-60h]
	DvarValue v8; // [esp+14h] [ebp-50h]
	DvarValue result; // [esp+24h] [ebp-40h]
	DvarValue v10; // [esp+34h] [ebp-30h]
	bool wasString; // [esp+4Bh] [ebp-19h]
	DvarValue castValue; // [esp+4Ch] [ebp-18h]

	if (dvar->type != 7
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->type) = %i",
			"(dvar->type == DVAR_TYPE_STRING)",
			dvar->type))
	{
		__debugbreak();
	}
	dvar->type = type;
	dvar->domain = domain;
	if (flags & 0x40 || flags & 0x80 && dvar_cheats && !dvar_cheats->current.enabled)
	{
		castValue = resetValue;
	}
	else
	{
		v10 = *Dvar_StringToValue(dvar->type, dvar->current.string, dvar->domain);
		castValue = v10;
		v8 = Dvar_ClampValueToDomain(type, v10, resetValue, domain);
		castValue = v8;
	}
	v6 = dvar->type == 7 && castValue.integer;
	wasString = v6;
	if (v6)
		castValue.integer = (int)CopyString(castValue.string);
	if (dvar->type != 7 && Dvar_ShouldFreeCurrentString(dvar))
		Dvar_FreeString(&dvar->current);
	dvar->current.integer = 0;
	if (Dvar_ShouldFreeLatchedString(dvar))
		Dvar_FreeString(&dvar->latched);
	dvar->latched.integer = 0;
	if (Dvar_ShouldFreeResetString(dvar))
		Dvar_FreeString(&dvar->reset);
	dvar->reset.integer = 0;
	Dvar_UpdateResetValue(dvar, resetValue);
	Dvar_UpdateValue(dvar, castValue);
	g_dvar_modifiedFlags |= flags;
	if (wasString)
		FreeString(castValue.string);
}

void Dvar_ReinterpretDvar(dvar_t* dvar, unsigned int flags, const char* dvarName, dvarType_t type, DvarValue value, DvarLimits domain)
{
	DvarValue result; // [esp+0h] [ebp-38h]
	DvarValue v7; // [esp+10h] [ebp-28h]
	DvarValue resetValue; // [esp+20h] [ebp-18h]

	if (dvar->flags & 0x4000)
	{
		if (!(flags & 0x4000))
		{
			resetValue = value;
			Dvar_PerformUnregistration(dvar);
			FreeString(dvar->name);
			dvar->name = dvarName;
			dvar->flags &= 0xFFFFBFFF;
			Dvar_MakeExplicitType(dvar, dvarName, type, flags, resetValue, domain);
		}
	}
}

dvar_t* Dvar_RegisterNew(const char* dvarName, dvarType_t type, unsigned int flags, DvarValue value, DvarLimits domain, const char* description)
{
	const char* v6; // edi
	dvarType_t v7; // esi
	int v8; // eax
	dvar_t* v9; // ebx
	int v10; // xmm0_4
	float v11; // xmm1_4
	int v12; // xmm0_4
	float v13; // xmm1_4
	float v14; // xmm2_4
	int v15; // xmm0_4
	float v16; // xmm1_4
	float v17; // xmm2_4
	float v18; // xmm3_4
	__int64 v19; // xmm1_8
	__int64 v20; // xmm0_8
	int v21; // eax
	char v22; // al
	int v23; // esi
	int v24; // eax
	dvar_t* v25; // esi
	dvar_t* v26; // eax
	bool v27; // zf
	const char* v28; // eax
	volatile int v29; // edx
	dvar_t* v31; // [esp+0h] [ebp-1Ch]
	const char* hash; // [esp+4h] [ebp-18h]
	int hasha; // [esp+4h] [ebp-18h]
	DvarValue string; // [esp+8h] [ebp-14h]

	v6 = dvarName;
	string = value;
	v7 = type;
	hash = dvarName;
	Sys_LockWrite(&g_dvarCritSect);
	if (!s_isDvarSystemActive
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(s_isDvarSystemActive)", nullptr))
	{
		__debugbreak();
	}
	v8 = g_dvarCount;
	if (g_dvarCount >= 4320)
	{
		if (g_dvarCritSect.writeCount <= 0
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(critSect->writeCount > 0)",
				nullptr))
		{
			__debugbreak();
		}
		g_dvarCritSect.writeCount += 0xFFFFFFFF;
		Com_Error(ERR_FATAL, "Can't create dvar '%s': %i dvars already exist", v6, 4320);
		v8 = g_dvarCount;
	}
	v9 = &s_dvarPool[v8];
	s_sortedDvars[v8] = v9;
	s_areDvarsSorted = 0;
	g_dvarCount = v8 + 1;
	v9->type = v7;
	if (flags & 0x4000)
		v9->name = CopyString(v6);
	else
		v9->name = v6;
	switch (v7)
	{
	case 3:
		v10 = string.integer;
		v11 = string.vector.v[1];
		v9->current.integer = string.integer;
		v9->current.vector.v[1] = v11;
		v9->latched.integer = v10;
		v9->latched.vector.v[1] = v11;
		v9->reset.integer = v10;
		v9->reset.vector.v[1] = v11;
		break;
	case 4:
	case 11:
	case 12:
		v12 = string.integer;
		v13 = string.vector.v[1];
		v14 = string.vector.v[2];
		v9->current.integer = string.integer;
		v9->current.vector.v[1] = v13;
		v9->current.vector.v[2] = v14;
		v9->latched.integer = v12;
		v9->latched.vector.v[1] = v13;
		v9->latched.vector.v[2] = v14;
		v9->reset.integer = v12;
		v9->reset.vector.v[1] = v13;
		v9->reset.vector.v[2] = v14;
		break;
	case 5:
		v15 = string.integer;
		v16 = string.vector.v[1];
		v17 = string.vector.v[2];
		v18 = string.vector.v[3];
		v9->current.integer = string.integer;
		v9->current.vector.v[1] = v16;
		v9->current.vector.v[2] = v17;
		v9->current.vector.v[3] = v18;
		v9->latched.integer = v15;
		v9->latched.vector.v[1] = v16;
		v9->latched.vector.v[2] = v17;
		v9->latched.vector.v[3] = v18;
		v9->reset.integer = v15;
		v9->reset.vector.v[1] = v16;
		v9->reset.vector.v[2] = v17;
		v9->reset.vector.v[3] = v18;
		break;
	case 8:
		Dvar_CopyString(string.string, &v9->current);
		Dvar_WeakCopyString(v9->current.string, &v9->latched);
		Dvar_WeakCopyString(v9->current.string, &v9->reset);
		v6 = hash;
		break;
	default:
		v19 = string.integer64;
		v20 = *((__int64 *)&string.string + 1);
		v9->current.integer64 = string.integer64;
		v9->latched.integer64 = v19;
		v9->reset.integer64 = v19;
		*((__int64*)&v9->current.string + 1) = v20;
		*((__int64*)&v9->latched.string + 1) = v20;
		*((__int64*)&v9->reset.string + 1) = v20;
		break;
	}
	v9->domain = domain;
	v9->modified = 0;
	v9->flags = flags;
	v9->description = description;
	if (v6)
	{
		v22 = *v6;
		v23 = 5381;
		if (*v6)
		{
			do
			{
				++v6;
				v23 = 33 * v23 + tolower(v22);
				v22 = *v6;
			} while (*v6);
			v6 = hash;
		}
		hasha = v23;
		v21 = v23;
	}
	else
	{
		v21 = 0;
		hasha = 0;
	}
	v9->hash = v21;
	v24 = v21 & 0x437;
	v25 = s_dvarHashTable[v24];
	v26 = s_dvarHashTable[v24];
	v31 = v26;
	v27 = v25 == 0;
	if (v25)
	{
		do
		{
			v28 = nullptr;
			if (!v27)
				v28 = v25->name;
			if (I_stricmp(v6, v28) && hasha == v25->hash)
				Com_Error(
					ERR_FATAL,
					"dvar name hash collision between '%s' and '%s' Please change one of these names to remove the hash collision\n",
					v6,
					v25->name);
			v25 = v25->hashNext;
			v27 = v25 == 0;
		} while (v25);
		v26 = v31;
	}
	v29 = g_dvarCritSect.writeCount;
	v9->hashNext = v26;
	v26 = v9;
	if (v29 <= 0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(critSect->writeCount > 0)",
			nullptr))
	{
		__debugbreak();
	}
	g_dvarCritSect.writeCount += 0xFFFFFFFF;
	return v9;
}

void Dvar_AddFlags(dvar_t* dvar, int flags)
{
	int v2; // edi
	int v3; // edi

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
		__debugbreak();
	v2 = 0x240F0;
	if (flags & 0x20000 && s_canSetConfigDvars)
		v2 = 0x40F0;
	v3 = flags & v2;
	if (v3
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(((flags & unaddableFlags) == 0))",
			"(flags) = %i",
			flags))
	{
		__debugbreak();
	}
	if (!v3)
		dvar->flags |= flags;
}

int Com_SaveDvarsToBuffer(const char** dvarnames, unsigned int numDvars, char* buffer, unsigned int bufsize)
{
	int result; // eax
	unsigned int v5; // ebx
	char* v6; // edi
	const char* v7; // eax
	dvar_t* v8; // esi
	char* v9; // eax
	int v10; // eax
	unsigned int i; // [esp+Ch] [ebp-4h]

	result = 1;
	i = 0;
	if (numDvars)
	{
		v5 = bufsize;
		v6 = buffer;
		while (1)
		{
			v7 = dvarnames[i];
			if (v7)
			{
				if (*v7)
				{
					v8 = Dvar_FindMalleableVar(v7);
					if (v8)
						goto LABEL_13;
					goto LABEL_9;
				}
			}
			else if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(dvarName)", nullptr))
			{
				__debugbreak();
			}
			v8 = 0;
		LABEL_9:
			if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
				__debugbreak();
			if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
				__debugbreak();
		LABEL_13:
			v9 = Dvar_ValueToString(v8, v8->current);
			v10 = _snprintf(v6, v5, "%s \"%s\"\n", v8->name, v9);
			if (v10 < 0)
				return 0;
			v6 += v10;
			v5 -= v10;
			if (++i >= numDvars)
				return 1;
		}
	}
	return result;
}

void Dvar_SetModifiedCallback(dvar_t* dvar, void(*callback)(dvar_t const*))
{
	int v2; // eax
	int v3; // ecx
	dvarCallBack_t* v4; // esi
	int v5; // esi
	int v6; // eax

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "(0 != dvar)", nullptr))
		__debugbreak();
	v2 = s_nextFreeCallback;
	v3 = 0;
	if (s_nextFreeCallback <= 0)
	{
	LABEL_8:
		if (s_nextFreeCallback == 64)
		{
			Com_Error(ERR_FATAL, "Ran out of dvar callbacks, cap is %i", 64);
			v2 = s_nextFreeCallback;
		}
		v5 = 3 * v2;
		v6 = v2 + 1;
		v4 = s_dvarCallbackPool[v5];
		s_nextFreeCallback = v6;
	}
	else
	{
		v4 = *s_dvarCallbackPool;
		while (v4->dvar->hash != dvar->hash)
		{
			++v3;
			++v4;
			if (v3 >= s_nextFreeCallback)
				goto LABEL_8;
		}
		Com_PrintWarning(10, "Updating callback for dvar %s, check if this is really necessary\n", dvar->name);
		v6 = s_nextFreeCallback;
	}
	dvar->flags |= 0x40000u;
	v4->callback = callback;
	v4->needsCallback = 0;
	v4->dvar = dvar;
	Com_DPrintf(10, "Registered callback for %s, %i free callbacks remaining\n", dvar->name, 64 - v6);
}

void Dvar_SetCanSetConfigDvars(bool canSetConfigDvars)
{
	s_canSetConfigDvars = canSetConfigDvars;
}

bool Dvar_CanSetConfigDvar(dvar_t const* dvar)
{
	if (dvar)
	{
		if (dvar->flags & 0x20000 && Sys_IsMainThread())
			return s_canSetConfigDvars;
		else
			return true;
	}
	else
	{
		if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
			__debugbreak();
		return false;
	}
}

bool Dvar_CanChangeValue(dvar_t const* dvar, DvarValue value, DvarSetSource source)
{
	const dvar_t* v3; // esi
	unsigned int v5; // eax
	const char* v6; // eax

	v3 = dvar;
	if (!dvar)
	{
		if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
			__debugbreak();
		return false;
	}
	if (Dvar_ValuesEqual(value, dvar->reset))
		return true;
	v5 = v3->flags;
	if (v5 & 0x40)
	{
		v6 = va("%s is read only.\n", v3->name);
	}
	else if (v5 & 0x10)
	{
		v6 = va("%s is write protected.\n", v3->name);
	}
	else
	{
		if ((v5 & 0x80u) == 0 || dvar_cheats->current.enabled || source != 1 && source != 2)
			return 1;
		v6 = va("%s is cheat protected.\n", v3->name);
	}
	if (!v6)
		return true;
	Com_Printf(1, "%s", v6);
	return false;
}

void Dvar_SetVariant(dvar_t* dvar, DvarValue value, DvarSetSource source)
{
	const char* v3; // eax
	const char* v4; // edi
	char* v5; // eax
	char* v6; // eax
	const char* v7; // esi
	int v8; // xmm0_4
	float v9; // xmm1_4
	int v10; // xmm0_4
	float v11; // xmm1_4
	float v12; // xmm2_4
	int v13; // xmm0_4
	float v14; // xmm1_4
	float v15; // xmm2_4
	float v16; // xmm3_4
	__int64 v17; // xmm0_8
	__int64 v18; // xmm1_8
	bool v19; // zf
	dvarCallBack_t* v20; // esi
	const char* v21; // eax
	const char* v22; // [esp-10h] [ebp-438h]
	const char* oldString; // [esp+0h] [ebp-428h]
	bool shouldFreeString; // [esp+13h] [ebp-415h]
	DvarValue currentString; // [esp+14h] [ebp-414h]
	char string[1024]; // [esp+24h] [ebp-404h]

	currentString = value;
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	v3 = dvar->name;
	if (dvar->name)
		goto LABEL_57;
	if (!Assert_MyHandler(
		__FILE__,
		__LINE__,
		0,
		"(dvar->name)",
		nullptr))
		__debugbreak();
	v3 = dvar->name;
	if (dvar->name)
	{
	LABEL_57:
		if (*v3)
		{
			if (!Dvar_CanSetConfigDvar(dvar))
				Com_PrintError(
					10,
					"Setting config Dvar %s outside of config block - Will not happen in ship builds\n",
					dvar->name);
			Dvar_ValuesEqual(dvar->current, currentString);
			if (Com_LogFileOpen() && !Dvar_ValuesEqual(dvar->current, currentString))
			{
				v4 = dvar->name;
				v5 = Dvar_ValueToString(dvar, currentString);
				Com_sprintf(string, 1024, "      dvar set %s %s\n", v4, v5);
				Com_PrintMessage(8, string, 0);
			}
			if (!Dvar_ValueInDomain(dvar->type, currentString, dvar->domain))
			{
				v22 = dvar->name;
				v6 = Dvar_ValueToString(dvar, currentString);
				Com_Printf(1, "'%s' is not a valid value for dvar '%s'\n", v6, v22);
				Dvar_PrintDomain(dvar->type, dvar->domain);
				if (dvar->type == 7)
				{
					if (!Dvar_ValueInDomain(DVAR_TYPE_ENUM, dvar->reset, dvar->domain)
						&& !Assert_MyHandler(
							__FILE__,
							__LINE__,
							0,
							"((Dvar_ValueInDomain( dvar->type, dvar->reset, dvar->domain )))",
							"(dvar->name) = %s",
							dvar->name))
					{
						__debugbreak();
					}
					Dvar_SetVariant(dvar, dvar->reset, source);
				}
				return;
			}
			if (source != 1 && source != 2)
			{
				if (source == 3 && dvar->flags & 0x800)
				{
					Dvar_SetLatchedValue(dvar, currentString);
					return;
				}
			LABEL_29:
				if (Dvar_ValuesEqual(dvar->current, currentString))
				{
					Dvar_SetLatchedValue(dvar, dvar->current);
				}
				else
				{
					g_dvar_modifiedFlags |= dvar->flags;
					switch (dvar->type)
					{
					case 3:
						v8 = currentString.integer;
						v9 = currentString.vector.v[1];
						dvar->current.integer = currentString.integer;
						dvar->current.vector.v[1] = v9;
						dvar->latched.integer = v8;
						dvar->latched.vector.v[1] = v9;
						break;
					case 4:
					case 0xB:
					case 0xC:
						v10 = currentString.integer;
						v11 = currentString.vector.v[1];
						v12 = currentString.vector.v[2];
						dvar->current.integer = currentString.integer;
						dvar->current.vector.v[1] = v11;
						dvar->current.vector.v[2] = v12;
						dvar->latched.integer = v10;
						dvar->latched.vector.v[1] = v11;
						dvar->latched.vector.v[2] = v12;
						break;
					case 5:
						v13 = currentString.integer;
						v14 = currentString.vector.v[1];
						v15 = currentString.vector.v[2];
						v16 = currentString.vector.v[3];
						dvar->current.integer = currentString.integer;
						dvar->current.vector.v[1] = v14;
						dvar->current.vector.v[2] = v15;
						dvar->current.vector.v[3] = v16;
						dvar->latched.integer = v13;
						dvar->latched.vector.v[1] = v14;
						dvar->latched.vector.v[2] = v15;
						dvar->latched.vector.v[3] = v16;
						break;
					case 8:
						if (!dvar->name
							&& !Assert_MyHandler(
								__FILE__,
								__LINE__,
								0,
								"(dvar->name)",
								nullptr))
						{
							__debugbreak();
						}
						if (currentString.integer == dvar->current.integer
							&& currentString.integer != dvar->latched.integer
							&& currentString.integer != dvar->reset.integer
							&& !Assert_MyHandler(
								__FILE__,
								__LINE__,
								0,
								"((value.string != dvar->current.string || value.string == dvar->latched.string || value.string == "
								"dvar->reset.string))",
								"(dvar->name) = %s",
								dvar->name))
						{
							__debugbreak();
						}
						shouldFreeString = Dvar_ShouldFreeCurrentString(dvar);
						if (shouldFreeString)
							oldString = dvar->current.string;
						Dvar_AssignCurrentStringValue(dvar, &currentString, currentString.string);
						dvar->current.integer = currentString.integer;
						if (Dvar_ShouldFreeLatchedString(dvar))
							Dvar_FreeString(&dvar->latched);
						v7 = dvar->current.string;
						dvar->latched.integer = 0;
						Dvar_WeakCopyString(v7, &dvar->latched);
						if (shouldFreeString)
							FreeString(oldString);
						break;
					default:
						v17 = currentString.integer64;
						v18 = *((__int64*)&currentString.string + 1);
						dvar->current.integer64 = currentString.integer64;
						dvar->latched.integer64 = v17;
						*((__int64*)&dvar->current.string + 1) = v18;
						*((__int64*)&dvar->latched.string + 1) = v18;
						break;
					}
					v19 = (dvar->flags & 0x40000) == 0;
					dvar->modified = 1;
					if (!v19)
					{
						v20 = findCallBackForDvar(dvar);
						if (!v20)
						{
							v21 = va(
								"dvar %s has DVAR_HAS_CALLBACK set, but we didn't find a callback for them in s_dvarCallbackPool\n",
								dvar->name);
							if (!Assert_MyHandler(
								__FILE__,
								__LINE__,
								0,
								"(0 != callbackPtr)",
								"%s",
								v21))
								__debugbreak();
						}
						v20->needsCallback = 1;
					}
				}
				return;
			}
			if (!Dvar_CanChangeValue(dvar, currentString, source))
				return;
			if (!(dvar->flags & 0x20))
				goto LABEL_29;
			Dvar_SetLatchedValue(dvar, currentString);
			if (!Dvar_ValuesEqual(dvar->latched, dvar->current))
				Com_Printf(10, "%s will be changed upon restarting.\n", dvar->name);
		}
	}
}

void Dvar_UpdateEnumDomain(dvar_t* dvar, const char** stringTable)
{
	const char* v2; // eax
	const char* v3; // eax
	DvarValue result; // [esp+0h] [ebp-28h]
	DvarValue v5; // [esp+10h] [ebp-18h]
	int stringCount; // [esp+20h] [ebp-8h]
	dvar_t* malleableDvar; // [esp+24h] [ebp-4h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (!stringTable
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(stringTable)",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar->type != 6)
	{
		v2 = va("dvar %s type %i", dvar->name, dvar->type);
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t%s",
			"dvar->type == DVAR_TYPE_ENUM",
			v2))
			__debugbreak();
	}
	for (stringCount = 0; stringTable[stringCount]; ++stringCount)
		;
	if (dvar->reset.integer < 0 || dvar->reset.integer >= stringCount && dvar->reset.integer)
	{
		v3 = va("name %s reset %i count %i", dvar->name, dvar->reset.integer, stringCount);
		if (!Assert_MyHandler(
			__FILE__,
			1226,
			0,
			"%s\n\t%s",
			"dvar->reset.integer >= 0 && (dvar->reset.integer < stringCount || dvar->reset.integer == 0)",
			v3))
			__debugbreak();
	}
	malleableDvar = dvar;
	dvar->domain.enumeration.stringCount = stringCount;
	malleableDvar->domain.integer.max = (int)stringTable;
	v5 = Dvar_ClampValueToDomain(dvar->type, dvar->current, dvar->reset, dvar->domain);
	malleableDvar->current = v5;
	malleableDvar->latched = dvar->current;
}

bool Dvar_GetBool(dvar_t const* dvar)
{
	bool result; // al

	if (!dvar)
		return 0;
	if (dvar->type
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->type) = %i",
			"(dvar->type == DVAR_TYPE_BOOL || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->type))
	{
		__debugbreak();
	}
	if (dvar->type)
		result = Dvar_StringToBool(dvar->current.string);
	else
		result = dvar->current.enabled;
	return result;
}

int Dvar_GetInt(dvar_t const* dvar)
{
	if (!dvar)
		return 0;
	if (dvar->type
		&& dvar->type != 5
		&& dvar->type != 1
		&& dvar->type != 6
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->type) = %i",
			"(dvar->type == DVAR_TYPE_BOOL || dvar->type == DVAR_TYPE_INT || dvar->type == DVAR_TYPE_FLOAT || dvar->type =="
			" DVAR_TYPE_ENUM || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->type))
	{
		__debugbreak();
	}
	switch (dvar->type)
	{
	case 5:
	case 6:
		return dvar->current.integer;
	case 0:
		return dvar->current.enabled;
	case 1:
		return (int)dvar->current.value;
	}
	return Dvar_StringToInt(dvar->current.string);
}

__int64 Dvar_GetInt64(dvar_t const* dvar)
{
	dvarType_t v2; // eax
	dvarType_t v3; // eax
	dvarType_t v4; // eax

	if (!dvar)
		return 0;
	v2 = dvar->type;
	if (v2 != 1
		&& v2 != 10
		&& v2 != 6
		&& v2 != 2
		&& v2 != 7
		&& (v2 != 8 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((dvar->type == DVAR_TYPE_BOOL || dvar->type == DVAR_TYPE_INT64 || dvar->type == DVAR_TYPE_INT || dvar->type ="
			"= DVAR_TYPE_FLOAT || dvar->type == DVAR_TYPE_ENUM || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14)))))",
			"(dvar->type) = %i",
			dvar->type))
	{
		__debugbreak();
	}
	v3 = dvar->type;
	if (v3 == 6 || v3 == 7)
		return dvar->current.integer;
	Com_PrintWarning(
		23,
		"Dvar '%s' silently casting to a different type (%s -> int64)\n",
		dvar->name,
		s_dvarTypeNames[v3]);
	v4 = dvar->type;
	switch (v4)
	{
	case 1:
		return dvar->current.enabled;
	case 10:
		return dvar->current.integer;
	case 2:
		return (int)dvar->current.value;
	}
	return Dvar_StringToInt64(dvar->current.string);
}

float Dvar_GetFloat(dvar_t const* dvar)
{
	if (!dvar)
		return 0.0;
	if (dvar->type != 1
		&& dvar->type != 5
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->type) = %i",
			"(dvar->type == DVAR_TYPE_FLOAT || dvar->type == DVAR_TYPE_INT ||(dvar->type == DVAR_TYPE_STRING && (dvar->flag"
			"s & (1 << 14))))",
			dvar->type))
	{
		__debugbreak();
	}
	if (dvar->type == 1)
		return dvar->current.value;
	if (dvar->type == 5)
		return (double)dvar->current.integer;
	return Dvar_StringToFloat(dvar->current.string);
}

void Dvar_MakeLatchedValueCurrent(dvar_t const* dvar)
{
	Dvar_SetVariant((dvar_t*)dvar, dvar->latched, DVAR_SOURCE_INTERNAL);
}

void Dvar_Reregister(dvar_t* dvar, const char* dvarName, dvarType_t type, unsigned int flags, DvarValue resetValue, DvarLimits domain, const char* description)
{
	const char* v7; // eax
	const char* v8; // eax
	const char* v9; // eax
	const char* v10; // eax
	const char* v11; // [esp-4h] [ebp-8h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvarName
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvarName"))
	{
		__debugbreak();
	}
	if (dvar->type != type && !(dvar->flags & 0x4000))
	{
		v7 = va("%s: %i != %i", dvarName, dvar->type, type);
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t%s",
			"dvar->type == type || (dvar->flags & DVAR_EXTERNAL)",
			v7))
			__debugbreak();
	}
	if ((dvar->flags ^ flags) & 0x4000)
		Dvar_ReinterpretDvar(dvar, flags, dvarName, type, resetValue, domain);
	if (dvar->flags & 0x4000 && dvar->type != type)
	{
		if (dvar->type != 7)
		{
			v8 = va("dvar %s, type %i", dvar->name, dvar->type);
			if (!Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"%s\n\t%s",
				"dvar->type == DVAR_TYPE_STRING",
				v8))
				__debugbreak();
		}
		Dvar_MakeExplicitType(dvar, dvarName, type, flags, resetValue, domain);
	}
	if (dvar->type != type
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvarName) = %s",
			"(dvar->type == type)",
			dvarName))
	{
		__debugbreak();
	}
	if (!(dvar->flags & 0x9200) && !Dvar_ValuesEqual(dvar->reset, resetValue))
	{
		v11 = Dvar_ValueToString(dvar, resetValue);
		v9 = Dvar_DisplayableResetValue(dvar);
		v10 = va("dvar %s, %s != %s", dvarName, v9, v11);
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t%s",
			"(dvar->flags & (DVAR_CHANGEABLE_RESET|DVAR_SAVED|DVAR_AUTOEXEC)) || Dvar_ValuesEqual( type, dvar->reset, resetValue )",
			v10))
			__debugbreak();
	}
	dvar->flags |= flags;
	if (description)
		dvar->description = description;
	if (dvar->flags & 0x80 && dvar_cheats && !dvar_cheats->current.enabled)
	{
		Dvar_SetVariant(dvar, dvar->reset, DVAR_SOURCE_INTERNAL);
		Dvar_SetLatchedValue(dvar, dvar->reset);
	}
	if (dvar->flags & 0x20)
		Dvar_MakeLatchedValueCurrent(dvar);
}

dvar_t* Dvar_RegisterVariant(const char* dvarName, dvarType_t type, unsigned int flags, DvarValue value, DvarLimits domain, const char* description)
{
	dvar_t* dvar; // [esp+0h] [ebp-8h]

	if (!(flags & 0x4000)
		&& !CanKeepStringPointer(dvarName)
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvarName) = %s",
			"((flags & (1 << 14)) || CanKeepStringPointer( dvarName ))",
			dvarName))
	{
		__debugbreak();
	}
	dvar = Dvar_FindMalleableVar(dvarName);
	if (!dvar)
		return Dvar_RegisterNew(dvarName, type, flags, value, domain, description);
	Dvar_Reregister(dvar, dvarName, type, flags, value, domain, description);
	return dvar;
}

dvar_t* _Dvar_RegisterBool(const char* dvarName, bool value, unsigned int flags, const char* description)
{
	DvarValue dvarValue; // [esp+4h] [ebp-10h]
	DvarLimits empty;

	dvarValue.integer = value;
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_BOOL, flags, dvarValue, empty, description);
}

dvar_t* _Dvar_RegisterInt(const char* dvarName, int value, int min, int max, unsigned int flags, const char* description)
{
	DvarLimits v7; // [esp-14h] [ebp-38h]
	DvarValue dvarValue; // [esp+4h] [ebp-20h]
	DvarLimits dvarDomain; // [esp+14h] [ebp-10h]

	dvarValue.integer = value;
	dvarDomain.integer64.min = __PAIR64__(max, min);
	v7.integer64.min = __PAIR64__(max, min);
	v7.integer64.max = dvarDomain.integer64.max;
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_INT, flags, dvarValue, v7, description);
}

dvar_t* _Dvar_RegisterInt64(const char* dvarName, __int64 value, __int64 min, __int64 max, unsigned int flags, const char* description)
{
	DvarValue v7; // [esp-24h] [ebp-48h]
	DvarLimits v8; // [esp-14h] [ebp-38h]
	__int64 dvarValue_8; // [esp+Ch] [ebp-18h]

	v8.integer64.min = min;
	v8.integer64.max = max;
	v7.integer64 = value;
	*((__int64*)&v7.string + 1) = dvarValue_8;
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_INT64, flags, v7, v8, description);
}

dvar_t* _Dvar_RegisterFloat(const char* dvarName, float value, float min, float max, unsigned int flags, const char* description)
{
	DvarLimits v7; // [esp-18h] [ebp-38h]
	DvarValue dvarValue; // [esp+0h] [ebp-20h]
	DvarLimits dvarDomain; // [esp+10h] [ebp-10h]

	dvarValue.value = value;
	dvarDomain.integer64.min = __PAIR64__(LODWORD(max), LODWORD(min));
	v7.integer64.min = __PAIR64__(LODWORD(max), LODWORD(min));
	v7.integer64.max = dvarDomain.integer64.max;
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_FLOAT, flags, dvarValue, v7, description);
}

dvar_t* _Dvar_RegisterVec2(const char* dvarName, float x, float y, float min, float max, unsigned int flags, const char* description)
{
	DvarValue v8; // [esp-28h] [ebp-4Ch]
	DvarLimits v9; // [esp-18h] [ebp-3Ch]
	__int64 dvarDomain_8; // [esp+8h] [ebp-1Ch]
	__int64 dvarValue_8; // [esp+18h] [ebp-Ch]

	v9.integer64.min = __PAIR64__(LODWORD(max), LODWORD(min));
	v9.integer64.max = dvarDomain_8;
	v8.integer64 = __PAIR64__(LODWORD(y), LODWORD(x));
	*((__int64*)&v8.string + 1) = dvarValue_8;
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_FLOAT_2, flags, v8, v9, description);
}

dvar_t* _Dvar_RegisterVec3(const char* dvarName, float x, float y, float z, float min, float max, unsigned int flags, const char* description)
{
	DvarValue v9; // [esp-28h] [ebp-4Ch]
	DvarLimits v10; // [esp-18h] [ebp-3Ch]
	__int64 dvarDomain_8; // [esp+8h] [ebp-1Ch]
	__int64 dvarValue_8; // [esp+18h] [ebp-Ch]

	*(float*)&dvarValue_8 = z;
	v10.integer64.min = __PAIR64__(LODWORD(max), LODWORD(min));
	v10.integer64.max = dvarDomain_8;
	v9.integer64 = __PAIR64__(LODWORD(y), LODWORD(x));
	*((__int64*)&v9.string + 1) = dvarValue_8;
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_FLOAT_3, flags, v9, v10, description);
}

dvar_t* _Dvar_RegisterVec4(const char* dvarName, float x, float y, float z, float w, float min, float max, unsigned int flags, const char* description)
{
	DvarValue v10; // [esp-28h] [ebp-4Ch]
	DvarLimits v11; // [esp-18h] [ebp-3Ch]
	__int64 dvarDomain_8; // [esp+8h] [ebp-1Ch]

	v11.integer64.min = __PAIR64__(LODWORD(max), LODWORD(min));
	v11.integer64.max = dvarDomain_8;
	v10.integer64 = __PAIR64__(LODWORD(y), LODWORD(x));
	*((__int64*)&v10.string + 1) = __PAIR64__(LODWORD(w), LODWORD(z));
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_FLOAT_4, flags, v10, v11, description);
}

dvar_t* _Dvar_RegisterString(const char* dvarName, const char* value, unsigned int flags, const char* description)
{
	DvarValue dvarValue; // [esp+Ch] [ebp-10h]
	DvarLimits empty;

	if (!dvarName
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvarName)",
			nullptr))
	{
		__debugbreak();
	}
	if (!value
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(value)", nullptr))
	{
		__debugbreak();
	}
	if (!(flags & 0x4000)
		&& !CanKeepStringPointer(value)
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(((flags & (1 << 14)) || CanKeepStringPointer( value )))",
			"(dvarName) = %s",
			dvarName))
	{
		__debugbreak();
	}
	dvarValue.integer = (int)value;
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_STRING, flags, dvarValue, empty, description);
}

dvar_t* _Dvar_RegisterEnum(const char* dvarName, const char** valueList, int defaultIndex, unsigned int flags, const char* description)
{
	int v5; // eax
	bool v6; // zf
	DvarValue dvarValue; // [esp+8h] [ebp-20h]
	DvarLimits dvarDomain; // [esp+18h] [ebp-10h]

	if (!dvarName
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvarName)",
			nullptr))
	{
		__debugbreak();
	}
	if (!valueList
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(valueList)",
			nullptr))
	{
		__debugbreak();
	}
	v5 = 0;
	dvarValue.integer = defaultIndex;
	dvarDomain.integer.max = (int)valueList;
	dvarDomain.enumeration.stringCount = 0;
	if (*valueList)
	{
		do
			++v5;
		while (valueList[v5]);
		dvarDomain.enumeration.stringCount = v5;
	}
	v6 = defaultIndex == 0;
	if (defaultIndex >= 0)
	{
		if (defaultIndex < v5)
			return Dvar_RegisterVariant(dvarName, DVAR_TYPE_ENUM, flags, dvarValue, dvarDomain, description);
		v6 = defaultIndex == 0;
	}
	if (!v6
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((defaultIndex >= 0 && defaultIndex < dvarDomain.enumeration.stringCount || defaultIndex == 0))",
			"(dvarName) = %s",
			dvarName))
	{
		__debugbreak();
	}
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_ENUM, flags, dvarValue, dvarDomain, description);
}

dvar_t* _Dvar_RegisterColor(const char* dvarName, float r, float g, float b, float a, unsigned int flags, const char* description)
{
	DvarLimits v8; // [esp-14h] [ebp-ACh]
	float v9; // [esp+0h] [ebp-98h]
	float v10; // [esp+4h] [ebp-94h]
	float v11; // [esp+8h] [ebp-90h]
	float v12; // [esp+Ch] [ebp-8Ch]
	float v13; // [esp+24h] [ebp-74h]
	float v14; // [esp+3Ch] [ebp-5Ch]
	float v15; // [esp+54h] [ebp-44h]
	float v16; // [esp+6Ch] [ebp-2Ch]
	DvarValue dvarValue; // [esp+80h] [ebp-18h]

	if ((r - 1.0) < 0.0)
		v16 = r;
	else
		v16 = 1.0;
	if ((0.0 - v16) < 0.0)
		v12 = v16;
	else
		v12 = 0.0;
	if ((g - 1.0) < 0.0)
		v15 = g;
	else
		v15 = 1.0;
	if ((0.0 - v15) < 0.0)
		v11 = v15;
	else
		v11 = 0.0;
	if ((b - 1.0) < 0.0)
		v14 = b;
	else
		v14 = 1.0;
	if ((0.0 - v14) < 0.0)
		v10 = v14;
	else
		v10 = 0.0;
	if ((a - 1.0) < 0.0)
		v13 = a;
	else
		v13 = 1.0;
	if ((0.0 - v13) < 0.0)
		v9 = v13;
	else
		v9 = 0.0;
	dvarValue.color[3] = (((255.0 * v9) + 0.001) + 9.313225746154785e-10);
	v8.enumeration.stringCount = 0;
	*&v8.value.max = 0i64;
	*((__int64*)&v8.vector + 3) = 0;
	dvarValue.enabled = (((255.0 * v12) + 0.001) + 9.313225746154785e-10);
	dvarValue.color[1] = (((255.0 * v11) + 0.001) + 9.313225746154785e-10);
	dvarValue.color[2] = (((255.0 * v10) + 0.001) + 9.313225746154785e-10);
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_COLOR, flags, dvarValue, v8, description);
}

dvar_t* _Dvar_RegisterLinearRGB(const char* dvarName, float x, float y, float z, float min, float max, unsigned int flags, const char* description)
{
	DvarValue v9; // [esp-24h] [ebp-4Ch]
	DvarLimits v10; // [esp-14h] [ebp-3Ch]
	__int64 dvarDomain_4; // [esp+4h] [ebp-24h]
	int dvarDomain_12; // [esp+Ch] [ebp-1Ch]
	unsigned int dvarValue_12; // [esp+1Ch] [ebp-Ch]

	*(float*)&dvarDomain_4 = max;
	v10.value.min = min;
	*(__int64*)&v10.value.max = dvarDomain_4;
	*((__int64*)&v10.vector + 3) = dvarDomain_12;
	v9.integer64 = __PAIR64__(LODWORD(y), LODWORD(x));
	*((__int64*)&v9.string + 1) = __PAIR64__(dvarValue_12, LODWORD(z));
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_LINEAR_COLOR_RGB, flags, v9, v10, description);
}

dvar_t* _Dvar_RegisterColorXYZ(const char* dvarName, float x, float y, float z, float min, float max, unsigned int flags, const char* description)
{
	DvarValue v9; // [esp-24h] [ebp-4Ch]
	DvarLimits v10; // [esp-14h] [ebp-3Ch]
	__int64 dvarDomain_4; // [esp+4h] [ebp-24h]
	int dvarDomain_12; // [esp+Ch] [ebp-1Ch]
	unsigned int dvarValue_12; // [esp+1Ch] [ebp-Ch]

	*(float*)&dvarDomain_4 = max;
	v10.value.min = min;
	*(__int64*)&v10.value.max = dvarDomain_4;
	*((__int64*)&v10.vector + 3) = dvarDomain_12;
	v9.integer64 = __PAIR64__(LODWORD(y), LODWORD(x));
	*((__int64*)&v9.string + 1) = __PAIR64__(dvarValue_12, LODWORD(z));
	return Dvar_RegisterVariant(dvarName, DVAR_TYPE_COLOR_XYZ, flags, v9, v10, description);
}

void Dvar_SetBoolFromSource(dvar_t const* dvar, bool value, DvarSetSource source)
{
	const char* v3; // [esp+0h] [ebp-1Ch]
	DvarValue newValue; // [esp+4h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_BOOL || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar && dvar->name)
	{
		if (dvar->type)
		{
			if (value)
				v3 = "1";
			else
				v3 = "0";
			newValue.integer = (int)v3;
		}
		else
		{
			newValue.enabled = value;
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetIntFromSource(dvar_t const* dvar, int value, DvarSetSource source)
{
	char string[32]; // [esp+0h] [ebp-38h]
	DvarValue newValue; // [esp+20h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type != 5
		&& dvar->type != 6
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_INT || dvar->type == DVAR_TYPE_ENUM || (dvar->type == DVAR_TYPE_STRING && (dvar->flag"
			"s & (1 << 14))))",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar && dvar->name)
	{
		if (dvar->type != 5 && dvar->type != 6)
		{
			Com_sprintf(string, 32, "%i", value);
			newValue.integer = (int)string;
		}
		else
		{
			newValue.integer = value;
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetInt64FromSource(dvar_t const* dvar, __int64 value, DvarSetSource source)
{
	char string[32]; // [esp+0h] [ebp-38h]
	DvarValue newValue; // [esp+20h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type != 9
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_INT64 || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar && dvar->name)
	{
		if (dvar->type == 9)
		{
			newValue.integer64 = value;
		}
		else
		{
			Com_sprintf(string, 32, "%lli", value);
			newValue.integer = (int)string;
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetFloatFromSource(dvar_t const* dvar, float value, DvarSetSource source)
{
	char string[32]; // [esp+8h] [ebp-38h]
	DvarValue newValue; // [esp+28h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type != 1
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_FLOAT || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar && dvar->name)
	{
		if (dvar->type == 1)
		{
			newValue.value = value;
		}
		else
		{
			Com_sprintf(string, 32, "%g", value);
			newValue.integer = (int)string;
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetVec2FromSource(dvar_t const* dvar, float x, float y, DvarSetSource source)
{
	char string[64]; // [esp+10h] [ebp-58h]
	DvarValue newValue; // [esp+50h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type != 2
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_FLOAT_2 || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar && dvar->name)
	{
		if (dvar->type == 2)
		{
			newValue.integer64 = __PAIR64__(LODWORD(y), LODWORD(x));
		}
		else
		{
			Com_sprintf(string, 64, "%g %g", x, y);
			newValue.integer = (int)string;
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetVec3FromSource(dvar_t const* dvar, float x, float y, float z, DvarSetSource source)
{
	char string[96]; // [esp+18h] [ebp-78h]
	DvarValue newValue; // [esp+78h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type != 3
		&& dvar->type != 11
		&& dvar->type != 10
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_FLOAT_3 || dvar->type == DVAR_TYPE_COLOR_XYZ || dvar->type == DVAR_TYPE_LINEAR_COLOR_"
			"RGB || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar && dvar->name)
	{
		if (dvar->type != 3 && dvar->type != 10 && dvar->type != 11)
		{
			Com_sprintf(string, 96, "%g %g %g", x, y, z);
			newValue.integer = (int)string;
		}
		else
		{
			newValue.integer64 = __PAIR64__(LODWORD(y), LODWORD(x));
			newValue.vector.v[2] = z;
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetVec4FromSource(dvar_t const* dvar, float x, float y, float z, float w, DvarSetSource source)
{
	char string[128]; // [esp+20h] [ebp-98h]
	DvarValue newValue; // [esp+A0h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type != 4
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_FLOAT_4 || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar && dvar->name)
	{
		if (dvar->type == 4)
		{
			newValue.integer64 = __PAIR64__(LODWORD(y), LODWORD(x));
			*((__int64*)&newValue.string + 1) = __PAIR64__(LODWORD(w), LODWORD(z));
		}
		else
		{
			Com_sprintf(string, 128, "%g %g %g %g", x, y, z, w);
			newValue.integer = (int)string;
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetStringFromSource(dvar_t const* dvar, const char* string, DvarSetSource source)
{
	const char* v3; // eax
	char stringCopy[1024]; // [esp+0h] [ebp-418h]
	DvarValue newValue; // [esp+400h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type != 7
		&& dvar->type != 6
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_STRING || dvar->type == DVAR_TYPE_ENUM)",
			dvar->name))
	{
		__debugbreak();
	}
	if (!string && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "string"))
		__debugbreak();
	if (dvar && dvar->name)
	{
		if (dvar->type == 7)
		{
			I_strncpyz(stringCopy, string, 1024);
			newValue.integer = (int)stringCopy;
		}
		else
		{
			newValue.integer = Dvar_StringToEnum(&dvar->domain, string);
			if (newValue.integer == -1337)
			{
				v3 = va("%s doesn't include %s", dvar->name, string);
				if (!Assert_MyHandler(
					__FILE__,
					__LINE__,
					0,
					"%s\n\t%s",
					"newValue.integer != DVAR_INVALID_ENUM_INDEX",
					v3))
					__debugbreak();
			}
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetColorFromSource(dvar_t const* dvar, float r, float g, float b, float a, DvarSetSource source)
{
	float v6; // [esp+20h] [ebp-108h]
	float v7; // [esp+24h] [ebp-104h]
	float v8; // [esp+28h] [ebp-100h]
	float v9; // [esp+2Ch] [ebp-FCh]
	float v10; // [esp+44h] [ebp-E4h]
	float v11; // [esp+5Ch] [ebp-CCh]
	float v12; // [esp+74h] [ebp-B4h]
	float v13; // [esp+8Ch] [ebp-9Ch]
	char string[128]; // [esp+90h] [ebp-98h]
	DvarValue newValue; // [esp+110h] [ebp-18h]

	if (!dvar && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar"))
		__debugbreak();
	if (!dvar->name
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "dvar->name"))
	{
		__debugbreak();
	}
	if (dvar->type != 8
		&& (dvar->type != 7 || !(dvar->flags & 0x4000))
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"%s\n\t(dvar->name) = %s",
			"(dvar->type == DVAR_TYPE_COLOR || (dvar->type == DVAR_TYPE_STRING && (dvar->flags & (1 << 14))))",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar && dvar->name)
	{
		if (dvar->type == 8)
		{
			if ((r - 1.0) < 0.0)
				v13 = r;
			else
				v13 = 1.0;
			if ((0.0 - v13) < 0.0)
				v9 = v13;
			else
				v9 = 0.0;
			newValue.enabled = ((255.0 * v9) + 9.313225746154785e-10);
			if ((g - 1.0) < 0.0)
				v12 = g;
			else
				v12 = 1.0;
			if ((0.0 - v12) < 0.0)
				v8 = v12;
			else
				v8 = 0.0;
			newValue.color[1] = ((255.0 * v8) + 9.313225746154785e-10);
			if ((b - 1.0) < 0.0)
				v11 = b;
			else
				v11 = 1.0;
			if ((0.0 - v11) < 0.0)
				v7 = v11;
			else
				v7 = 0.0;
			newValue.color[2] = ((255.0 * v7) + 9.313225746154785e-10);
			if ((a - 1.0) < 0.0)
				v10 = a;
			else
				v10 = 1.0;
			if ((0.0 - v10) < 0.0)
				v6 = v10;
			else
				v6 = 0.0;
			newValue.color[3] = ((255.0 * v6) + 9.313225746154785e-10);
		}
		else
		{
			Com_sprintf(string, 128, "%g %g %g %g", r, g, b, a);
			newValue.integer = (int)string;
		}
		Dvar_SetVariant((dvar_t*)dvar, newValue, source);
	}
}

void Dvar_SetBool(dvar_t const* dvar, bool value)
{
	Dvar_SetBoolFromSource(dvar, value, DVAR_SOURCE_INTERNAL);
}

void Dvar_SetInt(dvar_t const* dvar, int value)
{
	Dvar_SetIntFromSource(dvar, value, DVAR_SOURCE_INTERNAL);
}

void Dvar_SetFloat(dvar_t const* dvar, float value)
{
	Dvar_SetFloatFromSource(dvar, value, DVAR_SOURCE_INTERNAL);
}

void Dvar_SetVec2(dvar_t const* dvar, float x , float y)
{
	Dvar_SetVec2FromSource(dvar, x, y, DVAR_SOURCE_INTERNAL);
}

void Dvar_SetVec3(dvar_t const* dvar, float x, float y, float z)
{
	Dvar_SetVec3FromSource(dvar, x, y, z, DVAR_SOURCE_INTERNAL);
}

void Dvar_SetVec4(dvar_t const* dvar, float x, float y, float z, float w)
{
	Dvar_SetVec4FromSource(dvar, x, y, z, w, DVAR_SOURCE_INTERNAL);
}

void Dvar_SetString(dvar_t const* dvar, const char* value)
{
	Dvar_SetStringFromSource(dvar, value, DVAR_SOURCE_INTERNAL);
}

void Dvar_SetFromStringFromSource(dvar_t const* dvar, const char* string, DvarSetSource source)
{
	__int64* v3; // eax
	__int64 v4; // xmm0_8
	__int64 v5; // xmm1_8
	bool v6; // zf
	DvarLimits* v7; // [esp-14h] [ebp-43Ch]
	DvarValue v8; // [esp-14h] [ebp-43Ch]
	DvarLimits v9; // [esp-10h] [ebp-438h] OVERLAPPED
	DvarLimits domain; // [esp+4h] [ebp-424h]
	DvarValue newValue; // [esp+14h] [ebp-414h]
	char buf[1024]; // [esp+24h] [ebp-404h]

	if (dvar && dvar->name)
	{
		I_strncpyz(buf, string, 1024);
		v9 = dvar->domain;
		v7 = &domain;
		v3 = (__int64*)Dvar_StringToValue((const dvarType_t)dvar->type, buf, *(const DvarLimits*)((char*)&v9 - 4));
		v4 = *v3;
		v5 = v3[1];
		v6 = dvar->type == 7;
		newValue.integer64 = *v3;
		if (v6 && newValue.integer == -1337)
		{
			Com_Printf(10, "'%s' is not a valid value for dvar '%s'\n", buf, dvar->name);
			Dvar_PrintDomain(dvar->type, dvar->domain);
			v4 = dvar->reset.integer64;
			v5 = *((__int64*)&dvar->reset.string + 1);
		}
		v8.integer64 = v4;
		*((__int64*)&v8.string + 1) = v5;
		Dvar_SetVariant((dvar_t*)dvar, v8, source);
	}
}

dvar_t* Dvar_SetOrRegisterString(dvar_t const* dvar, const char* dvarName, const char* value)
{
	if (!dvar)
		return _Dvar_RegisterString(dvarName, value, 0x4000u, "External Dvar");
	Dvar_SetStringFromSource(dvar, value, DVAR_SOURCE_INTERNAL);
	return (dvar_t*)dvar;
}

void Dvar_SetColor(dvar_t const* dvar, float r, float g, float b, float a)
{
	Dvar_SetColorFromSource(dvar, r, g, b, a, DVAR_SOURCE_INTERNAL);
}

void Dvar_SetFromString(dvar_t const* dvar, const char* string)
{
	Dvar_SetFromStringFromSource(dvar, string, DVAR_SOURCE_INTERNAL);
}

dvar_t* Dvar_SetFromStringByNameFromSource(const char* dvarName, const char* string, DvarSetSource source, unsigned int flags)
{
	const dvar_t* v4; // edi

	if (!dvarName)
	{
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvarName)",
			nullptr))
			__debugbreak();
		return _Dvar_RegisterString(dvarName, string, flags | 0x4000, "External Dvar");
	}
	if (!*dvarName)
		return _Dvar_RegisterString(dvarName, string, flags | 0x4000, "External Dvar");
	v4 = Dvar_FindMalleableVar(dvarName);
	if (!v4)
		return _Dvar_RegisterString(dvarName, string, flags | 0x4000, "External Dvar");
	Dvar_SetFromStringFromSource(v4, string, source);
	return (dvar_t*)v4;
}

void Dvar_SetFromStringByName(const char* dvarName, const char* string)
{
	Dvar_SetFromStringByNameFromSource(dvarName, string, DVAR_SOURCE_INTERNAL, 0);
}

void Dvar_SetCommand(const char* dvarName, const char* string)
{
	dvar_t* v2; // eax
	dvar_t* v3; // esi

	v2 = Dvar_SetFromStringByNameFromSource(dvarName, string, DVAR_SOURCE_EXTERNAL, 0);
	v3 = v2;
	if (v2)
	{
		if (s_isLoadingAutoExecGlobalFlag)
		{
			Dvar_AddFlags(v2, 0x8000);
			Dvar_UpdateResetValue(v3, v3->current);
		}
	}
}

void Dvar_Reset(dvar_t const* dvar, DvarSetSource setSource)
{
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", nullptr))
	{
		__debugbreak();
	}
	Dvar_SetVariant((dvar_t*)dvar, dvar->reset, setSource);
}

void Dvar_SetCheatState(void) {

}

void Dvar_Init(void)
{
    DvarValue v0; // [esp-Ch] [ebp-70h]
    DvarLimits g;

    DvarValue v812; // [esp+28h] [ebp-3Ch]
    __int64 v814; // [esp+38h] [ebp-2Ch]

    DvarLimits emptyLimit;
    DvarValue emptyValue;

    v812.integer = 0;
    s_nextFreeCallback = 0;
    s_isDvarSystemActive = 1;
    cg_hudGrenadeIconInScope = Dvar_RegisterVariant(
        "cg_hudGrenadeIconInScope",
        DVAR_TYPE_BOOL,
        0x84000u,
        v812,
        emptyLimit,
        "Show the grenade indicator when aiming down the sight of a scoped weapon");
    v812.integer = 0;
    cg_hudGrenadeIndicatorFadeUp = Dvar_RegisterVariant(
        "cg_hudGrenadeIndicatorFadeUp",
        DVAR_TYPE_BOOL,
        0x84000u,
        v812,
        emptyLimit,
        "Draw grenade indicator with distance fade(COD3 style)");
    v812.integer = 0;
    v0 = v812;
    v812.integer = 1;
    cg_scoreboardPingGraph = Dvar_RegisterVariant(
        "cg_scoreboardPingGraph",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Whether to show graphical ping");
    cg_scoreboardPingText = Dvar_RegisterVariant(
        "cg_scoreboardPingText",
        DVAR_TYPE_BOOL,
        0x84000u,
        v812,
        emptyLimit,
        "Whether to show numeric ping value");
    v812.integer = 0;
    v0 = v812;
    v814 = 5.0;
    v812.integer = 0;
    v812.vector.v[1] = 100.0;
    con_restricted = Dvar_RegisterVariant(
        "con_restricted",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Restrict console access");
    g = v812;
    v0.integer64 = 5.0;
    *((__int64*)&v0.string + 1) = 0i64;
    *(float*)&v814 = 0.75;
    v812.integer = 0;
    cg_connectionIconSize = Dvar_RegisterVariant(
        "cg_connectionIconSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Size of the connection icon");
    LODWORD(v812.vector.v[1]) = 1.0;
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    *(float*)&v814 = 435.0;
    v812.integer = 0;
    v812.vector.v[1] = 3.4028235e38;
    cg_infraredHighlightOffset = Dvar_RegisterVariant(
        "cg_infraredHighlightOffset",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Offset to the player highlight when using infrared scope");
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    *(float*)&v814 = 0.25999999;
    v812.integer = 0;
    v812.vector.v[1] = 3.4028235e38;
    cg_scoreboardHeight = Dvar_RegisterVariant(
        "cg_scoreboardHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Height of the scoreboard");
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    *(float*)&v814 = 0.60000002;
    v812.integer = 0;
    v812.vector.v[1] = 3.4028235e38;
    cg_scoreboardRankFontScale = Dvar_RegisterVariant(
        "cg_scoreboardRankFontScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale of rank font");
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    *(float*)&v814 = 11.0;
    v812.integer = 0;
    v812.vector.v[1] = 100.0;
    cg_scoreboardTextOffset = Dvar_RegisterVariant(
        "cg_scoreboardTextOffset",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scoreboard text offset");
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    *(float*)&v814 = 18.0;
    v812.integer = 0;
    v812.vector.v[1] = 3.4028235e38;
    cg_youInKillCamSize = Dvar_RegisterVariant(
        "cg_youInKillCamSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Size of the 'you' Icon in the kill cam");
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    *(float*)&v814 = 18.0;
    v812.integer = 0;
    v812.vector.v[1] = 3.4028235e38;
    compassObjectiveIconHeight = Dvar_RegisterVariant(
        "compassObjectiveIconHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the objective on the full map");
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    compassObjectiveIconWidth = Dvar_RegisterVariant(
        "compassObjectiveIconWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the objective on the full map");
    *(float*)&v814 = 18.75;
    v812.integer = 0;
    v812.vector.v[1] = 3.4028235e38;
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    *(float*)&v814 = 18.75;
    v812.integer = 0;
    v812.vector.v[1] = 3.4028235e38;
    compassPlayerHeight = Dvar_RegisterVariant(
        "compassPlayerHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the player's icon on the compass");
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    v812.integer = 0;
    v812.vector.v[1] = 30.0;
    compassPlayerWidth = Dvar_RegisterVariant(
        "compassPlayerWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the player's icon on the compass");
    g = v812;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    v812.integer = 0;
    hud_fade_ammodisplay = Dvar_RegisterVariant(
        "hud_fade_ammodisplay",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The time for the ammo display to fade in seconds");
    v812.vector.v[1] = 30.0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    hud_fade_offhand = Dvar_RegisterVariant(
        "hud_fade_offhand",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The time for the offhand weapons to fade in seconds");
    g.integer64.min = 0x28000000064i64;
    g.integer64.max = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 600;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_objectiveListWrapCountStandard = Dvar_RegisterVariant(
        "cg_objectiveListWrapCountStandard",
        DVAR_TYPE_INT,
        0x84000u,
        v0,
        g,
        "The amount of on-screen length to wrap an objective in non wide-screen mode");
    g.integer64.min = 0x7FFFFFFF00000000i64;
    g.integer64.max = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 41;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_rangeFinderActiveReticleIndex = Dvar_RegisterVariant(
        "cg_rangeFinderActiveReticleIndex",
        DVAR_TYPE_INT,
        0x84000u,
        v0,
        g,
        "Reticle index for the rang finder when an enemy is spotted.");
    g.integer64.min = 0x7FFFFFFF00000000i64;
    g.integer64.max = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 40;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_rangeFinderDefaultReticleIndex = Dvar_RegisterVariant(
        "cg_rangeFinderDefaultReticleIndex",
        DVAR_TYPE_INT,
        0x84000u,
        v0,
        g,
        "Default reticle index for the rang finder attachment.");
    g.integer64.min = 0x6400000000i64;
    g.integer64.max = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 5;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_scriptIconSize = Dvar_RegisterVariant(
        "cg_scriptIconSize",
        DVAR_TYPE_INT,
        0x84000u,
        v0,
        g,
        "Size of Icons defined by script");
    g.integer64.min = 0x7FFFFFFF00000000i64;
    g.integer64.max = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 400;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_waterTrailRippleFrequency = Dvar_RegisterVariant(
        "cg_waterTrailRippleFrequency",
        DVAR_TYPE_INT,
        0x84000u,
        v0,
        g,
        "How often (in ms) will play the waist ripple fx for actors in water");
    g.integer64.min = 0x7FFFFFFF00000000i64;
    g.integer64.max = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 200;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_waterTrailRippleVariance = Dvar_RegisterVariant(
        "cg_waterTrailRippleVariance",
        DVAR_TYPE_INT,
        0x84000u,
        v0,
        g,
        "How late (in ms) the waist ripple fx can be played");
    g.integer64.min = 0x61A8000003E8i64;
    g.integer64.max = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 5000;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_maxRate = Dvar_RegisterVariant("sv_maxRate", DVAR_TYPE_INT, 0x84000u, v0, g, "Maximum byte rate (per second)");
    g.integer64.min = __PAIR64__(640.0, -200.0);
    g.integer64.max = *((__int64*)&v812.string + 1);
    v0.integer64 = 0i64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.vector.v[1] = 20.0;
    cg_debugInfoCornerOffset = Dvar_RegisterVariant(
        "cg_debugInfoCornerOffset",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Offset from top-right corner, for cg_drawFPS, etc");
    g.integer64.min = __PAIR64__(-10000.0, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(20.0, -40.0);
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    ui_buildLocation = Dvar_RegisterVariant(
        "ui_buildLocation",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Where to draw the build number");
    g.integer64.min = 0x12C00000000i64;
    g.integer64.max = 0i64;
    v812.integer = 15;
    live_steam_server_client_checks_interval = Dvar_RegisterVariant(
        "live_steam_server_client_checks_interval",
        DVAR_TYPE_INT,
        0x84000u,
        v812,
        g,
        "timeout(in seconds) between checks for server clients if they are steam authorized");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ai_ShowCanSeeChecks = Dvar_RegisterVariant(
        "ai_ShowCanSeeChecks",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Display debugging information for 'cansee' checks");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ai_badPathSpam = Dvar_RegisterVariant(
        "ai_badPathSpam",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Write debugging information for a bad AI paths");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ai_debugEntLock = Dvar_RegisterVariant(
        "ai_debugEntLock",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Lock the currently selected debug entity");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ai_debug_dynamic_nodes = Dvar_RegisterVariant(
        "ai_debug_dynamic_nodes",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "enable dynamic nodes debug prints");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ai_enableBadPlaces = Dvar_RegisterVariant(
        "ai_enableBadPlaces",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "toggle badplaces system on/off");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ai_secondaryThreats = Dvar_RegisterVariant(
        "ai_secondaryThreats",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Allow secondary targets");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    bg_drawGrenadeInHand = Dvar_RegisterVariant(
        "bg_drawGrenadeInHand",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Whether or not to draw grenade in hand for grenade animations");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    bg_drawProximity = Dvar_RegisterVariant(
        "bg_drawProximity",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw the collision surfaces in the proximity of the player");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    bg_enableIKActiveFix = Dvar_RegisterVariant(
        "bg_enableIKActiveFix",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable fix for the IK active flag being lost whenever a player dobj is rebuilt");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    bg_freeCamClipToHeliPatch = Dvar_RegisterVariant(
        "bg_freeCamClipToHeliPatch",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Toggles clipping to the heli patch rectangle.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    bg_playStandToCrouchAnims = Dvar_RegisterVariant(
        "bg_playStandToCrouchAnims",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Whether or not to use animations to transition between stand and crouch, if not it just blends");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_ai_useServerAnims = Dvar_RegisterVariant(
        "cg_ai_useServerAnims",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Copy the AI animations from the server instead of using networked animations.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_bloodLimit = Dvar_RegisterVariant(
        "cg_bloodLimit",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Limit blood effects (to 'prevent excess blood stacking')");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_brass = Dvar_RegisterVariant("cg_brass", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "Weapons eject brass");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_debugDrawSafeAreas = Dvar_RegisterVariant(
        "cg_debugDrawSafeAreas",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "This determines whether to display the safe areas for all active viewports");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_debug_triggers = Dvar_RegisterVariant(
        "cg_debug_triggers",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Debug client side Triggers, prints out all the client triggers the first time they are hit.");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_defensive_anim_delay = Dvar_RegisterVariant(
        "cg_defensive_anim_delay",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Delay the anim commands to the server time");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_descriptiveText = Dvar_RegisterVariant(
        "cg_descriptiveText",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw descriptive spectator messages");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawFPSLabels = Dvar_RegisterVariant(
        "cg_drawFPSLabels",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw FPS Info Labels");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawMantleHint = Dvar_RegisterVariant(
        "cg_drawMantleHint",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw a 'press key to mantle' hint");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawScriptUsage = Dvar_RegisterVariant(
        "cg_drawScriptUsage",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw debugging information for scripts");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawSnapshot = Dvar_RegisterVariant(
        "cg_drawSnapshot",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw debugging information for snapshots");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawSpectatorMessages = Dvar_RegisterVariant(
        "cg_drawSpectatorMessages",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enables drawing of spectator HUD messages.");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawTurretCrosshair = Dvar_RegisterVariant(
        "cg_drawTurretCrosshair",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw a cross hair when using a turret");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawVersion = Dvar_RegisterVariant(
        "cg_drawVersion",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw the game version");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawWaypointsPostBloom = Dvar_RegisterVariant(
        "cg_drawWaypointsPostBloom",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw the game waypoints after the bloom");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_drawYouInKillCamAsWaypoint = Dvar_RegisterVariant(
        "cg_drawYouInKillCamAsWaypoint",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw the 'you' icon as a way point - false will draw as a billboard game waypoints after the bloom");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_fullscreenFinalKillcam = Dvar_RegisterVariant(
        "cg_fullscreenFinalKillcam",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Fullscreen final killcam");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_forceEmp = Dvar_RegisterVariant(
        "cg_forceEmp",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Force local player to be EMP jammed");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_hudDamageIconInScope = Dvar_RegisterVariant(
        "cg_hudDamageIconInScope",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw damag icons when aiming down the sight of a scoped weapon");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_hudGrenadeIconEnabledFlash = Dvar_RegisterVariant(
        "cg_hudGrenadeIconEnabledFlash",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Show the grenade indicator for flash grenades");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_hudSplitscreenOffsetsUseScale = Dvar_RegisterVariant(
        "cg_hudSplitscreenOffsetsUseScale",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Use splitscreen scaling for element offsets");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_hudStanceHintPrints = Dvar_RegisterVariant(
        "cg_hudStanceHintPrints",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw helpful text to say how to chang stances");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_laserLight = Dvar_RegisterVariant(
        "cg_laserLight",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Whether to draw the light emitted from a laser (not the laser itself)");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_marks_ents_player_only = Dvar_RegisterVariant(
        "cg_marks_ents_player_only",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Marks on entities from player's bullets only.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_minicon = Dvar_RegisterVariant(
        "cg_minicon",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Display the mini console on screen");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_nopredict = Dvar_RegisterVariant(
        "cg_nopredict",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Don't do client side prediction");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_oneFriendlyHeadTracePerFrame = Dvar_RegisterVariant(
        "cg_oneFriendlyHeadTracePerFrame",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "head trace check");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_predictItems = Dvar_RegisterVariant(
        "cg_predictItems",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Turn on client side prediction for item pickup");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_profile_physics = Dvar_RegisterVariant(
        "cg_profile_physics",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw physics & collision profiltimers");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_scriptmover_useServerAnims = Dvar_RegisterVariant(
        "cg_scriptmover_useServerAnims",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Copy the script_mover animations from the server instead of using networked animations.");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_showZombieMap = Dvar_RegisterVariant(
        "cg_showZombieMap",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Show the zombie map up");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_teamChatsOnly = Dvar_RegisterVariant(
        "cg_teamChatsOnly",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Allow chatting only on the same team");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_thirdPersonFootstepsFromNotetracks = Dvar_RegisterVariant(
        "cg_thirdPersonFootstepsFromNotetracks",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Notetracks for third person are driven from notetracks instead of server events");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cheapSpawns = Dvar_RegisterVariant("cheapSpawns", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, nullptr);
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_forceavidemo = Dvar_RegisterVariant(
        "cl_forceavidemo",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Record AVI demo even if client is not active");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_hudDrawsBehindUI = Dvar_RegisterVariant(
        "cl_hudDrawsBehindUI",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Should the HUD draw when the UI is up?");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_noprint = Dvar_RegisterVariant(
        "cl_noprint",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print nothing to the console");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_showMouseRate = Dvar_RegisterVariant(
        "cl_showMouseRate",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print mouse rate debugging information to the console");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_showSend = Dvar_RegisterVariant(
        "cl_showSend",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable debugging information for sent commands");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_showServerCommands = Dvar_RegisterVariant(
        "cl_showServerCommands",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable debugging information for server commands");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_showTimeDelta = Dvar_RegisterVariant(
        "cl_showTimeDelta",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable debugging information for time delta");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_shownuments = Dvar_RegisterVariant(
        "cl_shownuments",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Show the number of entities");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    cl_socketpool_enabled = Dvar_RegisterVariant(
        "cl_socketpool_enabled",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "True is socketpool technolog is enabled");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    com_animCheck = Dvar_RegisterVariant("com_animCheck", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "Check anim tree");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    com_attractmode = Dvar_RegisterVariant(
        "com_attractmode",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Run attract mode");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    com_filter_output = Dvar_RegisterVariant(
        "com_filter_output",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Use console filters for filtering output.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    com_script_debugger_smoke_test = Dvar_RegisterVariant(
        "com_script_debugger_smoke_test",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "perform script debugger smoke test and exit");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    com_voip_bandwidth_restricted = Dvar_RegisterVariant(
        "com_voip_bandwidth_restricted",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Use VOIP inhibitor during hig bandwidth usage");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    useWalkPathnodesMode = Dvar_RegisterVariant(
        "useWalkPathnodesMode",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Walk path nodes and log static (map) texture streaming memory usage");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    compassRotation = Dvar_RegisterVariant(
        "compassRotation",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Style of compass");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    compassSpectatorsSeeEnemies = Dvar_RegisterVariant(
        "compassSpectatorsSeeEnemies",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Spectators always see enemies on the map.");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    con_matchPrefixOnly = Dvar_RegisterVariant(
        "con_matchPrefixOnly",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Only match the prefix when listing matching Dvars");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    con_typewriterEnabledSounds = Dvar_RegisterVariant(
        "con_typewriterEnabledSounds",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable the typewriter sounds and text effects.  Disabled still uses the typewrite glow setting.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    dcacheSimulateNoHDD = Dvar_RegisterVariant(
        "dcacheSimulateNoHDD",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "When turned on, simulated no HDD for caching.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    debugCurves = Dvar_RegisterVariant("debugCurves", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "Draw active curves.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    debug_triggers = Dvar_RegisterVariant(
        "debug_triggers",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Debug Triggers, prints out all the triggers the first time they are hit.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    demo_profiling = Dvar_RegisterVariant(
        "demo_profiling",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Used to turn the system on/off.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    demo_usefilesystem = Dvar_RegisterVariant(
        "demo_usefilesystem",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Used to turn HDD write ON or OFF.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    drawEntityCount = Dvar_RegisterVariant(
        "drawEntityCount",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable entity count drawing");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    drawKillcamData = Dvar_RegisterVariant(
        "drawKillcamData",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable drawing server killcam data");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    drawServerBandwidth = Dvar_RegisterVariant(
        "drawServerBandwidth",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable drawing server bandwidth");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    dynEnt_sentientAutoActivate = Dvar_RegisterVariant(
        "dynEnt_sentientAutoActivate",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "This allows the player and AI to push around dyn ents");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    enable_cheap_ents = Dvar_RegisterVariant(
        "enable_cheap_ents",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Toggles cheap entity support");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    friendlyNameFontObjective = Dvar_RegisterVariant(
        "friendlyNameFontObjective",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Use the objective font for friendly prints.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    fsShowStreamingGraph = Dvar_RegisterVariant(
        "fsShowStreamingGraph",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Shows a graph in-game with streaming history information.");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    fx_cull_effect_spawn = Dvar_RegisterVariant(
        "fx_cull_effect_spawn",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Culls entire effects for spawning");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    fx_cull_elem_draw = Dvar_RegisterVariant(
        "fx_cull_elem_draw",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Culls effect elems for drawing");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    fx_cull_elem_spawn = Dvar_RegisterVariant(
        "fx_cull_elem_spawn",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Culls effect elems for spawning");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    fx_marks = Dvar_RegisterVariant(
        "fx_marks",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Toggles whether bullet hits leave marks");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    fx_marks_ents = Dvar_RegisterVariant(
        "fx_marks_ents",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Toggles whether bullet hits leave marks on entities");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    fx_marks_smodels = Dvar_RegisterVariant(
        "fx_marks_smodels",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Toggles whether bullet hits leave marks on static models");
    v0.integer64 = 0x12C00000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_antilag = Dvar_RegisterVariant(
        "g_antilag",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Turn on antilag checks for weapon hits");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_bDebugRenderBrushes = Dvar_RegisterVariant(
        "g_bDebugRenderBrushes",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render brushes");
    v0.integer64 = 0x12C00000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_bDebugRenderBulletMeshes = Dvar_RegisterVariant(
        "g_bDebugRenderBulletMeshes",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render bullet meshes (point to entity)");
    g.integer64.min = 0x200000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = 0i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_bDebugRenderMeshType = Dvar_RegisterVariant(
        "g_bDebugRenderMeshType",
        DVAR_TYPE_INT,
        0x84000u,
        v0,
        g,
        "Render bullet collision type");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_bDebugRenderColoredPatches = Dvar_RegisterVariant(
        "g_bDebugRenderColoredPatches",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render each patch with a different color (Render Patches must be on )");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_bDebugRenderEntityBrushes = Dvar_RegisterVariant(
        "g_bDebugRenderEntityBrushes",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render the brushes associated with the entity you are looking at");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_bDebugRenderPatches = Dvar_RegisterVariant(
        "g_bDebugRenderPatches",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render patches");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_bDebugRenderStaticModelsBounds = Dvar_RegisterVariant(
        "g_bDebugRenderStaticModelsBounds",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render the bounds of the nearby static models");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_debugRankXP = Dvar_RegisterVariant(
        "g_debugRankXP",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print to the console debug output for rank XP");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_debugWeaponXP = Dvar_RegisterVariant(
        "g_debugWeaponXP",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print to the console debug output for weapon XP");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_deadChat = Dvar_RegisterVariant(
        "g_deadChat",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Allow dead players to chat with living players");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_debugRenderGjkTraceGeom = Dvar_RegisterVariant(
        "g_debugRenderGjkTraceGeom",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render gjk trace meshes");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_droppedWeaponPhysics = Dvar_RegisterVariant(
        "g_droppedWeaponPhysics",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Dropped weapons will g into rigid body physics if set");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_dumpStaticModels = Dvar_RegisterVariant(
        "g_dumpStaticModels",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Prints info about the static models");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_oldAnimCmdNetwork = Dvar_RegisterVariant(
        "g_oldAnimCmdNetwork",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Whether to use the old method of culling animCmds in the network layer");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    g_smoothClients = Dvar_RegisterVariant(
        "g_smoothClients",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable extrapolation between client states");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    gump_verbose = Dvar_RegisterVariant(
        "gump_verbose",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Turns on verbose mode for the gump functionality");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    loc_translate = Dvar_RegisterVariant(
        "loc_translate",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable translations");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    lui_enabled = Dvar_RegisterVariant("lui_enabled", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "Enables LUI");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    lui_highlightScaledText = Dvar_RegisterVariant(
        "lui_highlightScaledText",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Highlights scaled text");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    missileDebugAttractors = Dvar_RegisterVariant(
        "missileDebugAttractors",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw the attractors and repulsors.  Attractors are green, and repulsors are yellow.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    missileDebugDraw = Dvar_RegisterVariant(
        "missileDebugDraw",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw guided missile trajectories.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    missileDebugText = Dvar_RegisterVariant(
        "missileDebugText",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print debug missile info to console.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    modPrvUseAnimDump = Dvar_RegisterVariant(
        "modPrvUseAnimDump",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        nullptr);
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    msg_dumpEnts = Dvar_RegisterVariant(
        "msg_dumpEnts",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print snapshot entity info");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    msg_hudelemspew = Dvar_RegisterVariant(
        "msg_hudelemspew",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Debug hudelem fields changing");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    msg_printEntityNums = Dvar_RegisterVariant(
        "msg_printEntityNums",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print entity numbers");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    msg_zlibCompress = Dvar_RegisterVariant(
        "msg_zlibCompress",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable zlib compression");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    msg_zlibCompressOutput = Dvar_RegisterVariant(
        "msg_zlibCompressOutput",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable zlib compression console output");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    net_broadcast = Dvar_RegisterVariant(
        "net_broadcast",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        nullptr);
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    net_debug_server_time_error = Dvar_RegisterVariant(
        "net_debug_server_time_error",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Debug server time error");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    net_minigraph = Dvar_RegisterVariant(
        "net_minigraph",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Show the small network profile display");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    noCheapSpawns = Dvar_RegisterVariant(
        "noCheapSpawns",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        nullptr);
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    packetDebug = Dvar_RegisterVariant(
        "packetDebug",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable packet debugging information");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    party_reliableMigrate = Dvar_RegisterVariant(
        "party_reliableMigrate",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Use reliable lobby migration");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_buoyancyFastComputation = Dvar_RegisterVariant(
        "phys_buoyancyFastComputation",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Simplify the buoyancy calculations");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_debugBigQueries = Dvar_RegisterVariant(
        "phys_debugBigQueries",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw debug info when phys object moves very fast therefore generating hug query boxes");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_debugCallback = Dvar_RegisterVariant(
        "phys_debugCallback",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enbles the physics debug callback");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_debugDangerousRigidBodies = Dvar_RegisterVariant(
        "phys_debugDangerousRigidBodies",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draws debug line off the origins of dangerous rigid bodies");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_debugExpensivePushout = Dvar_RegisterVariant(
        "phys_debugExpensivePushout",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw debug info when the engine resort to exensive Player/AI pushouts");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_drawCollisionObj = Dvar_RegisterVariant(
        "phys_drawCollisionObj",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Debug draw collision geometry for each physics object");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_drawNitrousVehicle = Dvar_RegisterVariant(
        "phys_drawNitrousVehicle",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print info about the nitrous vehicles");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_drawNitrousVehicleEffects = Dvar_RegisterVariant(
        "phys_drawNitrousVehicleEffects",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Print info about the nitrous vehicle wheel effects");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_drawcontacts = Dvar_RegisterVariant(
        "phys_drawcontacts",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Debug draw contact points");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_entityCollision = Dvar_RegisterVariant(
        "phys_entityCollision",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable to turn on entity collision.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_fluid = Dvar_RegisterVariant(
        "phys_fluid",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Turn on and off the fluid simulation code");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_impact_render = Dvar_RegisterVariant(
        "phys_impact_render",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render a star at the impact origin.");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_player_step_on_actors = Dvar_RegisterVariant(
        "phys_player_step_on_actors",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "phys_player_step_on_actors");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_userRigidBodies = Dvar_RegisterVariant(
        "phys_userRigidBodies",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enables user rigid bodies.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    phys_verbose = Dvar_RegisterVariant(
        "phys_verbose",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Turns on verbose mode");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    player_disableWeaponsInWater = Dvar_RegisterVariant(
        "player_disableWeaponsInWater",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Disable weapons while in deep water");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    player_meleeCOD4ChargeMovement = Dvar_RegisterVariant(
        "player_meleeCOD4ChargeMovement",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Use melee charg movement and friction done in COD4");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    player_useSlopeAnims = Dvar_RegisterVariant(
        "player_useSlopeAnims",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "whether to use slope run animations for the player");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    profile_reset = Dvar_RegisterVariant(
        "profile_reset",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        nullptr);
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ps3_syscacheEnabled = Dvar_RegisterVariant(
        "ps3_syscacheEnabled",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        nullptr);
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_cacheModelLighting = Dvar_RegisterVariant(
        "r_cacheModelLighting",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Speed up model lighting by caching previous results");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_cache_shadowable_lights = Dvar_RegisterVariant(
        "r_cache_shadowable_lights",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Cache shadowable lights optimization");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_cmdbuf_worker = Dvar_RegisterVariant(
        "r_cmdbuf_worker",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Process command buffer in a separate thread");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_disableLightGridSuppresion = Dvar_RegisterVariant(
        "r_disableLightGridSuppresion",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Disable run-time collision trace from suppressing light grid entries");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_flameFX_enable = Dvar_RegisterVariant(
        "r_flameFX_enable",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable the flamethrower effect");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_fxaa = Dvar_RegisterVariant("r_fxaa", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "fxaa");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_genericFilter_enable = Dvar_RegisterVariant(
        "r_genericFilter_enable",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable the genericFilter material");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_portalBevelsOnly = Dvar_RegisterVariant(
        "r_portalBevelsOnly",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Use screen-space bounding box of portals rather than the actual shape of the portal projected onto the screen");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_setFrameBufferAlpha = Dvar_RegisterVariant(
        "r_setFrameBufferAlpha",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Sets the rendered pixels' alpha to 255 and the background to 0");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_skinCache = Dvar_RegisterVariant(
        "r_skinCache",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable cache for vertices of animated models");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_smp_backend = Dvar_RegisterVariant(
        "r_smp_backend",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Process renderer back end in a separate thread");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_stereo3DShadowOpt = Dvar_RegisterVariant(
        "r_stereo3DShadowOpt",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Shadow opt toggle");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_streamAssertHashes = Dvar_RegisterVariant(
        "r_streamAssertHashes",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        nullptr);
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_streamAllowLargeImages = Dvar_RegisterVariant(
        "r_streamAllowLargeImages",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Allow images larger than 8mb to be streamed (used by LMG)");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_streamReflectionProbes = Dvar_RegisterVariant(
        "r_streamReflectionProbes",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Set true dynamically stream reflection probes");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_superFlare_debug = Dvar_RegisterVariant(
        "r_superFlare_debug",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable the superFlare debug occlusion circles");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_superFlare_enable = Dvar_RegisterVariant(
        "r_superFlare_enable",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable the superFlare effect");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_swrk_override_enable = Dvar_RegisterVariant(
        "r_swrk_override_enable",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable shaderworks overrides");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ragdoll_dump_anims = Dvar_RegisterVariant(
        "ragdoll_dump_anims",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Dump animation data when ragdoll fails");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    ragdoll_enable = Dvar_RegisterVariant(
        "ragdoll_enable",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Turn on ragdoll death animations");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    rb_drawCullWorkerDebugText = Dvar_RegisterVariant(
        "rb_drawCullWorkerDebugText",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw debug text with culling statistics");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    rb_drawSonarDebugText = Dvar_RegisterVariant(
        "rb_drawSonarDebugText",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Draw the word 'SONAR' on the screen when sonar is used.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    render_actor_collision = Dvar_RegisterVariant(
        "render_actor_collision",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "render actor collision");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    render_player_collision = Dvar_RegisterVariant(
        "render_player_collision",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "render player collision");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    render_script_movers = Dvar_RegisterVariant(
        "render_script_movers",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render script movers.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    render_fake_ents = Dvar_RegisterVariant(
        "render_fake_ents",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Render fake ents.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    riotshield_debug = Dvar_RegisterVariant(
        "riotshield_debug",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enables riotshield debug info");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    scr_skip_devblock = Dvar_RegisterVariant(
        "scr_skip_devblock",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "turn devblock skipping on/off");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    show_reticle_during_swimming = Dvar_RegisterVariant(
        "show_reticle_during_swimming",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Controls if the reticle is visible during swimming.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    showdrop = Dvar_RegisterVariant("showdrop", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "Show dropped packets");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    showevents = Dvar_RegisterVariant("showevents", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, nullptr);
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_disable_ducks = Dvar_RegisterVariant(
        "snd_disable_ducks",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "disable all currently applied ducks");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_dsp_futz = Dvar_RegisterVariant("snd_dsp_futz", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "enable dsp futz");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_enable_pitch = Dvar_RegisterVariant(
        "snd_enable_pitch",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "use pitch variation");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_force_pfutz = Dvar_RegisterVariant(
        "snd_force_pfutz",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "force alias with pfutz to use futz effect at any distance");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_losOcclusion = Dvar_RegisterVariant(
        "snd_losOcclusion",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "occlusion based off of LOS");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_solo_streams = Dvar_RegisterVariant(
        "snd_solo_streams",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "play all streams at regular volume");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_solo_loaded = Dvar_RegisterVariant(
        "snd_solo_loaded",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "play all RAM sounds at regular volume");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_skip_muted_sounds = Dvar_RegisterVariant(
        "snd_skip_muted_sounds",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "skip muted sounds");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_stress = Dvar_RegisterVariant("snd_stress", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "stress");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_SnapshotPeek = Dvar_RegisterVariant(
        "sv_SnapshotPeek",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "If true, server can peek over the wall in sp.");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_archiveClientsPositions = Dvar_RegisterVariant(
        "sv_archiveClientsPositions",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Archive the client positions to speed up SV_GetClientPositionsAtTime");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_botsPressAttackBtn = Dvar_RegisterVariant(
        "sv_botsPressAttackBtn",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Allow testclients to press attack button");
    v0.integer64 = 0x200000001i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_clientArchive = Dvar_RegisterVariant(
        "sv_clientArchive",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Have the clients archive data to save bandwidth on the server");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_clientFrameRateFix = Dvar_RegisterVariant(
        "sv_clientFrameRateFix",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Smooth the client frame rate when server becomes overloaded");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_debugRate = Dvar_RegisterVariant(
        "sv_debugRate",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable snapshot rate debugging info");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_debugReliableCmds = Dvar_RegisterVariant(
        "sv_debugReliableCmds",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable debugging information for 'reliable' commands");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_packet_info = Dvar_RegisterVariant(
        "sv_packet_info",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable packet info debugging information");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_printMessageSize = Dvar_RegisterVariant(
        "sv_printMessageSize",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "print out size of client messages");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_showAverageBPS = Dvar_RegisterVariant(
        "sv_showAverageBPS",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Show averag bytes per second for net debugging");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_showPingSpam = Dvar_RegisterVariant(
        "sv_showPingSpam",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Turns on ping info spam.");
    v0.integer64 = 0x200000000i64;
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000.0;
    sv_useAnimCulling = Dvar_RegisterVariant(
        "sv_useAnimCulling",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Toggle for anim command culling based on frustum/dist");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = 80.0;
    *((__int64*)&v0.string + 1) = 0i64;
    sv_fx_blockMinDist = Dvar_RegisterVariant(
        "sv_fx_blockMinDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "FX greater than this distance will block sight traces");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    tracer_explosiveOverride = Dvar_RegisterVariant(
        "tracer_explosiveOverride",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "When turned on, will apply an override to the tracer setting when shooting explosive bullets.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    tracer_stoppingPowerOverride = Dvar_RegisterVariant(
        "tracer_stoppingPowerOverride",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "When turned on, will apply an override to the tracer setting when the stopping power"
        " perk is active.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    turret_placement_debug = Dvar_RegisterVariant(
        "turret_placement_debug",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enables turret placement debug lines");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    ui_cinematicsTimestamp = Dvar_RegisterVariant(
        "ui_cinematicsTimestamp",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Shows cinematics timestamp on subtitle UI elements.");
    LODWORD(v814) = 1;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    ui_drawCrosshair = Dvar_RegisterVariant(
        "ui_drawCrosshair",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Whether to draw crosshairs.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    ui_fxFontTweaks = Dvar_RegisterVariant(
        "ui_fxFontTweaks",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable tweaks of the font params");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    ui_showList = Dvar_RegisterVariant(
        "ui_showList",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Show onscreen list of currently visible menus");
    LODWORD(v814) = 1;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    useFastFile = Dvar_RegisterVariant(
        "useFastFile",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enables loading data from fast files.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    vehControlMode = Dvar_RegisterVariant(
        "vehControlMode",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Changes the vehicle controls and camera mode");
    LODWORD(v814) = 1;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    vehHelicopterAlwaysFaceCamera = Dvar_RegisterVariant(
        "vehHelicopterAlwaysFaceCamera",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Setting this will have the helicopter always face the same direction as the camera.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    vehHelicopterHeadSwayDontSwayTheTurret = Dvar_RegisterVariant(
        "vehHelicopterHeadSwayDontSwayTheTurret",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "If set, the turret will not fire throug the crosshairs, but straight ahead"
        " of the vehicle, when the player is not freelooking.");
    LODWORD(v814) = 1;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    vehHudDrawPipOnStickWhenFreelooking = Dvar_RegisterVariant(
        "vehHudDrawPipOnStickWhenFreelooking",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Set to 0 to not draw the pip-on-a-stick reticle when the player is freelooking");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    vehPlaneInvertPitch = Dvar_RegisterVariant(
        "vehPlaneInvertPitch",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Setting this will invert pitch control.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    vehPlaneJetControls = Dvar_RegisterVariant(
        "vehPlaneJetControls",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Setting this will have the plane use jet controls.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    voiceDegrade = Dvar_RegisterVariant(
        "voiceDegrade",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Degrade voice quality");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    waypointDebugDraw = Dvar_RegisterVariant(
        "waypointDebugDraw",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        nullptr);
    LODWORD(v814) = 1;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    xbox_dwMatchmaking = Dvar_RegisterVariant(
        "xbox_dwMatchmaking",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "ON/OFF switch for DW matchmaking service on the Xbox");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    xenon_voiceDebug = Dvar_RegisterVariant(
        "xenon_voiceDebug",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Debug voice communication");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    useForceEliteCmds = Dvar_RegisterVariant(
        "useForceEliteCmds",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "allow forceElite* commands below to dictate subscription status.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    forceElitePopups = Dvar_RegisterVariant(
        "forceElitePopups",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "all Elite related popups will show regardless of state.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    forceEliteSubscription = Dvar_RegisterVariant(
        "forceEliteSubscription",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "gives valid Elite subscription, enabling CAC import checking and other features.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    forceEliteFounder = Dvar_RegisterVariant(
        "forceEliteFounder",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "gives founder status");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    grenadeUseRigidBodyPhysics = Dvar_RegisterVariant(
        "grenadeUseRigidBodyPhysics",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Grenades will use rigid body physics if this is turned on.  Used on moving platforms.");
    LODWORD(v814) = 0;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    net_logSnapshotTiming = Dvar_RegisterVariant(
        "net_logSnapshotTiming",
        DVAR_TYPE_BOOL,
        0x84000u,
        v0,
        emptyLimit,
        "Enable blackbox logging of snapshot timing using NTP");
    ui_playerNemesisColor = _Dvar_RegisterColor(
        "ui_playerNemesisColor",
        1.0,
        0.315,
        0.34999999,
        0.5,
        0x84000u,
        "Nemesis background colour in lobbies");
    LODWORD(v814) = 1;
    ui_playerSquadColor = _Dvar_RegisterColor(
        "ui_playerSquadColor",
        0.315,
        0.34999999,
        1.0,
        0.5,
        0x84000u,
        "Squad Member background colour in lobbies");
    v812 = emptyValue;
    v0.integer64 = v814;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 250.0;
    sm_enable = Dvar_RegisterVariant("sm_enable", DVAR_TYPE_BOOL, 0x84000u, v0, emptyLimit, "Enable shadow mapping");
    g.integer64.min = (unsigned int)10.0 | 0x461C400000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.30000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 10.0;
    ai_accu_player_lateral_speed = Dvar_RegisterVariant(
        "ai_accu_player_lateral_speed",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The max speed at which the player moves to cause the AI accuracy to g to 0");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.66000003;
    LODWORD(v814) = 0;
    bg_afterLifeGravityScale = Dvar_RegisterVariant(
        "bg_afterLifeGravityScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Zombies DLC2 afterlife gravity scale");
    *((float*)&v814 + 1) = 3.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.16;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    bg_maxWeaponAnimScale = Dvar_RegisterVariant(
        "bg_maxWeaponAnimScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The amount which weapon timers can cause player animations to scale their playback speed by");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.125;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    bg_weaponBobAmplitudeBase = Dvar_RegisterVariant(
        "bg_weaponBobAmplitudeBase",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The base speed-based weapon bob amplitude");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    LODWORD(v814) = 0;
    cg_cameraVzoomToggleTime = Dvar_RegisterVariant(
        "cg_cameraVzoomToggleTime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Time in seconds to transition between fovs in vzoom weapon");
    HIDWORD(v814) = 10.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 10.0;
    cg_deathScreenFadeInTime = Dvar_RegisterVariant(
        "cg_deathScreenFadeInTime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Time in seconds to transition to the death vision set upon player death");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 90.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 180.0;
    cg_deathScreenFadeOutTime = Dvar_RegisterVariant(
        "cg_deathScreenFadeOutTime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Time in seconds to fade to black after player death");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 100.0;
    cg_debugEllipsesOffset = Dvar_RegisterVariant(
        "cg_debugEllipsesOffset",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "amount to rotate the line segments making the debug ellipses smooth.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 50.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 512;
    cg_drawFPSScale = Dvar_RegisterVariant("cg_drawFPSScale", DVAR_TYPE_FLOAT, 0x84000u, v0, g, "Draw FPS size scale");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 5.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 512;
    cg_drawVersionX = Dvar_RegisterVariant(
        "cg_drawVersionX",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "X offset for the version string");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 100.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cg_drawVersionY = Dvar_RegisterVariant(
        "cg_drawVersionY",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Y offset for the version string");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 85.0;
    cg_errorDecay = Dvar_RegisterVariant(
        "cg_errorDecay",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Decay for predicted error");
    g.integer64.min = __PAIR64__(160.0, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 65.0;
    cg_fov_comp_max = Dvar_RegisterVariant(
        "cg_fov_comp_max",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The maximum field of view angle(in degrees) to compensate for gun placement");
    g.integer64.min = __PAIR64__(90.0, 65.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 40.0;
    cg_fov_default = Dvar_RegisterVariant(
        "cg_fov_default",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "User default field of view angle in degrees");
    g.integer64.min = __PAIR64__(160.0, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 65.0;
    cg_fov_default_thirdperson = Dvar_RegisterVariant(
        "cg_fov_default_thirdperson",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "User default 3rd person field of view angle in degrees");
    g.integer64.min = __PAIR64__(80.0, 65.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -2.0;
    cg_fov_default_nocomp = Dvar_RegisterVariant(
        "cg_fov_default_nocomp",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The default field of view angle(in degrees) for which there is no need to compensate for gun placement");
    g.integer64.min = __PAIR64__(3.4028235e38, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    cg_gun_fovcomp_x = Dvar_RegisterVariant(
        "cg_gun_fovcomp_x",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "x position FOV offset compensation of the viewmodel");
    g.integer64.min = __PAIR64__(3.4028235e38, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    cg_gun_fovcomp_y = Dvar_RegisterVariant(
        "cg_gun_fovcomp_y",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "y position FOV offset compensation of the viewmodel");
    g.integer64.min = __PAIR64__(3.4028235e38, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.3;
    cg_gun_fovcomp_z = Dvar_RegisterVariant(
        "cg_gun_fovcomp_z",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "z position FOV offset compensation of the viewmodel");
    g.integer64.min = __PAIR64__(3.0, 0.5);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 64.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 512;
    cg_gamemessageiconscale = Dvar_RegisterVariant(
        "cg_gamemessageiconscale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale value to increase/decrease the size of the icons in the game messages window which i"
        "s used by obiturary etc.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 128.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 512;
    cg_hudDamageIconHeight = Dvar_RegisterVariant(
        "cg_hudDamageIconHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The height of the damag icon");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_hudDamageIconOffset = Dvar_RegisterVariant(
        "cg_hudDamageIconOffset",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The offset from the center of the damag icon");
    v812.value = 128.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 512;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 24.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 640.0;
    cg_hudDamageIconWidth = Dvar_RegisterVariant(
        "cg_hudDamageIconWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The width of the damag icon");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 24.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 640.0;
    cg_hudFriendlyIndicatorHeight = Dvar_RegisterVariant(
        "cg_hudFriendlyIndicatorHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The height of offscreen friendly icons");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 104.0;
    LODWORD(v814) = 0;
    cg_hudFriendlyIndicatorWidth = Dvar_RegisterVariant(
        "cg_hudFriendlyIndicatorWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The width of offscreen friendly icons");
    *((float*)&v814 + 1) = 1000.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.7;
    cg_hudGrenadeIconMaxHeight = Dvar_RegisterVariant(
        "cg_hudGrenadeIconMaxHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The minimum height difference between a player and a grenade for the grenade to be show"
        "n on the grenade indicator");
    g.integer64.min = __PAIR64__(50.0, 0.1);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.85;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.0;
    cg_hudGrenadePointerPulseFreq = Dvar_RegisterVariant(
        "cg_hudGrenadePointerPulseFreq",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The number of times per second that the grenade indicator flashes in Hertz");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.30000001;
    cg_hudGrenadePointerPulseMax = Dvar_RegisterVariant(
        "cg_hudGrenadePointerPulseMax",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The maximum alpha of the grenade indicator pulse. Values higher than 1 will cause the"
        " indicator to remain at full brightness for longer");
    g.integer64.min = __PAIR64__(1.0, -3.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    cg_hudGrenadePointerPulseMin = Dvar_RegisterVariant(
        "cg_hudGrenadePointerPulseMin",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The minimum alpha of the grenade indicator pulse. Values lower than 0 will cause the "
        "indicator to remain at full transparency for longer");
    g.integer64.min = __PAIR64__(3.4028235e38, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -160.0;
    cg_hudLegacySplitscreenScale = Dvar_RegisterVariant(
        "cg_hudLegacySplitscreenScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Screen scale for hud elements in splitscreen");
    g.integer64.min = __PAIR64__(-10000.0, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.4;
    cg_hudProneY = Dvar_RegisterVariant(
        "cg_hudProneY",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Virtual screen y coordinate of the prone blocked message");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.0000099999997);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.5;
    cg_hudSplitscreenBannerScoreboardScale = Dvar_RegisterVariant(
        "cg_hudSplitscreenBannerScoreboardScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale value to apply to the splitscreen banner scoreboard");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.000099999997);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    cg_hudSplitscreenCompassElementScale = Dvar_RegisterVariant(
        "cg_hudSplitscreenCompassElementScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale value to apply to compass elements in splitscreen");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.000099999997);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    cg_hudSplitscreenCompassScale = Dvar_RegisterVariant(
        "cg_hudSplitscreenCompassScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale value to apply to the compass in splitscreen");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.0000099999997);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    cg_hudSplitscreenScoreboardScale = Dvar_RegisterVariant(
        "cg_hudSplitscreenScoreboardScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale value to apply to the scoreboard in splitscreen");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.000099999997);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 200.0;
    cg_hudSplitscreenStanceScale = Dvar_RegisterVariant(
        "cg_hudSplitscreenStanceScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale value to apply to the stance HUD element in splitscreen");
    g.integer64.min = __PAIR64__(1000.0, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.60000002;
    cg_infraredHighlightScale = Dvar_RegisterVariant(
        "cg_infraredHighlightScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale of the player highlight when using infrared scope");
    g.integer64.min = __PAIR64__(1.0, 0.001);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 128.0;
    LODWORD(v814) = 0;
    cg_mapLocationSelectionCursorSpeed = Dvar_RegisterVariant(
        "cg_mapLocationSelectionCursorSpeed",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Speed of the cursor when selecting a location on the map");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.98000002;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    cg_maxExplosionRadius = Dvar_RegisterVariant(
        "cg_maxExplosionRadius",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Maximum radius of client-side explosions");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 24.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 100.0;
    cg_playersInViewMinDot = Dvar_RegisterVariant(
        "cg_playersInViewMinDot",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The minimum cos(angle) value above which we want the player overhead names to marquee");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 8.0;
    cg_rangefinderIndicatorSize = Dvar_RegisterVariant(
        "cg_rangefinderIndicatorSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Size of each of the corners of the player indicator");
    g.integer64.min = __PAIR64__(100.0, -100.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.69999999;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    cg_rangefinderIndicatorGap = Dvar_RegisterVariant(
        "cg_rangefinderIndicatorGap",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Minimum gap between the corners of the player indicator");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.035999998;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    cg_scoreboardPingHeight = Dvar_RegisterVariant(
        "cg_scoreboardPingHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Height of the ping graph as a % of the scoreboard row height");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 395.0;
    LODWORD(v814) = 0;
    cg_scoreboardPingWidth = Dvar_RegisterVariant(
        "cg_scoreboardPingWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Width of the ping graph as a % of the scoreboard");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 700.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cg_scoreboardQuarterscreenWidth = Dvar_RegisterVariant(
        "cg_scoreboardQuarterscreenWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Width of the scoreboard for a quarter of the screen splitscreen");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 500;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cg_scoreboardSplitscreenWidth = Dvar_RegisterVariant(
        "cg_scoreboardSplitscreenWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Width of the scoreboard in splitscreen");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.1;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 0.5;
    cg_scoreboardWidth = Dvar_RegisterVariant(
        "cg_scoreboardWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Width of the scoreboard");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.30000001;
    cg_splitscreenLetterboxSize = Dvar_RegisterVariant(
        "cg_splitscreenLetterboxSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Amount of border to leave at the side edges of the screen in 2 & 3 was split screen.");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.000099999997);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 3.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cg_splitscreenSpectatorScaleIncrease = Dvar_RegisterVariant(
        "cg_splitscreenSpectatorScaleIncrease",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale value to apply to the spectator messag in splitscreen");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 100.0;
    cg_subtitleMinTime = Dvar_RegisterVariant(
        "cg_subtitleMinTime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The minimum time that the subtitles are displayed on screen in seconds");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_voiceIconSize = Dvar_RegisterVariant(
        "cg_voiceIconSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Size of the 'voice' icon");
    cg_sensorGrenadePulsePeriodMs = _Dvar_RegisterInt(
        "cg_sensorGrenadePulsePeriodMs",
        800,
        0,
        0x7FFFFFFF,
        0x84000u,
        "How often (ms) sensor grenade pulses to trigger its targeting lasers (LASERS!)");
    v812.value = 1000.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cg_sensorGrenadeLaserActiveTimeMs = _Dvar_RegisterInt(
        "cg_sensorGrenadeLaserActiveTimeMs",
        400,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Duration (ms) for lasers to draw after a pulse");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 35.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cg_sensorGrenadeRange = Dvar_RegisterVariant(
        "cg_sensorGrenadeRange",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Rang of the sensor grenade laser");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 25.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cg_sensorGrenadeZOffset = Dvar_RegisterVariant(
        "cg_sensorGrenadeZOffset",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Z offset for sensor grenade trace");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 5.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cg_sensorGrenadeCrouchZOffset = Dvar_RegisterVariant(
        "cg_sensorGrenadeCrouchZOffset",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Z offset for sensor grenade trace to use when the target is crouched");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.4;
    cg_sensorGrenadeProneZOffset = Dvar_RegisterVariant(
        "cg_sensorGrenadeProneZOffset",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Z offset for sensor grenade trace to use when the target is prone");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.001);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    cg_sensorGrenadeLaserRadius = Dvar_RegisterVariant(
        "cg_sensorGrenadeLaserRadius",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Radius for sensor grenade laser");
    cg_sensorGrenadeRampAlpha = _Dvar_RegisterBool(
        "cg_sensorGrenadeRampAlpha",
        0,
        0x84000u,
        "Sensor grenade laser alpha fades over time.");
    v812.value = 0.80000001;
    cg_sensorGrenadeExplosionTrackTimeMs = _Dvar_RegisterInt(
        "cg_sensorGrenadeExplosionTrackTimeMs",
        6000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Time to satellite track the players tagged by a sensor grenade explosion.");
    g.integer64.min = __PAIR64__(1.0, 0.000099999997);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.5;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cl_analog_attack_threshold = Dvar_RegisterVariant(
        "cl_analog_attack_threshold",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The threshold before firing");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 140.0;
    cl_anglespeedkey = Dvar_RegisterVariant(
        "cl_anglespeedkey",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Multiplier for max angle speed for game pad and keyboard");
    g.integer64.min = __PAIR64__(3.4028235e38, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.15000001;
    LODWORD(v814) = 0;
    cl_pitchspeed = Dvar_RegisterVariant(
        "cl_pitchspeed",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Max pitch speed in degrees for game pad");
    *((float*)&v814 + 1) = 100.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 140.0;
    cl_viewport_lerptime = Dvar_RegisterVariant(
        "cl_viewport_lerptime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "viewport expansion/contract lerp time");
    g.integer64.min = __PAIR64__(3.4028235e38, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 37.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    cl_yawspeed = Dvar_RegisterVariant(
        "cl_yawspeed",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Max yaw speed in degrees for game pad and keyboard");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 18.75;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassECoordCutoff = Dvar_RegisterVariant(
        "compassECoordCutoff",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Left cutoff for the scrolling east-west coords");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 18.75;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassFriendlyHeight = Dvar_RegisterVariant(
        "compassFriendlyHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the friendly icon on the compass");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 20.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassFriendlyWidth = Dvar_RegisterVariant(
        "compassFriendlyWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the friendly icon on the compass");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 20.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassObituaryHeight = Dvar_RegisterVariant(
        "compassObituaryHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the obituary icon on the compass");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 40.0;
    compassObituaryWidth = Dvar_RegisterVariant(
        "compassObituaryWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the obituary icon on the compass");
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 40.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassIconOtherVehHeight = Dvar_RegisterVariant(
        "compassIconOtherVehHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        nullptr);
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 25.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassIconOtherVehWidth = Dvar_RegisterVariant(
        "compassIconOtherVehWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        nullptr);
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 25.0;
    LODWORD(v814) = 0;
    compassIconTankHeight = Dvar_RegisterVariant(
        "compassIconTankHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        nullptr);
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.89999998;
    compassIconTankWidth = Dvar_RegisterVariant(
        "compassIconTankWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        nullptr);
    g.integer64.min = __PAIR64__(2.0, 0.0099999998);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 16.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassIconTankScale = Dvar_RegisterVariant(
        "compassIconTankScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale value applied to the compass tank icon dimensions in MP");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 16.0;
    LODWORD(v814) = 0;
    compassObjectiveIconHeightZombie = Dvar_RegisterVariant(
        "compassObjectiveIconHeightZombie",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the objective on the full map");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2048.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassObjectiveIconWidthZombie = Dvar_RegisterVariant(
        "compassObjectiveIconWidthZombie",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the objective on the full map");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    HIDWORD(v814) = 1.0;
    compassObjectiveMaxRange = Dvar_RegisterVariant(
        "compassObjectiveMaxRange",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The maximum rang at which an objective is visible on the compass");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 80.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassObjectiveMinAlpha = Dvar_RegisterVariant(
        "compassObjectiveMinAlpha",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The minimum alpha for an objective at the edg of the compass");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassObjectiveRingSize = Dvar_RegisterVariant(
        "compassObjectiveRingSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The maximum objective ring sig when a new objective appears on the compass");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    compassSize = Dvar_RegisterVariant("compassSize", DVAR_TYPE_FLOAT, 0x84000u, v0, g, "Scale the compass");
    g.integer64.min = __PAIR64__(1.0, 0.0099999998);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.4;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    compassTickertapeStretch = Dvar_RegisterVariant(
        "compassTickertapeStretch",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "How far the tickertape should stretch from its center.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 8.0;
    con_MiniConSplitscreenScale = Dvar_RegisterVariant(
        "con_MiniConSplitscreenScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Splitscreen scale value for the mini console");
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 4.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    con_errormessagetime = Dvar_RegisterVariant(
        "con_errormessagetime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Onscreen time for error messages in seconds");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 32.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    con_minicontime = Dvar_RegisterVariant(
        "con_minicontime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Onscreen time for minicon messages in seconds");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 32.0;
    LODWORD(v814) = 0;
    drawKillcamDataSize = Dvar_RegisterVariant(
        "drawKillcamDataSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "How big to draw the killcam data graph");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1250.0;
    drawServerBandwidthSize = Dvar_RegisterVariant(
        "drawServerBandwidthSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "How big to draw the entity count graph");
    g.integer64.min = (unsigned int)10.0 | 0x47C3500000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.40000001;
    facepaintLodDist = Dvar_RegisterVariant(
        "facepaintLodDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance to stop drawing the player facepaint.");
    g.integer64.min = __PAIR64__(100.0, 0.0099999998);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    LODWORD(v814) = 0;
    friendlyNameSplitScreenFontSize = Dvar_RegisterVariant(
        "friendlyNameSplitScreenFontSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Fontsize of the popup friendly names in splitscreen.");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    fudgefactor = Dvar_RegisterVariant("fudgefactor", DVAR_TYPE_FLOAT, 0x84000u, v0, g, nullptr);
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 80.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    g_changelevel_time = Dvar_RegisterVariant(
        "g_changelevel_time",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Time for chang level fade out");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = (int)10.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000.0;
    g_clonePlayerMaxVelocity = Dvar_RegisterVariant(
        "g_clonePlayerMaxVelocity",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Maximum velocity in each axis of a cloned player\n(for death animations)");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 100.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000.0;
    g_dropForwardSpeed = Dvar_RegisterVariant(
        "g_dropForwardSpeed",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Forward speed of a dropped item");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = (int)10.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000.0;
    g_dropHorzSpeedRand = Dvar_RegisterVariant(
        "g_dropHorzSpeedRand",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Random component of the initial horizontal speed of a dropped item");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 5.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000.0;
    g_dropUpSpeedBase = Dvar_RegisterVariant(
        "g_dropUpSpeedBase",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Base component of the initial vertical speed of a dropped item");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 25.0;
    LODWORD(v814) = 0;
    g_dropUpSpeedRand = Dvar_RegisterVariant(
        "g_dropUpSpeedRand",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Random component of the initial vertical speed of a dropped item");
    *((float*)&v814 + 1) = 32000.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    g_playerCollisionEjectSpeed = Dvar_RegisterVariant(
        "g_playerCollisionEjectSpeed",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Speed at which to push intersecting players away from each other");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 4.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 100.0;
    g_voteAbstainWeight = Dvar_RegisterVariant(
        "g_voteAbstainWeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "How much an abstained vote counts as a 'no' vote");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 4.0;
    LODWORD(v814) = 0;
    glassExtraAngVelocity = Dvar_RegisterVariant(
        "glassExtraAngVelocity",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Additional random extra angular velocity to the shards when shattering.");
    *((float*)&v814 + 1) = 100.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 10.0;
    glassExtraLinVelocity = Dvar_RegisterVariant(
        "glassExtraLinVelocity",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Additional random extra linear velocity when cracked glass times out.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    glassForceAttenuation = Dvar_RegisterVariant(
        "glassForceAttenuation",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Attenuation of the force as the shard is farther from the hit position.");
    g.integer64.min = __PAIR64__(100.0, 0.1);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    LODWORD(v814) = 0;
    glassForceMultiplier = Dvar_RegisterVariant(
        "glassForceMultiplier",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Multiplier of the force to apply to shards.");
    HIDWORD(v814) = 10.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 250.0;
    glassForceOriginMult = Dvar_RegisterVariant(
        "glassForceOriginMult",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Multiplier of the distance of the origin of the shards spread point - the smaller it is, clos"
        "er to the glass then the shards will spread to the sides. The farther it is the shards will f"
        "ly in the same direction.");
    g.integer64.min = 0x461C400000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 450.0;
    glassLinVelMultiplier = Dvar_RegisterVariant(
        "glassLinVelMultiplier",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Multiplier of the linear velocity to apply to shards when breaking.");
    g.integer64.min = (unsigned int)10.0 | 0x47C3500000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 3.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000.0;
    glassLowLodDist = Dvar_RegisterVariant(
        "glassLowLodDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Adjusted (by fov) distance to switch to low glass lod");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 30.0;
    glassMaxShardLife = Dvar_RegisterVariant(
        "glassMaxShardLife",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Maximum life time of a shard that is contolled by the glass physics");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 30.0;
    hud_fade_compass = Dvar_RegisterVariant(
        "hud_fade_compass",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The time for the compass to fade in seconds");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.7;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 30.0;
    hud_fade_healthbar = Dvar_RegisterVariant(
        "hud_fade_healthbar",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The time for the health bar to fade in seconds");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    LODWORD(v814) = 0;
    hud_fade_sprint = Dvar_RegisterVariant(
        "hud_fade_sprint",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The time for the sprint meter to fade in seconds");
    *((float*)&v814 + 1) = 30.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 30.0;
    hud_fade_vehiclecontrols = Dvar_RegisterVariant(
        "hud_fade_vehiclecontrols",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The time for the vehicle controls to fade in seconds");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 30.0;
    hud_flash_period_offhand = Dvar_RegisterVariant(
        "hud_flash_period_offhand",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Offhand weapons flash period on changing weapon");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.050000001;
    LODWORD(v814) = 0;
    hud_flash_time_offhand = Dvar_RegisterVariant(
        "hud_flash_time_offhand",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Offhand weapons flash duration on changing weapon");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 180.0;
    lastStandSwayAccel = Dvar_RegisterVariant(
        "lastStandSwayAccel",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Sway acceleration while in last stand");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.7;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    lastStandSwayMaxDrift = Dvar_RegisterVariant(
        "lastStandSwayMaxDrift",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Max angle from center player will drift while in last stand");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.5;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    lowAmmoWarningPulseFreq = Dvar_RegisterVariant(
        "lowAmmoWarningPulseFreq",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Frequency of the pulse (oscilation between the 2 colors)");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    lowAmmoWarningPulseMax = Dvar_RegisterVariant(
        "lowAmmoWarningPulseMax",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Min of oscilation range: 0 is color1 and 1.0 is color2.  Can be < 0, and the wave will clip at 0.");
    g.integer64.min = __PAIR64__(1.0, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 3.0;
    lowAmmoWarningPulseMin = Dvar_RegisterVariant(
        "lowAmmoWarningPulseMin",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Max of oscilation range: 0 is color1 and 1.0 is color2.  Can be > 1.0, and the wave will clip at 1.0.");
    g.integer64.min = __PAIR64__(1000.0, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 3.0;
    narrowShardRatio = Dvar_RegisterVariant(
        "narrowShardRatio",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "minimum ratio between a shard's local bbox edges or between the shard's bbox area vs the shard's "
        "area to cause the shard to be rejected");
    g.integer64.min = __PAIR64__(100.0, 0.0099999998);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 2.0;
    phys_bulletSpinScale = Dvar_RegisterVariant(
        "phys_bulletSpinScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale of the effective offset from the center of mass for the bullet impacts.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    phys_bulletUpBias = Dvar_RegisterVariant(
        "phys_bulletUpBias",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Up Bias for the direction of the bullet impact.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.029999999;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    phys_dragAngular = Dvar_RegisterVariant(
        "phys_dragAngular",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The amount of angular drag, applied globally");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = (int)10.0;
    LODWORD(v814) = 0;
    phys_dragLinear = Dvar_RegisterVariant(
        "phys_dragLinear",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The amount of linear drag, applied globally");
    *((float*)&v814 + 1) = 50.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = (int)10.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 50.0;
    phys_impact_max_pfx_per_frame = Dvar_RegisterVariant(
        "phys_impact_max_pfx_per_frame",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Maximum number of particle effects spawned on physics object impact. The value is per frame.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.30000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 10.0;
    phys_impact_max_sfx_per_frame = Dvar_RegisterVariant(
        "phys_impact_max_sfx_per_frame",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Maximum number of sound effects spawned on physics objects impact. The value is per frame.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    LODWORD(v814) = 0;
    phys_impact_silence_window = Dvar_RegisterVariant(
        "phys_impact_silence_window",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The length of the silence window. When a body triggers an impact sound it will not trig"
        "ger new sounds for the duration of this silence window.");
    *((float*)&v814 + 1) = 5.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.039999999;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    phys_player_collision_adjust_height = Dvar_RegisterVariant(
        "phys_player_collision_adjust_height",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "adjust the height of the gjk player collision capsule");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    phys_ragdoll_joint_damp_scale = Dvar_RegisterVariant(
        "phys_ragdoll_joint_damp_scale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Sets the phys joint damp for ragdolls.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.1;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 2.0;
    phys_reeval_frequency = Dvar_RegisterVariant(
        "phys_reeval_frequency",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Controls how often the engine reevalutes the piority of the physics object. If the object ha"
        "s low priority /its too far or smth/ we dont play effect.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -2.0;
    phys_vehicleFriction = Dvar_RegisterVariant(
        "phys_vehicleFriction",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Default vehicle friction with the world and other ents.");
    g.integer64.min = -3.4028235e38;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -1.0;
    phys_waterDragAngular = Dvar_RegisterVariant(
        "phys_waterDragAngular",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The amount of angular drag for buoyant objects");
    g.integer64.min = -3.4028235e38;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 20.0;
    phys_waterDragLinear = Dvar_RegisterVariant(
        "phys_waterDragLinear",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The amount of linear drag for buoyant objects");
    g.integer64.min = __PAIR64__(-10000.0, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 100.0;
    player_floatSpeed = Dvar_RegisterVariant(
        "player_floatSpeed",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The speed at which the player will float up to the top of the water");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.7;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 5.0;
    player_turnRateScale = Dvar_RegisterVariant(
        "player_turnRateScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scales the rotation rate of the player's view.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1176256512;
    player_waistWaterFrictionScale = Dvar_RegisterVariant(
        "player_waistWaterFrictionScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scales the player's friction in waist deep water.");
    g.integer64.min = (unsigned int)1.0 | 0x461C400000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_dlightMaxFullScreenRadius = Dvar_RegisterVariant(
        "r_dlightMaxFullScreenRadius",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Maximum radius to limit dlights to in fullscreen.");
    v812.integer = 1176256512;
    g.integer64.min = (unsigned int)1.0 | 0x461C400000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1.25;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    r_dlightMaxNonFullScreenRadius = Dvar_RegisterVariant(
        "r_dlightMaxNonFullScreenRadius",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Maximum radius to limit dlights to when not fullscreen");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.40000001;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    r_fovScaleThresholdRigid = Dvar_RegisterVariant(
        "r_fovScaleThresholdRigid",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Threshold FOV LOD scaling must pass before taking effect for rigid models");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 6500.0;
    r_fovScaleThresholdSkinned = Dvar_RegisterVariant(
        "r_fovScaleThresholdSkinned",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Threshold FOV LOD scaling must pass before taking effect for skinned models");
    g.integer64.min = __PAIR64__(25000.0, 1650.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    HIDWORD(v814) = 1.0;
    r_heroLightColorTemp = Dvar_RegisterVariant(
        "r_heroLightColorTemp",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "hero diffuse light temp");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    HIDWORD(v814) = 1.0;
    r_heroLightSaturation = Dvar_RegisterVariant(
        "r_heroLightSaturation",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "hero diffuse light saturation");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    r_poisonFX_debug_amount = Dvar_RegisterVariant(
        "r_poisonFX_debug_amount",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Poison effect amount");
    g.integer64.min = __PAIR64__(360.0, -360.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    r_poisonFX_dvisionA = Dvar_RegisterVariant(
        "r_poisonFX_dvisionA",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "double vision angle");
    g.integer64.min = __PAIR64__(20.0, -20.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.2;
    r_poisonFX_dvisionY = Dvar_RegisterVariant(
        "r_poisonFX_dvisionY",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "amount of double vision y");
    g.integer64.min = __PAIR64__(0.30000001, -0.30000001);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -0.15000001;
    r_poisonFX_warpX = Dvar_RegisterVariant(
        "r_poisonFX_warpX",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "amount of distortion in x");
    g.integer64.min = __PAIR64__(0.30000001, -0.30000001);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    r_poisonFX_warpY = Dvar_RegisterVariant(
        "r_poisonFX_warpY",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "amount of distortion in y");
    v812.value = -1.0;
    g.integer64.min = -16.0;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -1.0;
    r_polygonOffsetBiasFloat = Dvar_RegisterVariant(
        "r_polygonOffsetBiasFloat",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Offset bias for decal polygons; bigger values z-fight less but poke throug walls more");
    g.integer64.min = -4.0;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.69999999;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    r_polygonOffsetScale = Dvar_RegisterVariant(
        "r_polygonOffsetScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Offset scale for decal polygons; bigger values z-fight less but poke throug walls more");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.02;
    LODWORD(v814) = 0;
    r_portalBevels = Dvar_RegisterVariant(
        "r_portalBevels",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Helps cull geometry by angles of portals that are acute when projected onto the screen, value is th"
        "e cosine of the angle");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 15.0;
    r_portalMinClipArea = Dvar_RegisterVariant(
        "r_portalMinClipArea",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Don't clip child portals by a parent portal smaller than this fraction of the screen area.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 20.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    r_waterSheetingFX_fadeDuration = Dvar_RegisterVariant(
        "r_waterSheetingFX_fadeDuration",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Sets fade duration in seconds");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 50.0;
    LODWORD(v814) = 0;
    riotshield_placement_foffset = Dvar_RegisterVariant(
        "riotshield_placement_foffset",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Forward offset of riotshield placeament.  Scales how far infront of player origin to trace and place");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    riotshield_placement_maxs = Dvar_RegisterVariant(
        "riotshield_placement_maxs",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "maxs of the bound used for the placement trace");
    riotshield_carry_model = _Dvar_RegisterString(
        "riotshield_carry_model",
        "t6_wpn_shield_carry_world",
        0x84000u,
        "Riotshield carry model name");
    v812.value = 0.2;
    riotshield_stowed_model = _Dvar_RegisterString(
        "riotshield_stowed_model",
        "t6_wpn_shield_stow_world",
        0x84000u,
        "Riotshield stowed model name");
    g.integer64.min = __PAIR64__(1.0, 0.0099999998);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 55.0;
    shardShatterSizeLimitScale = Dvar_RegisterVariant(
        "shardShatterSizeLimitScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Scale of maxShardSize & minShardSize when shattering a shard.");
    g.integer64.min = __PAIR64__(1000.0, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 20.0;
    shatterFxMaxDist = Dvar_RegisterVariant(
        "shatterFxMaxDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance between FX spawning points along the window edges");
    g.integer64.min = __PAIR64__(1000.0, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = (int)10.0;
    shatterFxMinEdgeLength = Dvar_RegisterVariant(
        "shatterFxMinEdgeLength",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Minimum length that the window edg has to be to have a shatter fx spawned on it.");
    g.integer64.min = __PAIR64__(1000.0, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 64.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1024.0;
    shrapnelSpanCylinderHeight = Dvar_RegisterVariant(
        "shrapnelSpanCylinderHeight",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Height of the 360 degree span for shrapnel radius explosion type (bouncing betty).");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.125;
    LODWORD(v814) = 0;
    sm_lightScore_eyeProjectDist = Dvar_RegisterVariant(
        "sm_lightScore_eyeProjectDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "When picking shadows for primary lights, measure distance from a point this far in fr"
        "ont of the camera.");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 8.0;
    sm_lightScore_spotProjectFrac = Dvar_RegisterVariant(
        "sm_lightScore_spotProjectFrac",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "When picking shadows for primary lights, measure distance to a point this fraction o"
        "f the light's radius along it's shadow direction.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 25.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 4000.0;
    sm_polygonOffsetScale = Dvar_RegisterVariant(
        "sm_polygonOffsetScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Shadow map offset scale");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 16.0;
    sm_sunPolygonOffsetBias = Dvar_RegisterVariant(
        "sm_sunPolygonOffsetBias",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Sun shadow map offset bias");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 500;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 4000.0;
    sm_sunPolygonOffsetScale = Dvar_RegisterVariant(
        "sm_sunPolygonOffsetScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Sun shadow map offset scale");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 16.0;
    sm_spotPolygonOffsetBias = Dvar_RegisterVariant(
        "sm_spotPolygonOffsetBias",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Spot shadow map offset bias");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 3000.0;
    sm_spotPolygonOffsetScale = Dvar_RegisterVariant(
        "sm_spotPolygonOffsetScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Spot shadow map offset scale");
    g.integer64.min = __PAIR64__(100.0, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_dlpf_max_distance = Dvar_RegisterVariant(
        "snd_dlpf_max_distance",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "max distance for distance lpf");
    v812.value = 800.0;
    g.integer64.min = __PAIR64__(100.0, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -30.0;
    snd_dlpf_min_distance = Dvar_RegisterVariant(
        "snd_dlpf_min_distance",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "min distance for distance lpf");
    g.integer64.min = -60.0;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.75;
    snd_dlpf_attenuation = Dvar_RegisterVariant(
        "snd_dlpf_attenuation",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "attenuation for distance lpf");
    g.integer64.min = __PAIR64__(20.0, 0.0099999998);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = (int)10.0;
    snd_dopplerRate = Dvar_RegisterVariant(
        "snd_dopplerRate",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "global doppler lerp rate");
    g.integer64.min = __PAIR64__(100.0, -200.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    HIDWORD(v814) = 1.0;
    snd_dopplerScale = Dvar_RegisterVariant(
        "snd_dopplerScale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "global doppler ratio");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.30000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    snd_dry_scale = Dvar_RegisterVariant(
        "snd_dry_scale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "scalar value for dry signal path");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 0;
    LODWORD(v814) = 0;
    snd_flux_separation = Dvar_RegisterVariant(
        "snd_flux_separation",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "separation of L/R flux elements, 0 none, 1 180 deg, not linear in angle");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1400.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000.0;
    snd_futz_force = Dvar_RegisterVariant("snd_futz_force", DVAR_TYPE_FLOAT, 0x84000u, v0, g, nullptr);
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 2.0;
    snd_futz_min = Dvar_RegisterVariant("snd_futz_min", DVAR_TYPE_FLOAT, 0x84000u, v0, g, "futz min");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1000.0;
    snd_global_pitch = Dvar_RegisterVariant("snd_global_pitch", DVAR_TYPE_FLOAT, 0x84000u, v0, g, "global pitch");
    g.integer64.min = 0x461C400000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -35.0;
    snd_isbig_distance = Dvar_RegisterVariant(
        "snd_isbig_distance",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "distance where isbig doesn't make a difference in occlusion");
    g.integer64.min = -80.0;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2000.0;
    snd_occlusion_attenuation = Dvar_RegisterVariant(
        "snd_occlusion_attenuation",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "maximum occlusion attenuation");
    g.integer64.min = 0x461C400000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 6.0;
    snd_occlusion_cull_distance = Dvar_RegisterVariant(
        "snd_occlusion_cull_distance",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "distance at which we don't do occlusion at start");
    g.integer64.min = __PAIR64__(100.0, 0.0099999998);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    snd_occlusion_rate = Dvar_RegisterVariant(
        "snd_occlusion_rate",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "snd occlusion rate");
    v812.value = 0.25;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 2.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 3.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 100.0;
    snd_pitch_timescale = Dvar_RegisterVariant(
        "snd_pitch_timescale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "pitch scalar by timescale");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 20.0;
    snd_playing_priority_boost = Dvar_RegisterVariant(
        "snd_playing_priority_boost",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "priority penalty for new sounds");
    g.integer64.min = 0x461C400000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.07;
    LODWORD(v814) = 0;
    snd_start_alias_distance = Dvar_RegisterVariant(
        "snd_start_alias_distance",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        nullptr);
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    HIDWORD(v814) = 1.0;
    snd_timescale_filter = Dvar_RegisterVariant(
        "snd_timescale_filter",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "filter coef for timescale fade");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 300.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3600.0;
    snd_wet_scale = Dvar_RegisterVariant(
        "snd_wet_scale",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "scalar value for wet signal path");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 6.0;
    sv_kickBanTime = Dvar_RegisterVariant(
        "sv_kickBanTime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Time in seconds for a player to be banned from the server after being kicked");
    g.integer64.min = __PAIR64__(3.4028235e38, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 4.0;
    tracer_explosiveWidth = Dvar_RegisterVariant(
        "tracer_explosiveWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The width of a bullet tracer when using explosive bullets");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.1);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 6.0;
    tracer_firstPersonMaxWidth = Dvar_RegisterVariant(
        "tracer_firstPersonMaxWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The maximum width our OWN tracers can be when looking throug our ADS");
    g.integer64.min = __PAIR64__(3.4028235e38, 1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    tracer_stoppingPowerWidth = Dvar_RegisterVariant(
        "tracer_stoppingPowerWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The width of a bullet tracer when using StoppingPower bullets");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.1);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 20.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    tracer_thermalWidthMult = Dvar_RegisterVariant(
        "tracer_thermalWidthMult",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The multiplier applied to the base width when viewed in thermal vision");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 42.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    turret_placement_feet_trace_dist_z = Dvar_RegisterVariant(
        "turret_placement_feet_trace_dist_z",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Max distance for a foot to be considered touching the ground");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 42.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    turret_placement_trace_dist = Dvar_RegisterVariant(
        "turret_placement_trace_dist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance along the trace axis where the turret will attempt to position itself");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    turret_placement_trace_dist_zm = Dvar_RegisterVariant(
        "turret_placement_trace_dist_zm",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance along the trace axis where the turret will attempt to position itself (zombies)");
    v812.value = 100.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.69999999;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    turret_placement_trace_dist_wallmount = Dvar_RegisterVariant(
        "turret_placement_trace_dist_wallmount",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance along the trace axis where the turret will attempt to position itse"
        "lf (wallmounted for zombies)");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 20.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    turret_placement_trace_min_normal = Dvar_RegisterVariant(
        "turret_placement_trace_min_normal",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Minimum normal to accept a turret position");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 5.0;
    LODWORD(v814) = 0;
    turret_placement_trace_pitch = Dvar_RegisterVariant(
        "turret_placement_trace_pitch",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Pitch used for the trace axis");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.99;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    turret_placement_trace_radius_canon_safety = Dvar_RegisterVariant(
        "turret_placement_trace_radius_canon_safety",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Extra radius used in the forward direction to compensate for the canon length");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.5;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    ui_blurAmount = Dvar_RegisterVariant(
        "ui_blurAmount",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Max amount to blur background menu items.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 250.0;
    LODWORD(v814) = 0;
    ui_blurDarkenAmount = Dvar_RegisterVariant(
        "ui_blurDarkenAmount",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Max amount to darken the background menu items.");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.2;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    ui_blurTime = Dvar_RegisterVariant(
        "ui_blurTime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Time in milliseconds to fade in/out the blur.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.22;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    ui_buildSize = Dvar_RegisterVariant(
        "ui_buildSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Font size to use for the build number");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.49000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    ui_fxFontFalloff = Dvar_RegisterVariant(
        "ui_fxFontFalloff",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Font edg fuzziness");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.38;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 0.5;
    ui_fxFontGlowSize = Dvar_RegisterVariant(
        "ui_fxFontGlowSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Size of the glow");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = -1.0;
    ui_fxFontOutlineSize = Dvar_RegisterVariant(
        "ui_fxFontOutlineSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Size of the outline");
    g.integer64.min = __PAIR64__(1.0, -1.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.0;
    ui_fxFontShadowSize = Dvar_RegisterVariant(
        "ui_fxFontShadowSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Size of the shadow");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    ui_saveMessageMinTime = Dvar_RegisterVariant(
        "ui_saveMessageMinTime",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Minumum time for the save messag to be on screen in seconds");
    v812.value = 3.0;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 10.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 2.0;
    ui_textPaddingOverride = Dvar_RegisterVariant(
        "ui_textPaddingOverride",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Text padding");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.1);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.30000001;
    vehHelicopterAlwaysFaceCameraRate = Dvar_RegisterVariant(
        "vehHelicopterAlwaysFaceCameraRate",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "How quickly the helicopter catches up to the camera.");
    g.integer64.min = __PAIR64__(3.4028235e38, 0.000099999997);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 8.0;
    vehHelicopterJitterJerkyness = Dvar_RegisterVariant(
        "vehHelicopterJitterJerkyness",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Specifies how jerky the tilt jitter should be");
    g.integer64.min = __PAIR64__(0.0099999998, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 40.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000000.0;
    vehHudLineWidth = Dvar_RegisterVariant(
        "vehHudLineWidth",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The width of the line used by code to draw elements on the vehicle HUD");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 80.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 1000000.0;
    vehHudReticleBouncingDiamondSize = Dvar_RegisterVariant(
        "vehHudReticleBouncingDiamondSize",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The size of the bouncing diamond quad.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 400.0;
    LODWORD(v814) = 0;
    vehHudReticleBouncingRadius = Dvar_RegisterVariant(
        "vehHudReticleBouncingRadius",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The radius of the circle in which the diamond bounces.");
    *((float*)&v814 + 1) = 1000000.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 8.0;
    vehHudReticleBouncingSpeed = Dvar_RegisterVariant(
        "vehHudReticleBouncingSpeed",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The rate at which the bouncing diamond moves");
    g.integer64.min = __PAIR64__(0.0099999998, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.25;
    vehHudReticlePipOnAStickCenterCircle = Dvar_RegisterVariant(
        "vehHudReticlePipOnAStickCenterCircle",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The diameter of the small, center circle in the pip-on-a-stick reticle");
    g.integer64.min = __PAIR64__(-10000.0, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 32.0;
    vehHudReticlePipOnAStickCenterCircleBuffer = Dvar_RegisterVariant(
        "vehHudReticlePipOnAStickCenterCircleBuffer",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Tweaks how close the stick is drawn to the center circle.  Positive num"
        "bers makes the line longer.");
    g.integer64.min = __PAIR64__(0.0099999998, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.integer = 1.0;
    vehHudReticlePipOnAStickMovingCircle = Dvar_RegisterVariant(
        "vehHudReticlePipOnAStickMovingCircle",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "The diameter of the larg, moving circle in the pip-on-a-stick reticle");
    g.integer64.min = __PAIR64__(-10000.0, 10000.0);
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 0.74000001;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    vehHudReticlePipOnAStickMovingCircleBuffer = Dvar_RegisterVariant(
        "vehHudReticlePipOnAStickMovingCircleBuffer",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Tweaks how close the stick is drawn to the moving circle.  Positive num"
        "bers makes the line longer.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1250.0;
    vehicle_perk_boost_duration_seconds = Dvar_RegisterVariant(
        "vehicle_perk_boost_duration_seconds",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Vehicle perk boost duration in seconds.");
    g.integer64.min = (unsigned int)10.0 | 0x47C3500000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    weaponCamoLodDist = Dvar_RegisterVariant(
        "weaponCamoLodDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance to stop drawing the weapon camo.");
    v812.value = 300.0;
    g.integer64.min = (unsigned int)10.0 | 0x47C3500000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1000.0;
    weaponClanTagLodDist = Dvar_RegisterVariant(
        "weaponClanTagLodDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance to stop drawing the weapon clan tag.");
    g.integer64.min = (unsigned int)10.0 | 0x47C3500000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 300.0;
    weaponClanTagShieldLodDist = Dvar_RegisterVariant(
        "weaponClanTagShieldLodDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance to stop drawing the weapon clan tag for the riot shield.");
    g.integer64.min = (unsigned int)10.0 | 0x47C3500000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    v812.value = 1000.0;
    weaponEmblemLodDist = Dvar_RegisterVariant(
        "weaponEmblemLodDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance to stop drawing the weapon emblem.");
    g.integer64.min = (unsigned int)10.0 | 0x47C3500000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = 0i64;
    weaponEmblemShieldLodDist = Dvar_RegisterVariant(
        "weaponEmblemShieldLodDist",
        DVAR_TYPE_FLOAT,
        0x84000u,
        v0,
        g,
        "Distance to stop drawing the weapon emblem for the riot shield.");
    cg_ScoresPing_Interval = _Dvar_RegisterInt(
        "cg_ScoresPing_Interval",
        100,
        1,
        500,
        0x84000u,
        "Number of milliseconds each bar represents");
    cg_ScoresPing_MaxBars = _Dvar_RegisterInt(
        "cg_ScoresPing_MaxBars",
        4,
        1,
        10,
        0x84000u,
        "Number of bars to show in ping graph");
    cg_adsZoomToggleStyle = _Dvar_RegisterInt(
        "cg_adsZoomToggleStyle",
        1,
        0,
        1,
        0x84000u,
        "Style of zoom toggle - 0=oscillate, 1=rotate");
    cg_bloodLimitMsec = _Dvar_RegisterInt(
        "cg_bloodLimitMsec",
        330,
        1,
        2000,
        0x84000u,
        "When limiting blood effects, number of milliseconds between effects.");
    cg_chatHeight = _Dvar_RegisterInt("cg_chatHeight", 8, 0, 8, 0x84000u, "The font height of a chat message");
    cg_chatTime = _Dvar_RegisterInt(
        "cg_chatTime",
        12000,
        0,
        60000,
        0x84000u,
        "The amount of time that a chat messag is visible");
    cg_gameBoldMessageWidth = _Dvar_RegisterInt(
        "cg_gameBoldMessageWidth",
        390,
        130,
        1664,
        0x84000u,
        "The maximum character width of the bold game messages");
    cg_gameMessageWidth = _Dvar_RegisterInt(
        "cg_gameMessageWidth",
        455,
        130,
        1664,
        0x84000u,
        "The maximum character width of the game messages");
    cg_hintFadeTime = _Dvar_RegisterInt(
        "cg_hintFadeTime",
        100,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Time in milliseconds for the cursor hint to fade");
    cg_hudDamageDirectionalIconTime = _Dvar_RegisterInt(
        "cg_hudDamageDirectionalIconTime",
        1000,
        1,
        0x7FFFFFFF,
        0x84000u,
        "The amount of time for the damag icon to stay on screen after damag is taken");
    cg_hudDamageIconTime = _Dvar_RegisterInt(
        "cg_hudDamageIconTime",
        2000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "The amount of time for the damag icon to stay on screen after damag is taken");
    cg_invalidCmdHintBlinkInterval = _Dvar_RegisterInt(
        "cg_invalidCmdHintBlinkInterval",
        600,
        1,
        0x7FFFFFFF,
        0x84000u,
        "Blink rate of an invalid command hint");
    cg_invalidCmdHintDuration = _Dvar_RegisterInt(
        "cg_invalidCmdHintDuration",
        1800,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Duration of an invalid command hint");
    cg_mapLocationSelectionRotationSpeed = _Dvar_RegisterInt(
        "cg_mapLocationSelectionRotationSpeed",
        3,
        1,
        10,
        0x84000u,
        "Rotation speed of the cursor when selecting a location on the map");
    cg_marqueeTimeScale = _Dvar_RegisterInt(
        "cg_marqueeTimeScale",
        30,
        20,
        0x7FFFFFFF,
        0x84000u,
        "Time in milliseconds to show the full name in the marquee");
    cg_objectiveListWrapCountWidescreen = _Dvar_RegisterInt(
        "cg_objectiveListWrapCountWidescreen",
        530,
        100,
        640,
        0x84000u,
        "The amount of on-screen length to wrap an objective in wide-screen mode");
    cg_overheadNamesFont = _Dvar_RegisterInt(
        "cg_overheadNamesFont",
        2,
        0,
        7,
        0x84000u,
        "Font for overhead names ( see menudefinition.h )");
    cg_rangeFinderColorChangeDelay = _Dvar_RegisterInt(
        "cg_rangeFinderColorChangeDelay",
        0,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Time in milliseconds for the rang finder to chang reticle color after an enemy is spotted.");
    cg_scoreboardBannerHeight = _Dvar_RegisterInt(
        "cg_scoreboardBannerHeight",
        35,
        1,
        100,
        0x84000u,
        "Banner height of the scoreboard");
    cg_scoreboardFont = _Dvar_RegisterInt(
        "cg_scoreboardFont",
        3,
        0,
        7,
        0x84000u,
        "Scoreboard font enum ( see menudefinition.h )");
    cg_scoreboardItemHeight = _Dvar_RegisterInt(
        "cg_scoreboardItemHeight",
        18,
        1,
        1000,
        0x84000u,
        "Item height of each item");
    cg_scoreboardScrollStep = _Dvar_RegisterInt(
        "cg_scoreboardScrollStep",
        3,
        1,
        8,
        0x84000u,
        "Scroll step amount for the scoreboard");
    cg_seatHintFadeTime = _Dvar_RegisterInt(
        "cg_seatHintFadeTime",
        2000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Time in milliseconds for the seat hint to fade");
    cg_showmiss = _Dvar_RegisterInt("cg_showmiss", 0, 0, 2, 0x84000u, "Show prediction errors");
    cg_weaponCycleDelay = _Dvar_RegisterInt(
        "cg_weaponCycleDelay",
        0,
        0,
        0x7FFFFFFF,
        0x84000u,
        "The delay after cycling to a new weapon to prevent holding down the cycle weapon button from cycling too fast");
    cl_dblTapMaxDelayTime = _Dvar_RegisterInt(
        "cl_dblTapMaxDelayTime",
        500,
        0,
        5000,
        0x84000u,
        "The maximum amount of time (ms) between button presses to be considered a double tap");
    cl_dblTapMaxHoldTime = _Dvar_RegisterInt(
        "cl_dblTapMaxHoldTime",
        500,
        0,
        5000,
        0x84000u,
        "The maximum amount of time (ms) the player can hold the button to be considered a double tap");
    cl_dtpHoldTime = _Dvar_RegisterInt(
        "cl_dtpHoldTime",
        200,
        0,
        1000,
        0x84000u,
        "The time to hold the stance button while sprinting before the player dives to prone");
    cl_maxpackets = _Dvar_RegisterInt("cl_maxpackets", 30, 15, 100, 0x84000u, "Maximum number of packets sent per frame");
    cl_packetdup = _Dvar_RegisterInt("cl_packetdup", 1, 0, 5, 0x84000u, "Enable packet duplication");
    cl_serverStatusResendTime = _Dvar_RegisterInt(
        "cl_serverStatusResendTime",
        750,
        0,
        3600,
        0x84000u,
        "Time in milliseconds to resend a server status message");
    cl_shownet = _Dvar_RegisterInt("cl_shownet", 0, -2, 4, 0x84000u, "Display network debugging information");
    cl_socketpool_size = _Dvar_RegisterInt(
        "cl_socketpool_size",
        10,
        0,
        50,
        0x84000u,
        "Number of sockets to use for server browser socket pool");
    cl_stanceHoldTime = _Dvar_RegisterInt(
        "cl_stanceHoldTime",
        300,
        0,
        1000,
        0x84000u,
        "The time to hold the stance button before the player goes prone");
    cl_weapNextHoldTime = _Dvar_RegisterInt(
        "cl_weapNextHoldTime",
        250,
        0,
        0x7FFFFFFF,
        0x84000u,
        "The time to hold the weapon next button before the player switches to the inventory weapon");
    com_attractmodeduration = _Dvar_RegisterInt(
        "com_attractmodeduration",
        90,
        0,
        120,
        0x84000u,
        "Time when controller is unused before attract mode is enabled");
    com_maxFrameTime = _Dvar_RegisterInt(
        "com_maxFrameTime",
        100,
        50,
        5000,
        0x84000u,
        "Time slows down if a frame takes longer than this many milliseconds");
    com_voip_disable_threshold = _Dvar_RegisterInt(
        "com_voip_disable_threshold",
        1200,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Messag size at which voip becomes disabled");
    compassObjectiveNumRings = _Dvar_RegisterInt(
        "compassObjectiveNumRings",
        10,
        0,
        20,
        0x84000u,
        "The number of ring when a new objective appears");
    compassObjectiveRingTime = _Dvar_RegisterInt(
        "compassObjectiveRingTime",
        10000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "The amount of time between each ring when an objective appears");
    compassPartialType = _Dvar_RegisterInt(
        "compassPartialType",
        0,
        0,
        1,
        0x84000u,
        "The style of compass to use 2d or 3d");
    con_miniconlines = _Dvar_RegisterInt(
        "con_miniconlines",
        5,
        1,
        100,
        0x84000u,
        "Number of lines in the minicon messag window");
    con_typewriterDecayDuration = _Dvar_RegisterInt(
        "con_typewriterDecayDuration",
        1000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Time (in milliseconds) to spend disolving the line away.");
    con_typewriterDecayStartTime = _Dvar_RegisterInt(
        "con_typewriterDecayStartTime",
        4000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Time (in milliseconds) to spend between the build and disolve phases.");
    con_typewriterPrintSpeed = _Dvar_RegisterInt(
        "con_typewriterPrintSpeed",
        40,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Time (in milliseconds) to print each letter in the line.");
    debug_audio = _Dvar_RegisterInt("debug_audio", 0, 0, 10, 0x84000u, "Enables debug rendering for audio.");
    debug_mover = _Dvar_RegisterInt("debug_mover", 0, 0, 1, 0x84000u, "Turns on mover debug mode");
    demo_drawdebuginformation = _Dvar_RegisterInt(
        "demo_drawdebuginformation",
        0,
        0,
        4,
        0x84000u,
        "Used to draw debug information.");
    drawEntityCountSize = _Dvar_RegisterInt(
        "drawEntityCountSize",
        32,
        0,
        0x7FFFFFFF,
        0x84000u,
        "How big to draw the entity count graph");
    dvar_maxCallbackTimeMS = _Dvar_RegisterInt(
        "dvar_maxCallbackTimeMS",
        10,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Max ms to spend doing dvar modified callbacks");
    cl_avidemo = _Dvar_RegisterInt("cl_avidemo", 0, 0, 0x7FFFFFFF, 0x84000u, "AVI demo frames per second");
    emblem_scroll_delay_first = _Dvar_RegisterInt(
        "emblem_scroll_delay_first",
        150,
        0,
        1000,
        0x84000u,
        "First repeat delay for emblem editor");
    emblem_scroll_delay_rest = _Dvar_RegisterInt(
        "emblem_scroll_delay_rest",
        50,
        0,
        1000,
        0x84000u,
        "Repeat delay for emblem editor");
    freeBuffersDelay = _Dvar_RegisterInt(
        "freeBuffersDelay",
        1,
        0,
        10,
        0x84000u,
        "Frames delay until vertex & index buffers are freed");
    g_deathDelay = _Dvar_RegisterInt("g_deathDelay", 4000, 0, 0x7FFFFFFF, 0x84000u, "Delay a level restart on death");
    g_inactivity = _Dvar_RegisterInt(
        "g_inactivity",
        0,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Time delay before player is kicked for inactivity");
    g_playerRespawnTime = _Dvar_RegisterInt(
        "g_playerRespawnTime",
        0,
        0,
        20000,
        0x84000u,
        "Time for player to respawn in milliseconds");
    g_useholdtime = _Dvar_RegisterInt(
        "g_useholdtime",
        250,
        0,
        0x7FFFFFFF,
        0x84000u,
        "The time to hold down the 'use' button to activate a 'use' command");
    g_voiceChatTalkingDuration = _Dvar_RegisterInt(
        "g_voiceChatTalkingDuration",
        500,
        0,
        10000,
        0x84000u,
        "Time after the last talk packet was received that the player is considered by the\n"
        "server to still be talking in milliseconds");
    gpad_menu_scroll_delay_first = _Dvar_RegisterInt(
        "gpad_menu_scroll_delay_first",
        420,
        0,
        1000,
        0x84000u,
        "Menu scroll key-repeat delay, for the first repeat, in milliseconds");
    gpad_menu_scroll_delay_rest = _Dvar_RegisterInt(
        "gpad_menu_scroll_delay_rest",
        210,
        0,
        1000,
        0x84000u,
        "Menu scroll key-repeat delay, for repeats after the first, in milliseconds");
    hud_deathQuoteFadeTime = _Dvar_RegisterInt(
        "hud_deathQuoteFadeTime",
        1000,
        0,
        100000,
        0x84000u,
        "The time for the death quote to fade");
    lua_systemTest = _Dvar_RegisterInt("lua_systemTest", 0, 0, 2, 0x84000u, "Debug UI running.");
    maxAttachmentsPerWeapon = _Dvar_RegisterInt(
        "maxAttachmentsPerWeapon",
        2,
        0,
        255,
        0x84000u,
        "Max attachments allowed for a weapon");
    maxGlassShatters = _Dvar_RegisterInt(
        "maxGlassShatters",
        2,
        1,
        999,
        0x84000u,
        "Max number of glass shatters that can happen in a single frame");
    modPrvAnimDumpInTime = _Dvar_RegisterInt("modPrvAnimDumpInTime", 0, 0, 0x7FFFFFFF, 0x84000u, nullptr);
    modPrvAnimDumpOutTime = _Dvar_RegisterInt("modPrvAnimDumpOutTime", 0, 0, 0x7FFFFFFF, 0x84000u, nullptr);
    net_profile = _Dvar_RegisterInt("net_profile", 0, 0, 2, 0x84000u, "Profile network performance");
    net_showprofile = _Dvar_RegisterInt("net_showprofile", 0, 0, 3, 0x84000u, "Show network profiling display");
    party_debug = _Dvar_RegisterInt("party_debug", 0, 0, 0x7FFFFFFF, 0x84000u, "Debug party functionality");
    party_debugMembers = _Dvar_RegisterInt(
        "party_debugMembers",
        0,
        0,
        3,
        0x84000u,
        "Display debug data about member reading and writing, 1 - writing, 2 - reading, 3 - both");
    phys_ai_collision_mode = _Dvar_RegisterInt("phys_ai_collision_mode", 1, 0, 1, 0x84000u, "phys ai collision mode");
    phys_buoyancyRippleFrequency = _Dvar_RegisterInt(
        "phys_buoyancyRippleFrequency",
        3000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "How often (in ms) floating objects will play the ripple fx");
    phys_buoyancyRippleVariance = _Dvar_RegisterInt(
        "phys_buoyancyRippleVariance",
        2000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "How late (in ms) the ripple fx can be played");
    phys_floatTimeVariance = _Dvar_RegisterInt(
        "phys_floatTimeVariance",
        10000,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Variance in time (in ms) objects will float for");
    phys_msecStep = _Dvar_RegisterInt(
        "phys_msecStep",
        0,
        0,
        1000,
        0x84000u,
        "Physics simulation update increment in milliseconds");
    phys_player_collision_mode = _Dvar_RegisterInt(
        "phys_player_collision_mode",
        1,
        0,
        1,
        0x84000u,
        "phys player collision mode");
    r_clear = _Dvar_RegisterInt("r_clear", 2, 0, 4, 0x84000u, "Controls how the color buffer is cleared");
    r_framelock = _Dvar_RegisterInt(
        "r_framelock",
        1,
        0,
        2,
        0x84000u,
        "Limit frame rate to minimum number of v-syncs between each frame. eg 0 for none, 1 for 60fps, 2 for 30fps, etc ");
    r_framesyncThreshold = _Dvar_RegisterInt(
        "r_framesyncThreshold",
        8,
        0,
        100,
        0x84000u,
        "Percentag of frame scanout to occur before flipping display. 0 = force flip on vsync, 100 = flip anytime");
    r_fullHDRrendering = _Dvar_RegisterInt("r_fullHDRrendering", 0, 0, 2, 0x84000u, "enable full HDR rendering");
    r_hideShadowingSmodels = _Dvar_RegisterInt(
        "r_hideShadowingSmodels",
        0,
        0,
        2,
        0x84000u,
        "Hide static models that cast shadows (1:flash 2:hide).");
    r_hideShadowingSurfaces = _Dvar_RegisterInt(
        "r_hideShadowingSurfaces",
        0,
        0,
        2,
        0x84000u,
        "Hide BSP surfaces that cast shadows (1:flash 2:hide).");
    r_pixelShaderGPRReallocation = _Dvar_RegisterInt(
        "r_pixelShaderGPRReallocation",
        96,
        0,
        128,
        0x84000u,
        "Number of general purpose registers to the pixel shader");
    r_polygonOffsetBiasInt = _Dvar_RegisterInt(
        "r_polygonOffsetBiasInt",
        -256,
        -4096,
        0,
        0x84000u,
        "Offset bias for decal polygons; bigger values z-fight less but poke throug walls more");
    ragdoll_max_simulating = _Dvar_RegisterInt(
        "ragdoll_max_simulating",
        16,
        0,
        32,
        0x84000u,
        "Max number of simultaneous active ragdolls");
    render_bpi_env_collision = _Dvar_RegisterInt(
        "render_bpi_env_collision",
        0,
        0,
        5000,
        0x84000u,
        "render bpi_env collision");
    showpackets = _Dvar_RegisterInt("showpackets", 0, 0, 2, 0x84000u, "Show packets");
    snd_levelFadeTime = _Dvar_RegisterInt(
        "snd_levelFadeTime",
        250,
        0,
        5000,
        0x84000u,
        "The amount of time in milliseconds for all audio to fade in at the start of a level");
    sv_forcelicensetype = _Dvar_RegisterInt("sv_forcelicensetype", 0, 0, 0x7FFFFFFF, 0x84000u, nullptr);
    sv_maxHappyPingTime = _Dvar_RegisterInt(
        "sv_maxHappyPingTime",
        400,
        0,
        999,
        0x84000u,
        "Max ping that a player can have that we consider acceptable when calculating if the server is swamped");
    sv_padPackets = _Dvar_RegisterInt("sv_padPackets", 0, 0, 0x7FFFFFFF, 0x84000u, "add nop bytes to messages");
    test_levels_choices = _Dvar_RegisterInt("test_levels_choices", 0, 0, 0x7FFFFFFF, 0x84000u, "Number of Levels");
    tickerStandardWidth = _Dvar_RegisterInt(
        "tickerStandardWidth",
        300,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Ticker messag text standard width.");
    tickerWidescreenWidth = _Dvar_RegisterInt(
        "tickerWidescreenWidth",
        400,
        0,
        0x7FFFFFFF,
        0x84000u,
        "Ticker messag text widescreen width.");
    ui_dropdownListMax = _Dvar_RegisterInt(
        "ui_dropdownListMax",
        0,
        0x80000000,
        0x7FFFFFFF,
        0x84000u,
        "Number of elements in list that opened dropdown.");
    ui_dropdownMax = _Dvar_RegisterInt(
        "ui_dropdownMax",
        0,
        0x80000000,
        0x7FFFFFFF,
        0x84000u,
        "Number of elements in the dropdown.");
    uiscript_debug = _Dvar_RegisterInt("uiscript_debug", 0, 0, 2, 0x84000u, "spam debug info for the ui script");
    vehRecenterDelay = _Dvar_RegisterInt(
        "vehRecenterDelay",
        2700,
        0,
        10000,
        0x84000u,
        "Specifies a delay in msec, before triggering the camera auto-centering functionality in idle mode");
    xanim_monitorEnt = _Dvar_RegisterInt(
        "xanim_monitorEnt",
        -1,
        -1,
        1023,
        0x84000u,
        "If filter is set to send a single entity, this defines the entity number to send");
    xanim_monitorFilter = _Dvar_RegisterInt(
        "xanim_monitorFilter",
        0,
        0,
        4,
        0x84000u,
        "Determines what filter will be applied to data sent to the anim monitor");
    bg_weaponleftbone = _Dvar_RegisterString(
        "bg_weaponleftbone",
        "tag_weapon_left",
        0x84000u,
        "Left hand weapon bone name");
    bg_weaponrightbone = _Dvar_RegisterString(
        "bg_weaponrightbone",
        "tag_weapon_right",
        0x84000u,
        "Right handed weapon bone name");
    cg_rangeFinderDefaultDisplayStr = _Dvar_RegisterString(
        "cg_rangeFinderDefaultDisplayStr",
        "----",
        0x84000u,
        "Default string to display on the rang finder attachment when it is not active.");
    con_default_console_filter = _Dvar_RegisterString(
        "con_default_console_filter",
        "*",
        0x84000u,
        "Default channel filter for the console destination.");
    flameVar_editingFlameTable = _Dvar_RegisterString(
        "flameVar_editingFlameTable",
        "none",
        0x84000u,
        "This is the most recent flametable that has been put up for editing, mostly included to help artists");
    flameVar_lastFlameTable = _Dvar_RegisterString(
        "flameVar_lastFlameTable",
        "none",
        0x84000u,
        "This is the previous flametable that was put up for editing, mostly included to help artists");
    modPrvCurrAndMaxFrameIndexes = _Dvar_RegisterString(
        "modPrvCurrAndMaxFrameIndexes",
        nullptr,
        0x84000u,
        nullptr);
    snd_assert_on_duck_stop = _Dvar_RegisterString(
        "snd_assert_on_duck_stop",
        nullptr,
        0x84000u,
        "assert when duck stopped which matches this name");
    snd_assert_on_enqueue = _Dvar_RegisterString(
        "snd_assert_on_enqueue",
        nullptr,
        0x84000u,
        "assert if played alias is put into q");
    snd_assert_on_play = _Dvar_RegisterString(
        "snd_assert_on_play",
        nullptr,
        0x84000u,
        "assert if played alias");
    snd_assert_on_stop = _Dvar_RegisterString("snd_assert_on_stop", " ", 0x84000u, "assert if stopped alias");
    snd_assert_on_animation_prime = _Dvar_RegisterString(
        "snd_assert_on_animation_prime",
        nullptr,
        0x84000u,
        "assert if animation primes this alias");
    snd_debug_alias_filter = _Dvar_RegisterString(
        "snd_debug_alias_filter",
        nullptr,
        0x84000u,
        "only allow aliases containing this string to trigger");
    snd_debug_context_type = _Dvar_RegisterString(
        "snd_debug_context_type",
        nullptr,
        0x84000u,
        "enable debug context type");
    snd_debug_context_value = _Dvar_RegisterString(
        "snd_debug_context_value",
        nullptr,
        0x84000u,
        "enable debug context value");
    snd_debug_duck = _Dvar_RegisterString("snd_debug_duck", nullptr, 0x84000u, "enable debug duck");
    snd_gfutz_override = _Dvar_RegisterString("snd_gfutz_override", nullptr, 0x84000u, "override gfutz");
    snd_master_override = _Dvar_RegisterString(
        "snd_master_override",
        nullptr,
        0x84000u,
        "overide current master");
    snd_mute_alias_substring = _Dvar_RegisterString(
        "snd_mute_alias_substring",
        nullptr,
        0x84000u,
        "mute alias with not matching substring");
    snd_reverb_override = _Dvar_RegisterString(
        "snd_reverb_override",
        nullptr,
        0x84000u,
        "overide current verb");
    snd_start_alias = _Dvar_RegisterString("snd_start_alias", nullptr, 0x84000u, "start sound");
    snd_stop_alias = _Dvar_RegisterString("snd_stop_alias", nullptr, 0x84000u, "stop sound");
    snd_solo_alias_substring = _Dvar_RegisterString(
        "snd_solo_alias_substring",
        nullptr,
        0x84000u,
        "mute alias with matching substring");
    snd_solo_snapshot_group = _Dvar_RegisterString(
        "snd_solo_snapshot_group",
        nullptr,
        0x84000u,
        "solo aliases with matching snapshot group");
    snd_voice_duck_override = _Dvar_RegisterString(
        "snd_voice_duck_override",
        nullptr,
        0x84000u,
        "overide current voice duck patch");
    test_levels_bsps = _Dvar_RegisterString("test_levels_bsps", nullptr, 0x84000u, "Test Level Bsps");
    test_levels_names = _Dvar_RegisterString("test_levels_names", nullptr, 0x84000u, "Test Level Names");
    test_levels_selection = _Dvar_RegisterString(
        "test_levels_selection",
        nullptr,
        0x84000u,
        "Current Selection");
    g.integer64.min = __PAIR64__(10.0, -10.0);
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(1.72, 3.5);
    *((__int64*)&v0.string + 1) = 0i64;
    ui_fxFontShadowOffset = Dvar_RegisterVariant(
        "ui_fxFontShadowOffset",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Offset of the shadow (UV)");
    g.integer64.min = __PAIR64__(640.0, -200.0);
    g.integer64.max = 0i64;
    v0.integer64 = 0i64;
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 640.0;
    cg_animInfoCornerOffset = Dvar_RegisterVariant(
        "cg_animInfoCornerOffset",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Offset from bottom left corner, for drawing anim info, etc");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(470.0, 5.0);
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    cg_hudChatIntermissionPosition = Dvar_RegisterVariant(
        "cg_hudChatIntermissionPosition",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Position of the HUD chat box during intermission");
    *((float*)&v814 + 1) = 640.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(250.0, 5.0);
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 640.0;
    cg_hudChatPosition = Dvar_RegisterVariant(
        "cg_hudChatPosition",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Position of the HUD chat box");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(230.0, 5.0);
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 640.0;
    cg_hudSayPosition = Dvar_RegisterVariant(
        "cg_hudSayPosition",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Position of the HUD say box");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(220.0, 5.0);
    *((__int64*)&v0.string + 1) = 0i64;
    cg_hudVotePosition = Dvar_RegisterVariant(
        "cg_hudVotePosition",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Position of the HUD vote box");
    g.integer64.min = __PAIR64__(3.4028235e38, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(-180, -55.0);
    *((__int64*)&v0.string + 1) = 0i64;
    drawEntityCountPos = Dvar_RegisterVariant(
        "drawEntityCountPos",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Where to draw the entity count graph");
    g.integer64.min = __PAIR64__(3.4028235e38, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(-230.0, -55.0);
    *((__int64*)&v0.string + 1) = 0i64;
    drawKillcamDataPos = Dvar_RegisterVariant(
        "drawKillcamDataPos",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Where to draw the server killcam graph");
    g.integer64.min = __PAIR64__(3.4028235e38, -3.4028235e38);
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(-280.0, -55.0);
    *((__int64*)&v0.string + 1) = 0i64;
    drawServerBandwidthPos = Dvar_RegisterVariant(
        "drawServerBandwidthPos",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Where to draw the server bandwidth graph");
    g.integer64.min = 0x461C400000000000i64;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__((unsigned int)10.0, 1.0);
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 0.30000001;
    glassCrackedDamageRateRange = Dvar_RegisterVariant(
        "glassCrackedDamageRateRange",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Rang of the rate a cracked glass looses health points");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(0.15000001, 0.02);
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 180.0;
    shardEdgeSize = Dvar_RegisterVariant(
        "shardEdgeSize",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Rang of the new shard edg length as a fraction of the total shard edges lengths");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(60.0, 30.0);
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    shardSplitDir = Dvar_RegisterVariant(
        "shardSplitDir",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Angle rang of the first edg of the shard");
    *((float*)&v814 + 1) = 180.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(30.0, 10.0);
    *((__int64*)&v0.string + 1) = 0i64;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 10.0;
    shardSplitDir2 = Dvar_RegisterVariant(
        "shardSplitDir2",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Angle rang of the next edges of the shard");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(1.0, 0.25);
    *((__int64*)&v0.string + 1) = 0i64;
    v812.vector.v[2] = 400.0;
    LODWORD(v814) = 0;
    *((float*)&v814 + 1) = 3.4028235e38;
    timeUntilDropRange = Dvar_RegisterVariant(
        "timeUntilDropRange",
        DVAR_TYPE_FLOAT_2,
        0x84000u,
        v0,
        g,
        "Rang of how much time a glass shard will stay on the frame before falling");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(3590.0, 740.0);
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.integer = 0;
    *(__int64*)((char*)&v812.integer64 + 4) = __PAIR64__(0.5, 0.30000001);
    compassCoords = Dvar_RegisterVariant(
        "compassCoords",
        DVAR_TYPE_FLOAT_3,
        0x84000u,
        v0,
        g,
        "x = North-South coord base value, \n"
        "y = East-West coord base value, \n"
        "z = scale (game units per coord unit)");
    g.integer64.min = __PAIR64__(1.0, -1.0);
    g.integer64.max = 0i64;
    v0 = v812;
    v812.vector.v[2] = 0.5;
    player_AimBlend_Back_Low = Dvar_RegisterVariant(
        "player_AimBlend_Back_Low",
        DVAR_TYPE_FLOAT_3,
        0x84000u,
        v0,
        g,
        "3rd person player view aim blend - lower back");
    g.integer64.min = __PAIR64__(1.0, -1.0);
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(0.2, 0.1);
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.vector.v[2] = -0.60000002;
    player_AimBlend_Back_Mid = Dvar_RegisterVariant(
        "player_AimBlend_Back_Mid",
        DVAR_TYPE_FLOAT_3,
        0x84000u,
        v0,
        g,
        "3rd person player view aim blend - mid back");
    g.integer64.min = __PAIR64__(1.0, -1.0);
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(0.1, 0.5);
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.vector.v[2] = 0.0;
    player_AimBlend_Back_Up = Dvar_RegisterVariant(
        "player_AimBlend_Back_Up",
        DVAR_TYPE_FLOAT_3,
        0x84000u,
        v0,
        g,
        "3rd person player view aim blend - upper back");
    g.integer64.min = __PAIR64__(1.0, -1.0);
    g.integer64.max = 0i64;
    v0.integer64 = 0i64;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.value = 0.30000001;
    *(__int64*)((char*)&v812.integer64 + 4) = 0.30000001;
    player_AimBlend_Head = Dvar_RegisterVariant(
        "player_AimBlend_Head",
        DVAR_TYPE_FLOAT_3,
        0x84000u,
        v0,
        g,
        "3rd person player view aim blend - head");
    g.integer64.min = __PAIR64__(1.0, -1.0);
    g.integer64.max = 0i64;
    v0 = v812;
    v812.value = 0.40000001;
    *(__int64*)((char*)&v812.integer64 + 4) = 0.40000001;
    player_AimBlend_Neck = Dvar_RegisterVariant(
        "player_AimBlend_Neck",
        DVAR_TYPE_FLOAT_3,
        0x84000u,
        v0,
        g,
        "3rd person player view aim blend - neck");
    g.integer64.min = __PAIR64__(1.0, -1.0);
    g.integer64.max = 0i64;
    v0 = v812;
    LODWORD(v812.vector.v[2]) = 10.0;
    LODWORD(v814) = 0;
    player_AimBlend_Pelvis = Dvar_RegisterVariant(
        "player_AimBlend_Pelvis",
        DVAR_TYPE_FLOAT_3,
        0x84000u,
        v0,
        g,
        "3rd person player view aim blend - pelvis");
    *((float*)&v814 + 1) = 3.4028235e38;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(20.0, 17.0);
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    turret_placement_feet_offset = Dvar_RegisterVariant(
        "turret_placement_feet_offset",
        DVAR_TYPE_FLOAT_3,
        0x84000u,
        v0,
        g,
        "Position of the feet from the center axis.");
    g.integer64.min = __PAIR64__(3.4028235e38, 1.1754944e-38);
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(24.0, 408.0);
    *((__int64*)&v0.string + 1) = __PAIR64__(456.0, 608.0);
    v812.integer = 1.0;
    LODWORD(v812.vector.v[1]) = 1.0;
    LODWORD(v812.vector.v[2]) = 1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    HIDWORD(v814) = 1.0;
    cg_argus_datastack = Dvar_RegisterVariant(
        "cg_argus_datastack",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "defines bounding area for argus data - virtual coords");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    cg_hudGrenadeIndicatorStartColor = Dvar_RegisterVariant(
        "cg_hudGrenadeIndicatorStartColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v812,
        g,
        nullptr);
    v812.integer = 1.0;
    LODWORD(v812.vector.v[1]) = 1.0;
    LODWORD(v812.vector.v[2]) = 1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    HIDWORD(v814) = 1.0;
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    cg_hudGrenadeIndicatorTargetColor = Dvar_RegisterVariant(
        "cg_hudGrenadeIndicatorTargetColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v812,
        g,
        nullptr);
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(0.1, 0.80000001);
    *((__int64*)&v0.string + 1) = __PAIR64__(0.2, 0.1);
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    cg_sprintMeterDisabledColor = Dvar_RegisterVariant(
        "cg_sprintMeterDisabledColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "The color of the sprint meter when the sprint meter is disabled");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(0.5, 0.69999999);
    *((__int64*)&v0.string + 1) = __PAIR64__(0.80000001, 0.2);
    v812.value = 0.80000001;
    v812.vector.v[1] = 0.80000001;
    v812.vector.v[2] = 0.80000001;
    v812.vector.v[3] = 0.80000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    cg_sprintMeterEmptyColor = Dvar_RegisterVariant(
        "cg_sprintMeterEmptyColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "The color of the sprint meter when the sprint meter is empty");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0 = v812;
    v812.value = 0.25;
    v812.vector.v[1] = 0.25;
    HIDWORD(v814) = 1.0;
    cg_sprintMeterFullColor = Dvar_RegisterVariant(
        "cg_sprintMeterFullColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "The color of the sprint meter when the sprint meter is full");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(1.0, 0.2);
    v812.value = 0.40000001;
    v812.vector.v[1] = 0.40000001;
    con_inputBoxColor = Dvar_RegisterVariant(
        "con_inputBoxColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the console input box");
    HIDWORD(v814) = 1.0;
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(1.0, 0.34999999);
    v812.integer = 1.0;
    LODWORD(v812.vector.v[1]) = 1.0;
    HIDWORD(v814) = 1.0;
    con_inputHintBoxColor = Dvar_RegisterVariant(
        "con_inputHintBoxColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the console input hint box");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.60000002, 0.94999999);
    v812.value = 0.15000001;
    v812.vector.v[1] = 0.15000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    con_outputBarColor = Dvar_RegisterVariant(
        "con_outputBarColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the console output slider bar");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.60000002, 0.1);
    v812.integer = 0.34999999;
    LODWORD(v812.vector.v[1]) = 0.34999999;
    LODWORD(v814) = 0;
    con_outputSliderColor = Dvar_RegisterVariant(
        "con_outputSliderColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the console slider");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.75, 0.30000001);
    HIDWORD(v814) = 1.0;
    con_outputWindowColor = Dvar_RegisterVariant(
        "con_outputWindowColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the console output");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(0.5, 0.60000002);
    *((__int64*)&v0.string + 1) = __PAIR64__(1.0, 0.60000002);
    *(__int64*)((char*)&v812.integer64 + 4) = 0.30000001;
    v812.integer = 0;
    LODWORD(v814) = 0;
    con_typewriterColorGlowCheckpoint = Dvar_RegisterVariant(
        "con_typewriterColorGlowCheckpoint",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of typewritten objective text.");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(1.0, 0.80000001);
    con_typewriterColorGlowCompleted = Dvar_RegisterVariant(
        "con_typewriterColorGlowCompleted",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of typewritten objective text.");
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = 0.80000001;
    LODWORD(v812.vector.v[3]) = 1.0;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.vector.v[1] = 0.60000002;
    v812.integer = 0;
    LODWORD(v814) = 0;
    con_typewriterColorGlowFailed = Dvar_RegisterVariant(
        "con_typewriterColorGlowFailed",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of typewritten objective text.");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(1.0, 0.18000001);
    LODWORD(v814) = 0;
    con_typewriterColorGlowUpdated = Dvar_RegisterVariant(
        "con_typewriterColorGlowUpdated",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of typewritten objective text.");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(1.0, 0.89999998);
    *((__int64*)&v0.string + 1) = __PAIR64__(0.69999999, 0.89999998);
    friendlyNameFontColor = Dvar_RegisterVariant(
        "friendlyNameFontColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        nullptr);
    v812.integer = 0;
    *(__int64*)((char*)&v812.integer64 + 4) = 0.30000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    LODWORD(v812.vector.v[3]) = 1.0;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    friendlyNameFontGlowColor = Dvar_RegisterVariant(
        "friendlyNameFontGlowColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v812,
        g,
        nullptr);
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v812.value = 0.25;
    v812.vector.v[1] = 0.25;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    fsSlotEmptyHiddenColor = Dvar_RegisterVariant(
        "fsSlotEmptyHiddenColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        emptyValue,
        g,
        "Color of the material depicting a private slot when not selected.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.94999999, 0.25);
    v812.value = 0.36000001;
    *(__int64*)((char*)&v812.integer64 + 4) = 0.36000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    fsSlotEmptyMainColor = Dvar_RegisterVariant(
        "fsSlotEmptyMainColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the empty slot. Must be completely opaque to reduce the background to an outline.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.94999999, 0.36000001);
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    fsSlotEmptyNotSubscribedColor = Dvar_RegisterVariant(
        "fsSlotEmptyNotSubscribedColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of emtpy slot that is also classified.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = 0i64;
    v812.vector.v[3] = 0.80000001;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.vector.v[2] = 0.0;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    fsSlotEmptyShowColor = Dvar_RegisterVariant(
        "fsSlotEmptyShowColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the material depicting a private slot when not selected.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = 0i64;
    v812.vector.v[3] = 0.30000001;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 0;
    fsSlotEmptyShowColorBg = Dvar_RegisterVariant(
        "fsSlotEmptyShowColorBg",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the material depicting a private empty slot background when not selected.");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(1.0, 0.5);
    *((__int64*)&v0.string + 1) = __PAIR64__(0.80000001, 0.5);
    v812.integer = 1.0;
    LODWORD(v812.vector.v[1]) = 1.0;
    HIDWORD(v814) = 1.0;
    fsSlotHighlightedColor = Dvar_RegisterVariant(
        "fsSlotHighlightedColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the material depicting a private slot when selected.");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.89999998, 1.0);
    fsSlotHighlightedColorNoSel = Dvar_RegisterVariant(
        "fsSlotHighlightedColorNoSel",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the material depicting a private slot when selected but cannot be chosen.");
    v812.integer = 1.0;
    LODWORD(v812.vector.v[1]) = 1.0;
    LODWORD(v812.vector.v[2]) = 1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    HIDWORD(v814) = 1.0;
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0 = v812;
    v812.value = 0.80000001;
    *(__int64*)((char*)&v812.integer64 + 4) = 0.80000001;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    fsSlotMainColor = Dvar_RegisterVariant(
        "fsSlotMainColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the material depicting a private slot when not selected.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.30000001, 0.80000001);
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    fsSlotNumMainColor = Dvar_RegisterVariant(
        "fsSlotNumMainColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the slot number");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = 0i64;
    v812.vector.v[3] = 0.30000001;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 0;
    fsSlotNumNoSubsribeColor = Dvar_RegisterVariant(
        "fsSlotNumNoSubsribeColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of the empty slot when not a subscriber");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(0.2, 0.69999999);
    *((__int64*)&v0.string + 1) = __PAIR64__(0.80000001, 0.2);
    v812.value = 0.69999999;
    v812.vector.v[1] = 0.69999999;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    fsStarAvgColor = Dvar_RegisterVariant(
        "fsStarAvgColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Highlight color for star rating averag while rating.");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.80000001, 0.1);
    v812.integer = 1.0;
    *(__int64*)((char*)&v812.integer64 + 4) = (unsigned int)1.0;
    HIDWORD(v814) = 1.0;
    fsStarHighlightColor = Dvar_RegisterVariant(
        "fsStarHighlightColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Highlight color for star rating.");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = v812.integer64;
    *((__int64*)&v0.string + 1) = __PAIR64__(0.2, 1.0);
    fshOldItemColor = Dvar_RegisterVariant(
        "fshOldItemColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "Color of old items in search results that can't be selected.");
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = __PAIR64__(0.89999998, 1.0);
    *((__int64*)&v0.string + 1) = __PAIR64__(0.69999999, 0.89999998);
    LODWORD(v814) = 0;
    hostileNameFontColor = Dvar_RegisterVariant(
        "hostileNameFontColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        nullptr);
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = 0.60000002;
    LODWORD(v812.vector.v[3]) = 1.0;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    hostileNameFontGlowColor = Dvar_RegisterVariant(
        "hostileNameFontGlowColor",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        nullptr);
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    tracer_explosiveColor1 = Dvar_RegisterVariant(
        "tracer_explosiveColor1",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        emptyValue,
        g,
        "The 1st color of a bullet tracer when using explosive bullets");
    v812.integer = 1.0;
    LODWORD(v812.vector.v[2]) = 1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    HIDWORD(v814) = 1.0;
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0 = v812;
    v812.vector.v[2] = 0.0;
    LODWORD(v814) = 0;
    tracer_explosiveColor2 = Dvar_RegisterVariant(
        "tracer_explosiveColor2",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "The 2nd color of a bullet tracer when using explosive bullets");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = (unsigned int)1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.vector.v[2] = 0.0;
    LODWORD(v814) = 0;
    tracer_explosiveColor3 = Dvar_RegisterVariant(
        "tracer_explosiveColor3",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "The 3rd color of a bullet tracer when using explosive bullets");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = (unsigned int)1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    tracer_explosiveColor4 = Dvar_RegisterVariant(
        "tracer_explosiveColor4",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "The 4th color of a bullet tracer when using explosive bullets");
    v812.integer = 1.0;
    *(__int64*)((char*)&v812.integer64 + 4) = (unsigned int)1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    HIDWORD(v814) = 1.0;
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    tracer_explosiveColor5 = Dvar_RegisterVariant(
        "tracer_explosiveColor5",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v812,
        g,
        "The 5th color of a bullet tracer when using explosive bullets");
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v812.integer = 1.0;
    LODWORD(v812.vector.v[2]) = 1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    HIDWORD(v814) = 1.0;
    tracer_stoppingPowerColor1 = Dvar_RegisterVariant(
        "tracer_stoppingPowerColor1",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        emptyValue,
        g,
        "The 1st color of a bullet tracer when using StoppingPower bullets");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    tracer_stoppingPowerColor2 = Dvar_RegisterVariant(
        "tracer_stoppingPowerColor2",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v812,
        g,
        "The 2nd color of a bullet tracer when using StoppingPower bullets");
    v812.vector.v[2] = 0.0;
    LODWORD(v814) = 0;
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = (unsigned int)1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.vector.v[2] = 0.0;
    LODWORD(v814) = 0;
    tracer_stoppingPowerColor3 = Dvar_RegisterVariant(
        "tracer_stoppingPowerColor3",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "The 3rd color of a bullet tracer when using StoppingPower bullets");
    HIDWORD(v814) = 1.0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    v0.integer64 = (unsigned int)1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    *((__int64*)&v0.string + 1) = *((__int64*)&v812.string + 1);
    v812.integer = 1.0;
    *(__int64*)((char*)&v812.integer64 + 4) = (unsigned int)1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    HIDWORD(v814) = 1.0;
    tracer_stoppingPowerColor4 = Dvar_RegisterVariant(
        "tracer_stoppingPowerColor4",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v0,
        g,
        "The 4th color of a bullet tracer when using StoppingPower bullets");
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    tracer_stoppingPowerColor5 = Dvar_RegisterVariant(
        "tracer_stoppingPowerColor5",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v812,
        g,
        "The 5th color of a bullet tracer when using StoppingPower bullets");
    v812.integer = 1.0;
    LODWORD(v812.vector.v[2]) = 1.0;
    LODWORD(v812.vector.v[3]) = 1.0;
    HIDWORD(v814) = 1.0;
    LODWORD(v814) = 0;
    g.integer64.min = v814;
    g.integer64.max = 0i64;
    tracer_stoppingPowerColor6 = Dvar_RegisterVariant(
        "tracer_stoppingPowerColor6",
        DVAR_TYPE_FLOAT_4,
        0x84000u,
        v812,
        g,
        "The 6th color of a bullet tracer when using StoppingPower bullets");
    dvar_cheats = _Dvar_RegisterBool("sv_cheats", 1, 0x18u, "External Dvar");
    allClientDvarsEnabled = _Dvar_RegisterBool("allClientDvarsEnabled", 0, 0x20000u, "Allow host to set all client dvars");
    Dvar_AddCommands();
}

void Dvar_LoadDvarsAddFlags(MemoryFile*, unsigned short)
{
}

void Dvar_LoadDvars(MemoryFile* memFile)
{
	if (!memFile
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(memFile)", nullptr))
	{
		__debugbreak();
	}
	Dvar_LoadDvarsAddFlags(memFile, 0);
}

void Dvar_LoadScriptInfo(MemoryFile* memFile)
{
	if (!memFile
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(memFile)", nullptr))
	{
		__debugbreak();
	}
	Dvar_LoadDvarsAddFlags(memFile, 0x400u);
}

void Dvar_ResetDvars(unsigned int filter, DvarSetSource setSource)
{
	int v2; // edi
	DvarValue* v3; // esi

	g_dvarCritSect.readCount += 1u;
	while (g_dvarCritSect.writeCount)
		NET_Sleep(0);
	v2 = 0;
	if (g_dvarCount > 0)
	{
		v3 = &s_dvarPool[0].reset;
		do
		{
			if (filter & LODWORD(v3[-3].vector.v[1]))
				Dvar_SetVariant((dvar_t*)(&v3[-4].string + 2), *v3, setSource);
			++v2;
			v3 += 6;
		} while (v2 < g_dvarCount);
	}
	if (g_dvarCritSect.readCount <= 0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(critSect->readCount > 0)",
			nullptr))
	{
		__debugbreak();
	}
	g_dvarCritSect.readCount += 0xFFFFFFFF;
}

int Com_LoadDvarsFromBufferOptional(const char** const, bool*, unsigned int, const char*, const char*)
{
	return 0;
}

void Dvar_SetBoolIfChanged(dvar_t const* dvar, bool value)
{
	if (dvar)
	{
		if (dvar->current.enabled != value)
			Dvar_SetBoolFromSource(dvar, value, DVAR_SOURCE_INTERNAL);
	}
}

void Dvar_SetIntIfChanged(dvar_t const* dvar, int value)
{
	if (dvar)
	{
		if (dvar->current.integer != value)
			Dvar_SetIntFromSource(dvar, value, DVAR_SOURCE_INTERNAL);
	}
}

void Dvar_SetFloatIfChanged(dvar_t const* dvar, float value)
{
	if (dvar)
	{
		if (dvar->current.value != value)
			Dvar_SetFloatFromSource(dvar, value, DVAR_SOURCE_INTERNAL);
	}
}

void Dvar_SetStringIfChanged(dvar_t const* dvar, const char* newString)
{
	if (dvar && newString)
	{
		if (I_strcmp(dvar->current.string, newString))
			Dvar_SetStringFromSource(dvar, newString, DVAR_SOURCE_INTERNAL);
	}
}

void Dvar_DoModifiedCallbacks(void)
{
	int v0; // ebx
	int v1; // edi
	dvarCallBack_t* v2; // esi
	int startTime; // [esp+0h] [ebp-4h]

	if (s_isDvarSystemActive)
	{
		if (!Sys_IsMainThread()
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(Sys_IsMainThread())",
				nullptr))
		{
			__debugbreak();
		}
		v0 = Dvar_GetInt(dvar_maxCallbackTimeMS);
		startTime = Sys_Milliseconds();
		g_dvarCritSect.readCount += 1u;
		while (g_dvarCritSect.writeCount)
			NET_Sleep(0);
		v1 = 0;
		if (s_nextFreeCallback > 0)
		{
			v2 = *s_dvarCallbackPool;
			while (1)
			{
				if (v2->needsCallback)
				{
					v2->callback(v2->dvar);
					v2->needsCallback = 0;
				}
				if (Sys_Milliseconds() - startTime >= v0)
					break;
				++v1;
				++v2;
				if (v1 >= s_nextFreeCallback)
					goto LABEL_15;
			}
			Com_PrintWarning(10, "Earlying out of Dvar_DoModifiedCallbacks - out of time after %ims\n", v0);
		}
	LABEL_15:
		if (g_dvarCritSect.readCount <= 0
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(critSect->readCount > 0)",
				nullptr))
		{
			__debugbreak();
		}
		g_dvarCritSect.readCount += 0xFFFFFFFF;
	}
}

int Com_LoadDvarsFromBuffer(const char** dvarnames, unsigned int numDvars, const char* buffer, const char* filename)
{
	return Com_LoadDvarsFromBufferOptional(dvarnames, 0, numDvars, buffer, filename);
}

void Dvar_Sort(void)
{
}

void Dvar_ForEach(void(* callback)(dvar_t const*, void*), void* userData)
{
	int i; // esi

	Dvar_Sort();
	for (i = 0; i < g_dvarCount; ++i)
		callback(s_sortedDvars[i], userData);
}

void Dvar_ForEachName(void(* callback)(const char*))
{
	int i; // esi
	const dvar_t* v2; // eax
	const char* v3; // eax

	Dvar_Sort();
	for (i = 0; i < g_dvarCount; ++i)
	{
		v2 = s_sortedDvars[i];
		if (v2)
			v3 = v2->name;
		else
			v3 = nullptr;
		callback(v3);
	}
}

void Dvar_ForEachName(LocalClientNum_t localClientNum, void(* callback)(LocalClientNum_t, const char*))
{
	int i; // esi
	const dvar_t* v3; // eax
	const char* v4; // eax

	Dvar_Sort();
	for (i = 0; i < g_dvarCount; ++i)
	{
		v3 = s_sortedDvars[i];
		if (v3)
			v4 = v3->name;
		else
			v4 = nullptr;
		callback(localClientNum, v4);
	}
}
