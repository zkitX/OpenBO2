#include "dvar.h"

#include <devgui/devgui.h>
#include <qcommon/common.h>
#include <universal/assertive.h>
#include <universal/com_math.h>
#include <universal/com_memory.h>
#include <win32/win_net.h>

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

DvarValue Dvar_ClampValueToDomain(dvarType_t, DvarValue, DvarValue, DvarLimits)
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

	_InterlockedExchangeAdd(&g_dvarCritSect.readCount, 1u);
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
		_InterlockedExchangeAdd(&g_dvarCritSect.readCount, 0xFFFFFFFF);
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
		_InterlockedExchangeAdd(&g_dvarCritSect.readCount, 0xFFFFFFFF);
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

int Dvar_GetInt(long)
{
	return 0;
}

unsigned int Dvar_GetUnsignedInt(const dvar_t* dvar)
{
	if (dvar)
		return dvar->current.unsignedInt;
	return 0;
}

float Dvar_GetFloat(long)
{
	return 0.0f;
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
