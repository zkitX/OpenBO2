#include "dvar.h"

#include <emmintrin.h>

#include "../qcommon/cmd.h"
#include "com_memory.h"
#include "assertive.h"

void Dvar_AddCommands()
{
	Cmd_AddCommandInternal("toggle", (void(__cdecl*)())dvar::Dvar_Toggle_f, &Dvar_Toggle_f_VAR);
	Cmd_AddCommandInternal("togglep", (void(__cdecl*)())dvar::Dvar_TogglePrint_f, &Dvar_TogglePrint_f_VAR);
	Cmd_AddCommandInternal("set", (void(__cdecl*)())dvar::Dvar_Set_f, &Dvar_Set_f_VAR);
	Cmd_AddCommandInternal("seta", (void(__cdecl*)())dvar::Dvar_SetA_f, &Dvar_SetA_f_VAR);
	Cmd_AddCommandInternal("setdvartotime", (void(__cdecl*)())Dvar_SetToTime_f, &Dvar_SetToTime_f_VAR);
	Cmd_AddCommandInternal("reset", (void(__cdecl*)())dvar::Dvar_Reset_f, &Dvar_Reset_f_VAR);
	Cmd_AddCommandInternal("dvarlist", (void(__cdecl*)())dvar::Dvar_List_f, &Dvar_List_f_VAR);
	Cmd_AddCommandInternal("dvardump", Dvar_Dump_f, &Dvar_Dump_f_VAR);
	Cmd_AddCommandInternal("dvar_bool", (void(__cdecl*)())dvar::Dvar_RegisterBool_f, &Dvar_RegisterBool_f_VAR);
	Cmd_AddCommandInternal("dvar_int", (void(__cdecl*)())dvar::Dvar_RegisterInt_f, &Dvar_RegisterInt_f_VAR);
	Cmd_AddCommandInternal("dvar_float", (void(__cdecl*)())dvar::Dvar_RegisterFloat_f, &Dvar_RegisterFloat_f_VAR);
	Cmd_AddCommandInternal("dvar_color", Dvar_RegisterColor_f, &Dvar_RegisterColor_f_VAR);
	Cmd_AddCommandInternal("dvarAddConfigFlag", Dvar_AddConfigFlag_f, &Dvar_AddConfigFlag_f_VAR);
	Cmd_AddCommandInternal("restoreDvars", (void(__cdecl*)())BG_EvalVehicleName, &Dvar_RestoreDvars_VAR);
}

void Dvar_AddConfigFlag_f()
{
	const char* v0; // eax
	dvar_t* v1; // eax

	if (Cmd_Argc() == 2)
	{
		v0 = Cmd_Argv(1);
		v1 = Dvar_FindVar((const char*)v0);
		if (v1)
		{
			Dvar_AddFlags(v1, 0x20000);
		}
#ifdef _DEBUG
		else if (!(unsigned char)Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvar)",
			(const char*)&scratch))
		{
			__debugbreak();
		}
#endif // _DEBUG
	}
}

void Dvar_AddFlags(const dvar_t* dvar, int flags)
{
}

char Dvar_CanSetConfigDvar(const dvar_t* dvar)
{
	return 0;
}

void Dvar_ChangeResetValue(const dvar_t* dvar, DvarValue value)
{
}

void Dvar_ClearModified(const dvar_t* dvar)
{
}

dvar_t* Dvar_Command()
{
	return nullptr;
}

const char* Dvar_DisplayableLatchedValue(const dvar_t* dvar)
{
	return nullptr;
}

const char* Dvar_DisplayableResetValue(const dvar_t* dvar)
{
	return nullptr;
}

const char* Dvar_DisplayableValue(const dvar_t* dvar)
{
	return nullptr;
}

void Dvar_DoModifiedCallbacks()
{
}

const char* Dvar_DomainToString_GetLines(dvarType_t type, DvarLimits domain, char* outBuffer, int outBufferLen, int* outLineCount)
{
	return nullptr;
}

void Dvar_Dump_f()
{
}

GfxViewParms* Dvar_EnumToString(const dvar_t* dvar)
{
	return nullptr;
}

dvar_t* Dvar_FindVar(int dvarHash)
{
	return nullptr;
}

dvar_t* Dvar_FindVar(const char* dvarName)
{
	return nullptr;
}

void Dvar_ForEach(void(__cdecl* callback)(const dvar_t*, void*), void* userData)
{
}

void Dvar_ForEachName(LocalClientNum_t localClientNum, void(__cdecl* callback)(LocalClientNum_t, const char*))
{
}

void Dvar_ForEachName(void(__cdecl* callback)(const char*))
{
}

bool Dvar_GetBool(const dvar_t* dvar)
{
	return false;
}

void Dvar_GetColor(const dvar_t* dvar, char* color)
{
}

double Dvar_GetColorAlpha(const dvar_t* dvar)
{
	return 0.0;
}

double Dvar_GetColorAlpha(int dvarHash)
{
	return 0.0;
}

double Dvar_GetColorBlue(const dvar_t* dvar)
{
	return 0.0;
}

double Dvar_GetColorBlue(int dvarHash)
{
	return 0.0;
}

double Dvar_GetColorGreen(const dvar_t* dvar)
{
	return 0.0;
}

double Dvar_GetColorGreen(int dvarHash)
{
	return 0.0;
}

double Dvar_GetColorRed(const dvar_t* dvar)
{
	return 0.0;
}

double Dvar_GetColorRed(int dvarHash)
{
	return 0.0;
}

DvarValue* Dvar_GetCurrent(DvarValue* result, const dvar_t* dvar)
{
	return nullptr;
}

const char* Dvar_GetDescription(const dvar_t* dvar)
{
	return nullptr;
}

DvarLimits* Dvar_GetDomain(DvarLimits* result, const dvar_t* dvar)
{
	return nullptr;
}

int Dvar_GetDomainEnumStringCount(const dvar_t* dvar)
{
	return 0;
}

const char** Dvar_GetDomainEnumStrings(const char** result)
{
	return nullptr;
}

double Dvar_GetDomainFloatMax(const dvar_t* dvar)
{
	return 0.0;
}

double Dvar_GetDomainFloatMin(const dvar_t* dvar)
{
	return 0.0;
}

long long Dvar_GetDomainInt64Max(const dvar_t* dvar)
{
	return 0;
}

long long Dvar_GetDomainInt64Min(const dvar_t* dvar)
{
	return 0;
}

int Dvar_GetDomainIntMax(const dvar_t* dvar)
{
	return 0;
}

int Dvar_GetDomainIntMin(const dvar_t* dvar)
{
	return 0;
}

double Dvar_GetDomainVecMax(const dvar_t* dvar)
{
	return 0.0;
}

double Dvar_GetDomainVecMin(const dvar_t* dvar)
{
	return 0.0;
}

const dvar_t* Dvar_GetFlags(const dvar_t* dvar)
{
	return nullptr;
}

double Dvar_GetFloat(const dvar_t* dvar)
{
	return 0.0;
}

double Dvar_GetFloat(int dvarHash)
{
	return 0.0;
}

int Dvar_GetInt(const dvar_t* dvar)
{
	return 0;
}

int Dvar_GetInt(int dvarHash)
{
	return 0;
}

long long Dvar_GetInt64(const dvar_t* dvar)
{
	return 0;
}

long long Dvar_GetInt64(int dvarHash)
{
	return 0;
}

bool Dvar_GetLatchedBool(const dvar_t* dvar)
{
	return false;
}

void Dvar_GetLatchedColor(const dvar_t* dvar, char* color)
{
}

double Dvar_GetLatchedFloat(const dvar_t* dvar)
{
	return 0.0;
}

int Dvar_GetLatchedInt(const dvar_t* dvar)
{
	return 0;
}

void Dvar_GetLatchedVec2(const dvar_t* dvar, vec2_t* result)
{
}

void Dvar_GetLatchedVec3(const dvar_t* dvar, vec3_t* result)
{
}

void Dvar_GetLatchedVec4(const dvar_t* dvar, vec4_t* result)
{
}

bool Dvar_GetModified(const dvar_t* dvar)
{
	return false;
}

const char* Dvar_GetName(const dvar_t* dvar)
{
	return nullptr;
}

int Dvar_GetResetInt(const dvar_t* dvar)
{
	return 0;
}

const char* Dvar_GetResetString(const dvar_t* dvar)
{
	return nullptr;
}

void Dvar_GetResetVec3(const dvar_t* dvar, vec3_t* result)
{
}

const char* Dvar_GetString(const dvar_t* dvar)
{
	return nullptr;
}

dvarType_t Dvar_GetType(const dvar_t* dvar)
{
	return dvarType_t();
}

void Dvar_GetUnpackedColor(const dvar_t* dvar, vec4_t* expandedColor)
{
}

const dvar_t* Dvar_GetUnsignedInt(const dvar_t* dvar)
{
	return nullptr;
}

const char* Dvar_GetVariantString(const dvar_t* dvar)
{
	return nullptr;
}

const char* Dvar_GetVariantString(int dvarHash)
{
	return nullptr;
}

void Dvar_GetVec2(const dvar_t* dvar, vec2_t* result)
{
}

void Dvar_GetVec3(const dvar_t* dvar, vec3_t* result)
{
}

void Dvar_GetVec4(const dvar_t* dvar, vec4_t* result)
{
}

bool Dvar_HasLatchedValue(const dvar_t* dvar)
{
	return false;
}

GfxViewParms* Dvar_IndexStringToEnumString(const dvar_t* dvar, const char* indexString)
{
	return nullptr;
}

char* Dvar_InfoString(LocalClientNum_t localClientNum, int bit)
{
	return nullptr;
}

void Dvar_InfoStringSingle(const dvar_t* dvar, void* userData)
{
}

void Dvar_InfoStringSingle_Big(const dvar_t* dvar, void* userData)
{
}

char* Dvar_InfoString_Big(int bit)
{
	return nullptr;
}

bool Dvar_IsSystemActive()
{
	return false;
}

char Dvar_IsValidName(const char* dvarName)
{
	return 0;
}

void Dvar_ListSingle(const dvar_t* dvar, void* userData)
{
}

void Dvar_MakeLatchedValueCurrent(const dvar_t* dvar)
{
}

void Dvar_RegisterColor_f()
{
}

void Dvar_Reset(const dvar_t* dvar, DvarSetSource setSource)
{
}

void Dvar_ResetDvars(unsigned int filter, DvarSetSource setSource)
{
}

void Dvar_SetBool(const dvar_t* dvar, bool value)
{
}

void Dvar_SetBoolFromSource(const dvar_t* dvar, bool value, DvarSetSource source)
{
}

void Dvar_SetBoolIfChanged(const dvar_t* dvar, bool value)
{
}

void Dvar_SetCanSetConfigDvars(bool canSetConfigDvars)
{
}

void Dvar_SetCheatState()
{
}

void Dvar_SetColor(const dvar_t* dvar, float r, float g, float b, float a)
{
}

void Dvar_SetColorFromSource(const dvar_t* dvar, float r, float g, float b, float a, DvarSetSource source)
{
}

void Dvar_SetCommand(const char* dvarName, const char* string)
{
}

void Dvar_SetFloat(const dvar_t* dvar, float value)
{
}

void Dvar_SetFloatFromSource(const dvar_t* dvar, float value, DvarSetSource source)
{
}

void Dvar_SetFloatIfChanged(const dvar_t* dvar, float value)
{
}

void Dvar_SetFromString(const dvar_t* dvar, const char* string)
{
}

void Dvar_SetFromStringByName(const char* dvarName, const char* string)
{
}

const dvar_t* Dvar_SetFromStringByNameFromSource(const char* dvarName, const char* string, DvarSetSource source, unsigned int flags)
{
	return nullptr;
}

void Dvar_SetFromStringFromSource(const dvar_t* dvar, const char* string, DvarSetSource source)
{
}

void Dvar_SetInAutoExec(bool inAutoExec)
{
}

void Dvar_SetInt(const dvar_t* dvar, int value)
{
}

void Dvar_SetInt64FromSource(const dvar_t* dvar, long long value, DvarSetSource source)
{
}

void Dvar_SetIntFromSource(const dvar_t* dvar, int value, DvarSetSource source)
{
}

void Dvar_SetIntIfChanged(const dvar_t* dvar, int value)
{
}

void Dvar_SetLatchedValue(DvarValue value)
{
}

void Dvar_SetModified(const dvar_t* dvar)
{
}

void Dvar_SetModifiedCallback(const dvar_t* dvar, void(__cdecl* callback)(const dvar_t*))
{
}

const dvar_t* Dvar_SetOrRegisterString(const dvar_t* dvar, const char* dvarName, const char* value)
{
	return nullptr;
}

void Dvar_SetString(const dvar_t* dvar, const char* value)
{
}

void Dvar_SetStringIfChanged(const dvar_t* dvar, const char* newString)
{
}

void Dvar_SetVec2(const dvar_t* dvar, float x, float y)
{
}

void Dvar_SetVec2FromSource(const dvar_t* dvar, float x, float y, DvarSetSource source)
{
}

void Dvar_SetVec3(const dvar_t* dvar, float x, float y, float z)
{
}

void Dvar_SetVec3FromSource(const dvar_t* dvar, float x, float y, float z, DvarSetSource source)
{
}

void Dvar_SetVec4(const dvar_t* dvar, float x, float y, float z, float w)
{
}

void Dvar_SetVec4FromSource(const dvar_t* dvar, float x, float y, float z, float w, DvarSetSource source)
{
}

void Dvar_SetVec4FromVec4(const dvar_t* dvar, vec4_t* vecin)
{
}

bool Dvar_ShouldFreeCurrentString(dvar_t* dvar)
{
	return false;
}

bool Dvar_ShouldFreeLatchedString(dvar_t* dvar)
{
	return false;
}

bool Dvar_ShouldFreeResetString(dvar_t* dvar)
{
	return false;
}

void Dvar_Shutdown()
{
}

void Dvar_Sort()
{
}

void Dvar_UpdateEnumDomain(const dvar_t* dvar, const char** stringTable)
{
	char* v2; // eax
	int v3; // esi
	int v4; // eax
	char* v5; // eax
	dvarType_t v6; // ecx
	DvarLimits v7; // ST20_16
	DvarValue v8; // ST10_16
	DvarValue v9; // ST00_16
	DvarValue* v10; // eax
	__m128i v11; // xmm0
	__m128i v12; // xmm1
	char v13; // [esp+Ch] [ebp-10h]

#ifdef _DEBUG
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", (const char*)&scratch))
	{
		__debugbreak();
	}
	if (!dvar->name
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvar->name)",
			(const char*)&scratch))
	{
		__debugbreak();
	}
	if (!stringTable
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"((stringTable))",
			"(dvar->name) = %s",
			dvar->name))
	{
		__debugbreak();
	}
	if (dvar->type != 7)
	{
		v2 = va("dvar %s type %i", dvar->name, dvar->type);
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvar->type == DVAR_TYPE_ENUM)",
			"%s",
			v2))
			__debugbreak();
	}
#endif // _DEBUG
	v3 = 0;
	if (*stringTable)
	{
		do
			++v3;
		while (stringTable[v3]);
	}
	v4 = dvar->reset.integer;
	if (v4 < 0 || v4 >= v3 && v4)
	{
		v5 = va("name %s reset %i count %i", dvar->name, v4, v3);
#ifdef _DEBUG
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvar->reset.integer >= 0 && (dvar->reset.integer < stringCount || dvar->reset.integer == 0))",
			"%s",
			v5))
			__debugbreak();
#endif // _DEBUG
	}
	v6 = dvar->type;
	dvar->domain.enumeration.stringCount = v3;
	dvar->domain.integer.max = (int)stringTable;
	v7.integer64.min = dvar->domain.integer64.min;
	v7.integer64.max = dvar->domain.integer64.max;
	v8.integer64 = dvar->reset.integer64;
	*((unsigned long long*) & v8.string + 1) = *((unsigned long long*) & dvar->reset.string + 1);
	v9.integer64 = dvar->current.integer64;
	*((unsigned long long*) & v9.string + 1) = *((unsigned long long*) & dvar->current.string + 1);
	v10 = dvar::Dvar_ClampValueToDomain(v6, (int)&v13, v9, v8, v7);
	v11 = _mm_loadl_epi64((const __m128i*)v10);
	v12 = _mm_loadl_epi64((const __m128i*)(&v10->string + 2));
	_mm_storel_epi64((__m128i*) & dvar->current, v11);
	_mm_storel_epi64((__m128i*) & dvar->latched, v11);
	_mm_storel_epi64((__m128i*)(&dvar->current.string + 2), v12);
	_mm_storel_epi64((__m128i*)(&dvar->latched.string + 2), v12);
}

void Dvar_UpdateResetValue(DvarValue value)
{
	dvar_t* dvar; // ecx
	dvar_t* v2; // edi
	const char* v3; // edx
	bool v4; // bl
	const char* oldString; // [esp+0h] [ebp-24h]
	DvarValue resetString; // [esp+10h] [ebp-14h]

	v2 = dvar;
	resetString = value;
#ifdef _DEBUG
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", (const char*)&scratch))
	{
		__debugbreak();
	}
#endif // _DEBUG
	switch (v2->type)
	{
	case 3:
		v2->reset.integer64 = resetString.integer64;
		break;
	case 4:
	case 0xB:
	case 0xC:
		v2->reset.integer64 = resetString.integer64;
		v2->reset.vector.v[2] = resetString.vector.v[2];
		break;
	case 5:
		v2->reset.integer64 = resetString.integer64;
		*((unsigned long long*) & v2->reset.string + 1) = *((unsigned long long*) & resetString.string + 1);
		break;
	case 6:
	case 7:
	case 9:
	case 0xA:
		_mm_storel_epi64((__m128i*) & v2->reset, _mm_loadl_epi64((const __m128i*) & resetString));
		_mm_storel_epi64((__m128i*)(&v2->reset.string + 2), _mm_loadl_epi64((const __m128i*)(&resetString.string + 2)));
		break;
	case 8:
		if (v2->reset.integer != resetString.integer)
		{
			v4 = Dvar_ShouldFreeResetString(v2);
			if (v4)
				oldString = v3;
			dvar::Dvar_AssignResetStringValue(v2, &resetString, resetString.string);
			v2->reset.integer = resetString.integer;
			if (v4)
				FreeString(oldString, 12);
		}
		break;
	}
}

void Dvar_UpdateValue(dvar_t* dvar, DvarValue value)
{
	const char* v2; // edx
	const char* v3; // esi
	long long v4; // kr00_8
	int v5; // xmm0_4
	long long v6; // kr08_8
	int v7; // xmm0_4
	long long v8; // kr10_8
	float v9; // xmm3_4
	__m128i v10; // xmm0
	__m128i v11; // xmm1
	const char* oldString; // [esp+0h] [ebp-28h]
	bool shouldFree; // [esp+13h] [ebp-15h]
	DvarValue currentString; // [esp+14h] [ebp-14h]

	currentString = value;
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", (const char*)&scratch))
	{
		__debugbreak();
	}
	switch (dvar->type)
	{
	case 3:
		v4 = currentString.integer64;
		dvar->current.integer64 = currentString.integer64;
		dvar->latched.integer64 = v4;
		break;
	case 4:
	case 0xB:
	case 0xC:
		v5 = currentString.integer;
		v6 = *(long long*)((char*)&currentString.integer64 + 4);
		dvar->current.integer = currentString.integer;
		*(long long*)((char*)&dvar->current.integer64 + 4) = v6;
		dvar->latched.integer = v5;
		*(long long*)((char*)&dvar->latched.integer64 + 4) = v6;
		break;
	case 5:
		v7 = currentString.integer;
		v8 = *(long long*)((char*)&currentString.integer64 + 4);
		v9 = currentString.vector.v[3];
		dvar->current.integer = currentString.integer;
		*(long long*)((char*)&dvar->current.integer64 + 4) = v8;
		dvar->current.vector.v[3] = v9;
		dvar->latched.integer = v7;
		*(long long*)((char*)&dvar->latched.integer64 + 4) = v8;
		dvar->latched.vector.v[3] = v9;
		break;
	case 6:
	case 7:
	case 9:
	case 0xA:
		v10 = _mm_loadl_epi64((const __m128i*) & currentString);
		v11 = _mm_loadl_epi64((const __m128i*)(&currentString.string + 2));
		_mm_storel_epi64((__m128i*) & dvar->current, v10);
		_mm_storel_epi64((__m128i*) & dvar->latched, v10);
		_mm_storel_epi64((__m128i*)(&dvar->current.string + 2), v11);
		_mm_storel_epi64((__m128i*)(&dvar->latched.string + 2), v11);
		break;
	case 8:
		if (currentString.integer != dvar->current.integer)
		{
			shouldFree = Dvar_ShouldFreeCurrentString(dvar);
			if (shouldFree)
				oldString = v2;
			dvar::Dvar_AssignCurrentStringValue(dvar, &currentString, currentString.string);
			dvar->current.integer = currentString.integer;
			if ((unsigned char)Dvar_ShouldFreeLatchedString(dvar))
				dvar::Dvar_FreeString(&dvar->latched);
			v3 = dvar->current.string;
			dvar->latched.integer = 0;
			dvar::Dvar_WeakCopyString(v3, &dvar->latched);
			if (shouldFree)
				FreeString(oldString, 12);
		}
		break;
	}
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
	DvarValue v11; // [esp+0h] [ebp-14h]

	v11 = value;
	switch (type)
	{
	case 1:
#ifdef _DEBUG
		if (v11.enabled != 1
			&& v11.enabled
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(value.enabled == 1 || value.enabled == 0)",
				(const char*)&scratch))
		{
			__debugbreak();
		}
#endif // _DEBUG
		return 1;
	case 2:
		if (domain.value.min > v11.value || v11.value > domain.value.max)
			return 0;
		return 1;
	case 3:
		v5 = 0;
		while (1)
		{
			v6 = *((float*)&v11.integer + v5);
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
			v8 = *((float*)&v11.integer + v7);
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
#ifdef _DEBUG
		if (domain.enumeration.stringCount > domain.integer.max
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(domain.integer.min <= domain.integer.max)",
				(const char*)&scratch))
		{
			__debugbreak();
		}
#endif
		if (v11.integer < domain.enumeration.stringCount)
			return 0;
		return v11.integer <= domain.integer.max;
	case 7:
		v4 = v11.integer == 0;
		if (v11.integer < 0)
			goto LABEL_28;
		if (v11.integer < domain.enumeration.stringCount)
			goto LABEL_49;
		v4 = v11.integer == 0;
	LABEL_28:
		if (v4)
			LABEL_49 :
			result = 1;
		else
			result = 0;
		return result;
	case 8:
	case 9:
		return 1;
	case 10:
#ifdef _DEBUG
		if (domain.integer64.min > domain.integer64.max
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(domain.integer64.min <= domain.integer64.max)",
				(const char*)&scratch))
		{
			__debugbreak();
		}
#endif
		if (SLODWORD(v11.vector.v[1]) <= domain.integer.max
			&& (SLODWORD(v11.vector.v[1]) < domain.integer.max || (unsigned int)v11.integer < domain.enumeration.stringCount)
			|| SLODWORD(v11.vector.v[1]) >= *((unsigned int*)&domain.vector + 3)
			&& (SLODWORD(v11.vector.v[1]) > * ((unsigned int*)&domain.vector + 3)
				|| (unsigned int)v11.integer > * ((unsigned int*)&domain.vector + 2)))
		{
			return 0;
		}
		return 1;
	}
	while (1)
	{
		v10 = *((float*)&v11.integer + v9);
		if (domain.value.min > v10 || v10 > domain.value.max)
			break;
		if (++v9 >= 4)
			return 1;
	}
	return 0;
}

int Dvar_ValuesEqual(DvarValue val0, DvarValue val1)
{
	dvarType_t type; // ecx
	int result; // eax
	vec4_t b; // [esp+0h] [ebp-24h]
	vec4_t a; // [esp+10h] [ebp-14h]

	a = val0.vector;
	b = val1.vector;
	switch (type)
	{
	case 1:
		result = LOBYTE(a.v[0]) == LOBYTE(b.v[0]);
		break;
	case 2:
		if (a.v[0] != b.v[0])
			goto LABEL_24;
		result = 1;
		break;
	case 3:
		if (a.v[0] != b.v[0] || a.v[1] != b.v[1])
			goto LABEL_24;
		result = 1;
		break;
	case 4:
	case 11:
	case 12:
		if (a.v[0] != b.v[0] || a.v[1] != b.v[1] || a.v[2] != b.v[2])
			goto LABEL_24;
		result = 1;
		break;
	case 5:
		result = Vec4Compare(&a, &b);
		break;
	case 6:
	case 7:
		result = LODWORD(a.v[0]) == LODWORD(b.v[0]);
		break;
	case 8:
		if (!LODWORD(a.v[0])
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(val0.string)",
				(const char*)&scratch))
		{
			__debugbreak();
		}
		if (!LODWORD(b.v[0])
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(val1.string)",
				(const char*)&scratch))
		{
			__debugbreak();
		}
		result = strcmp((const char*)LODWORD(a.v[0]), (const char*)LODWORD(b.v[0])) == 0;
		break;
	case 9:
		result = LODWORD(a.v[0]) == LODWORD(b.v[0]);
		break;
	case 10:
		if (*(unsigned long long*)a.v == *(unsigned long long*)b.v)
			result = 1;
		else
			LABEL_24 :
			result = 0;
		break;
	}
	return result;
}

void Dvar_WriteDefaults(int f)
{
	Dvar_ForEach(Dvar_WriteSingleDefault, &f);
}

void Dvar_WriteSingleDefault(const dvar_t* dvar, void* userData)
{
	const char* v2; // eax
	int v3; // edi
	const char* v4; // ST0C_4
	const char* v5; // eax

	v2 = Dvar_GetName(dvar);
	if (q_shared::I_stricmp(v2, "cl_cdkey"))
	{
		if (!((unsigned int)Dvar_GetFlags(dvar) & 0x40C0))
		{
			v3 = *(unsigned int*)userData;
			v4 = Dvar_DisplayableResetValue(dvar);
			v5 = Dvar_GetName(dvar);
			FS_Printf(v3, "set %s \"%s\"\n", v5, v4);
		}
	}
}

void Dvar_WriteSingleVariable(const dvar_t* dvar, void* userData)
{
	const char* v2; // eax
	int v3; // edi
	const char* v4; // ST0C_4
	const char* v5; // eax

	v2 = Dvar_GetName(dvar);
	if (q_shared::I_stricmp(v2, "cl_cdkey"))
	{
		if ((unsigned int)Dvar_GetFlags(dvar) & 1)
		{
			v3 = *(unsigned int*)userData;
			v4 = Dvar_DisplayableLatchedValue(dvar);
			v5 = Dvar_GetName(dvar);
			FS_Printf(v3, "seta %s \"%s\"\n", v5, v4);
		}
	}
}

void Dvar_WriteVariables(int f)
{
	Dvar_ForEach(Dvar_WriteSingleVariable, &f);
}

dvar_t* _Dvar_RegisterBool(const char* dvarName, bool value, unsigned int flags, const char* description)
{
	return nullptr;
}

dvar_t* _Dvar_RegisterFloat(const char* dvarName, float value, float min, float max, unsigned int flags, const char* description)
{
	DvarLimits v6; // ST18_16
	DvarValue v7; // ST08_16
	DvarValue dvarValue; // [esp+0h] [ebp-20h]
	DvarLimits dvarDomain; // [esp+10h] [ebp-10h]

	dvarValue.value = value;
	dvarDomain.integer64.min = __PAIR__(LODWORD(max), LODWORD(min));
	v6.integer64.min = __PAIR__(LODWORD(max), LODWORD(min));
	v6.integer64.max = dvarDomain.integer64.max;
	v7.integer64 = dvarValue.integer64;
	*((unsigned long long*)&v7.string + 1) = *((unsigned long long*)&dvarValue.string + 1);
	return dvar::Dvar_RegisterVariant(dvarName, DVAR_TYPE_FLOAT, flags, v7, v6, description);
}

dvar_t* Dvar_RegisterVariant(const char* dvarName, dvarType_t type, unsigned int flags, DvarValue value, DvarLimits domain, const char* description)
{

}

void dvar::Dvar_AssignCurrentStringValue(dvar_t* dvar, DvarValue* dest, const char* string)
{
}

void dvar::Dvar_AssignLatchedStringValue(dvar_t* dvar, DvarValue* dest, const char* string)
{
}

void dvar::Dvar_AssignResetStringValue(dvar_t* dvar, DvarValue* dest, const char* string)
{
}

char dvar::Dvar_CanChangeValue(const dvar_t* dvar, DvarSetSource source, DvarValue value)
{
	return 0;
}

DvarValue* dvar::Dvar_ClampValueToDomain(dvarType_t type, int a2, DvarValue value, const DvarValue resetValue, const DvarLimits domain)
{
	return nullptr;
}

void dvar::Dvar_CopyString(const char* string, DvarValue* value)
{
}

const char* dvar::Dvar_DomainToString_Internal(char* outBuffer, int outBufferLen, dvarType_t type, DvarLimits domain, int* outLineCount)
{
	return nullptr;
}

dvar_t* dvar::Dvar_FindMalleableVar(int dvarHash)
{
	return nullptr;
}

dvar_t* dvar::Dvar_FindMalleableVar(const char* dvarName)
{
	return nullptr;
}

void dvar::Dvar_FreeString(DvarValue* value)
{
}

void dvar::Dvar_Init(int a1)
{
}

void dvar::Dvar_List_f()
{
}

void dvar::Dvar_LoadDvars(MemoryFile* memFile)
{
}

void dvar::Dvar_LoadDvarsAddFlags(MemoryFile* memFile, unsigned short flags)
{
}

void dvar::Dvar_LoadScriptInfo(MemoryFile* memFile)
{
}

void dvar::Dvar_MakeExplicitType(dvar_t* dvar, dvarType_t type, const char* dvarName, unsigned int flags, DvarValue resetValue, DvarLimits domain)
{
}

void dvar::Dvar_PerformUnregistration(dvar_t* dvar)
{
}

void dvar::Dvar_PrintDomain(dvarType_t type, DvarLimits domain)
{
}

void dvar::Dvar_RegisterBool_f()
{
}

void dvar::Dvar_RegisterFloat_f()
{
}

void dvar::Dvar_RegisterInt_f()
{
}

dvar_t* dvar::Dvar_RegisterNew(const char* dvarName, dvarType_t type, unsigned int flags, DvarValue value, DvarLimits domain, const char* description)
{
	return nullptr;
}

dvar_t* dvar::Dvar_RegisterVariant(const char* dvarName, dvarType_t type, unsigned int flags, DvarValue value, DvarLimits domain, const char* description)
{
	dvar_t* dvart;
	if (!(flags & 0x4000)
		&& !CanKeepStringPointer(dvarName)
		&& !(unsigned __int8)Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(((flags & (1 << 14)) || CanKeepStringPointer( dvarName )))",
			"(dvarName) = %s",
			dvarName))
	{
		__debugbreak();
	}
	dvart = Dvar_FindMalleableVar(dvarName);
	if (dvart)
	{

	}
}

void dvar::Dvar_ReinterpretDvar(dvar_t* dvar, unsigned int flags, const char* dvarName, dvarType_t type, DvarValue value, DvarLimits domain)
{
}

void dvar::Dvar_Reregister(dvar_t* dvar, const char* dvarName, dvarType_t type, unsigned int flags, DvarValue resetValue, DvarLimits domain, const char* description)
{
}

void dvar::Dvar_Reset_f()
{
}

void dvar::Dvar_SetA_f()
{
}

void dvar::Dvar_SetStringFromSource(const dvar_t* dvar, const char* string, DvarSetSource source)
{
}

void dvar::Dvar_SetToTime_f()
{
}

void dvar::Dvar_SetVariant(dvar_t* dvar, DvarValue value, DvarSetSource source)
{
}

void dvar::Dvar_Set_f()
{
}

bool dvar::Dvar_StringToBool(const char* string)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	return atoi(string) != 0;
}

void dvar::Dvar_StringToColor(char* color, const char* string)
{
	float v2; // xmm1_4
	LocalClientNum_t v3; // xmm2_4
	float v4; // xmm0_4
	signed int v5; // ST24_4
	float v6; // xmm1_4
	bool v7; // cf
	signed int v8; // ST24_4
	float v9; // xmm1_4
	signed int v10; // ST24_4
	float v11; // xmm1_4
	vec4_t colorVec; // [esp+10h] [ebp-14h]

	*(unsigned long long*)colorVec.v = 0i64;
	*(unsigned long long*)& colorVec.b = 0i64;
	sscanf(string, "%g %g %g %g", &colorVec, &colorVec.g, &colorVec.b, &colorVec.a);
	v2 = colorVec.v[0];
	v3 = (LocalClientNum_t)1.0;
	v4 = 0.0;
	if ((float)(colorVec.v[0] - 1.0) >= 0.0)
		v2 = 1.0;
	if ((0.00000000 <= (float)((uint)v2 ^ 0x80000000)))
		v2 = 0.0;
	v5 = (signed int)((float)(v2 * 255.0) + 9.313225746154785e-10);
	v6 = colorVec.v[1];
	v7 = (float)(colorVec.v[1] - 1.0) < 0.0;
	*color = v5;
	if (!v7)
		v6 = 1.0;
	if (0.00000000 <= (float)((uint)v6 ^ 0x80000000))
		v6 = 0.0;
	v8 = (signed int)((float)(v6 * 255.0) + 9.313225746154785e-10);
	v9 = colorVec.v[2];
	v7 = (float)(colorVec.v[2] - 1.0) < 0.0;
	color[1] = v8;
	if (!v7)
		v9 = 1.0;
	if (0.00000000 <= (float)((uint)v9 ^ 0x80000000))
		v9 = 0.0;
	v10 = (signed int)((float)(v9 * 255.0) + 9.313225746154785e-10);
	v11 = colorVec.v[3];
	v7 = (float)(colorVec.v[3] - 1.0) < 0.0;
	color[2] = v10;
	if (v7)
		*(float*)&v3 = v11;
	if ((float)((uint)v3 ^ 0x80000000) < 0.00000000)
		v4 = *(float*)&v3;
	color[3] = (signed int)((float)(v4 * 255.0) + 9.313225746154785e-10);
}

int dvar::Dvar_StringToEnum(const DvarLimits* domain, const char* string)
{
	int v2; // esi
	char v3; // cl
	int result; // eax
	const char* v5; // edx
	int v6; // ebx

	if (!domain
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(domain)", (const char*)&scratch))
	{
		__debugbreak();
	}
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	v2 = 0;
	if (domain->enumeration.stringCount > 0)
	{
		while (q_shared::I_stricmp(string, domain->enumeration.strings[v2]))
		{
			if (++v2 >= domain->enumeration.stringCount)
				goto LABEL_10;
		}
		return v2;
	}
LABEL_10:
	v3 = *string;
	result = 0;
	v5 = string;
	if (*string)
	{
		while (v3 >= 48 && v3 <= 57)
		{
			++v5;
			result = v3 + 10 * result - 48;
			v3 = *v5;
			if (!*v5)
			{
				if (result < 0)
					goto LABEL_16;
				goto LABEL_15;
			}
		}
	}
	else
	{
	LABEL_15:
		if (result < domain->enumeration.stringCount)
			return result;
	LABEL_16:
		v6 = strlen(string);
		v2 = 0;
		if (domain->enumeration.stringCount > 0)
		{
			while (q_shared::I_strnicmp(string, domain->enumeration.strings[v2], v6))
			{
				if (++v2 >= domain->enumeration.stringCount)
					return -1337;
			}
			return v2;
		}
	}
	return -1337;
}

double dvar::Dvar_StringToFloat(const char* string)
{
	int result; // eax

	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	atof(string);
	return result;
}

int dvar::Dvar_StringToInt(const char* string)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	return atoi(string);
}

long long dvar::Dvar_StringToInt64(const char* string)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	return I_atoi64(string);
}

DvarValue* dvar::Dvar_StringToValue(const dvarType_t type, const char* string, double a3, const DvarLimits domain)
{
	const char* v5; // edi
	dvarType_t dvarType; // esi
	float value;

	v5 = string;
	dvarType = type;
	if (!string
		&& !Assert_MyHandler("c:\\t6\\code\\src_noserver\\universal\\dvar.cpp", 582, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	switch (dvarType)
	{
	case DVAR_TYPE_BOOL:
		*(unsigned char*)domain.enumeration.stringCount = Dvar_StringToBool(v5);
		break;
	case DVAR_TYPE_FLOAT:
		value = Dvar_StringToFloat(v5);
		*(float*)domain.enumeration.stringCount = value;
		break;
	case 3:
		Dvar_StringToVec2(v5, (vec2_t*)domain.enumeration.stringCount);
		break;
	case 4:
	case 11:
	case 12:
		Dvar_StringToVec3(v5, (vec3_t*)domain.enumeration.stringCount);
		break;
	case 5:
		Dvar_StringToVec4(v5, (vec4_t*)domain.enumeration.stringCount);
		break;
	case 6:
		*(unsigned int*)domain.enumeration.stringCount = Dvar_StringToInt(v5);
		break;
	case 7:
		*(unsigned int*)domain.enumeration.stringCount = Dvar_StringToEnum((const DvarLimits*)((char*)&domain + 4), v5);
		break;
	case 8:
		*(unsigned int*)domain.enumeration.stringCount = (int)v5;
		break;
	case 9:
		Dvar_StringToColor((char*)v5, (char*)domain.enumeration.stringCount);
		break;
	case 10:
		*(unsigned long long*)domain.enumeration.stringCount = Dvar_StringToInt64(v5);
		break;
	}
}

void dvar::Dvar_StringToVec2(const char* string, vec2_t* vector)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	sscanf(string, "%g %g", vector, &vector->y);
}

void dvar::Dvar_StringToVec3(const char* string, vec3_t* vector)
{
	char* v2; // [esp-8h] [ebp-8h]
	char* v3; // [esp-4h] [ebp-4h]

	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	v3 = (char*)&vector->v[2];
	*(unsigned long long*)& vector->x = 0i64;
	vector->v[2] = 0.0;
	v2 = (char*)&vector->v[1];
	if (*string == 40)
		sscanf(string, "( %g, %g, %g )", vector, v2, v3);
	else
		sscanf(string, "%g %g %g", vector, v2, v3);
}

void dvar::Dvar_StringToVec4(const char* string, vec4_t* vector)
{
#ifdef _DEBUG
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
#endif // _DEBUG
	* (unsigned long long*)vector->v = 0i64;
	*(unsigned long long*)& vector->b = 0i64;
	sscanf(string, "%g %g %g %g", vector, &vector->g, &vector->b, &vector->a);
}

bool dvar::Dvar_ToggleInternal()
{
	GfxViewParms* v0; // eax
	char result; // al
	GfxViewParms* v2; // esi
	dvar_t* v3; // ebx
	dvarType_t v4; // edi
	const char* v5; // esi
	int v6; // esi
	GfxViewParms* v7; // eax
	const char* v8; // edi
	GfxViewParms* v9; // eax
	unsigned int v10; // ecx
	GfxViewParms* v11; // edi
	GfxViewParms* v12; // eax
	int v13; // edi
	GfxViewParms* v14; // ST18_4
	GfxViewParms* v15; // ST14_4
	GfxViewParms* v16; // ST10_4
	GfxViewParms* v17; // eax
	const char* v18; // eax
	GfxViewParms* v19; // ST18_4
	GfxViewParms* v20; // ST14_4
	GfxViewParms* v21; // ST10_4
	GfxViewParms* v22; // eax
	const char* v23; // eax
	int v24; // edi
	GfxViewParms* v25; // ST18_4
	GfxViewParms* v26; // ST14_4
	GfxViewParms* v27; // eax
	const char* v28; // eax
	GfxViewParms* v29; // ST18_4
	GfxViewParms* v30; // ST14_4
	GfxViewParms* v31; // eax
	const char* v32; // eax
	int v33; // edi
	int v34; // ebx
	GfxViewParms* v35; // ST18_4
	GfxViewParms* v36; // eax
	const char* v37; // eax
	GfxViewParms* v38; // ST18_4
	GfxViewParms* v39; // eax
	const char* v40; // eax
	const char* v41; // eax
	GfxViewParms* v42; // ST18_4
	GfxViewParms* v43; // eax
	GfxViewParms* v44; // ST18_4
	GfxViewParms* v45; // ST14_4
	GfxViewParms* v46; // eax
	GfxViewParms* v47; // ST18_4
	GfxViewParms* v48; // ST14_4
	GfxViewParms* v49; // ST10_4
	GfxViewParms* v50; // eax
	const char* string; // [esp+4h] [ebp-10h]
	dvarType_t dvar_type; // [esp+8h] [ebp-Ch]
	const dvar_t* dvar; // [esp+Ch] [ebp-8h]
	const char* dvarName; // [esp+10h] [ebp-4h]

	if (Cmd_Argc() < 2)
	{
#ifdef _DEBUG
		if (!Cmd_Argv(0)
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"(Cmd_Argv( 0 ))",
				(const char*)&scratch))
		{
			__debugbreak();
		}
#endif // _DEBUG
		v0 = Cmd_Argv(0);
		Com_Printf(0, "USAGE: %s <variable> <optional value sequence>\n", v0);
		return 0;
	}
	v2 = Cmd_Argv(1);
	dvarName = (const char*)v2;
#ifdef _DEBUG
	if (!v2
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dvarName)",
			(const char*)&scratch))
	{
		__debugbreak();
	}
#endif // _DEBUG
	v3 = Dvar_FindVar((const char*)v2);
	dvar = v3;
	if (!v3)
	{
		Com_Printf(0, "toggle failed: dvar '%s' not found.\n", v2);
		return 0;
	}
	if (Cmd_Argc() == 2)
		return Dvar_ToggleSimple(v3);
	v4 = Dvar_GetType(v3);
	dvar_type = v4;
	v5 = Dvar_DisplayableValue(v3);
	string = v5;
	switch (v4)
	{
	case 3:
		v33 = 2;
		if (Cmd_Argc() <= 4)
			goto LABEL_22;
		v34 = 4;
		while (1)
		{
			v35 = Cmd_Argv(v34 - 1);
			v36 = Cmd_Argv(v33);
			v37 = va("%s %s", v36, v35);
			if (!q_shared::I_stricmp(v5, v37))
				break;
			v33 += 2;
			v34 += 2;
			if (v34 >= Cmd_Argc())
			{
				v3 = (dvar_t*)dvar;
				goto LABEL_22;
			}
		}
		v38 = Cmd_Argv(v33 + 3);
		v39 = Cmd_Argv(v33 + 2);
		v40 = va("%s %s", v39, v38);
		Dvar_SetCommand(dvarName, v40);
		result = 1;
		break;
	case 4:
		v24 = 2;
		if (Cmd_Argc() <= 5)
			goto LABEL_22;
		while (1)
		{
			v25 = Cmd_Argv(v24 + 2);
			v26 = Cmd_Argv(v24 + 1);
			v27 = Cmd_Argv(v24);
			v28 = va("%s %s %s", v27, v26, v25);
			if (!q_shared::I_stricmp(v5, v28))
				break;
			v24 += 3;
			if (v24 + 3 >= Cmd_Argc())
				goto LABEL_22;
		}
		v29 = Cmd_Argv(v24 + 5);
		v30 = Cmd_Argv(v24 + 4);
		v31 = Cmd_Argv(v24 + 3);
		v32 = va("%s %s %s", v31, v30, v29);
		Dvar_SetCommand(dvarName, v32);
		result = 1;
		break;
	case 5:
		v13 = 2;
		if (Cmd_Argc() <= 6)
			goto LABEL_22;
		while (1)
		{
			v14 = Cmd_Argv(v13 + 3);
			v15 = Cmd_Argv(v13 + 2);
			v16 = Cmd_Argv(v13 + 1);
			v17 = Cmd_Argv(v13);
			v18 = va("%s %s %s %s", v17, v16, v15, v14);
			if (!q_shared::I_stricmp(v5, v18))
				break;
			v13 += 4;
			if (v13 + 4 >= Cmd_Argc())
				goto LABEL_22;
		}
		v19 = Cmd_Argv(v13 + 7);
		v20 = Cmd_Argv(v13 + 6);
		v21 = Cmd_Argv(v13 + 5);
		v22 = Cmd_Argv(v13 + 4);
		v23 = va("%s %s %s %s", v22, v21, v20, v19);
		Dvar_SetCommand(dvarName, v23);
		result = 1;
		break;
	default:
		v6 = 2;
		if (Cmd_Argc() > 3)
		{
			while (1)
			{
				v7 = Cmd_Argv(v6);
				v8 = (const char*)v7;
				if (dvar_type == 7)
				{
					v9 = Dvar_IndexStringToEnumString(v3, (const char*)v7);
					v10 = strlen((const char*)v9);
					v3 = (dvar_t*)dvar;
					if (v10)
						v8 = (const char*)v9;
				}
				++v6;
				if (!I_stricmp(string, v8))
					break;
				if (v6 + 1 >= Cmd_Argc())
					goto LABEL_22;
			}
			v12 = Cmd_Argv(v6);
			Dvar_SetCommand(dvarName, (const char*)v12);
			return 1;
		}
	LABEL_22:
		v11 = Cmd_Argv(2);
		switch (dvar_type)
		{
		case 3:
			v42 = Cmd_Argv(3);
			v43 = Cmd_Argv(2);
			v41 = va("%s %s", v43, v42);
			goto LABEL_45;
		case 4:
			v44 = Cmd_Argv(4);
			v45 = Cmd_Argv(3);
			v46 = Cmd_Argv(2);
			v41 = va("%s %s %s", v46, v45, v44);
			goto LABEL_45;
		case 5:
			v47 = Cmd_Argv(5);
			v48 = Cmd_Argv(4);
			v49 = Cmd_Argv(3);
			v50 = Cmd_Argv(2);
			v41 = va("%s %s %s %s", v50, v49, v48, v47);
			goto LABEL_45;
		case 6:
			break;
		case 7:
			v41 = (const char*)Dvar_IndexStringToEnumString(v3, (const char*)v11);
			if (strlen(v41))
				LABEL_45:
			v11 = (GfxViewParms*)v41;
			break;
		}
		Dvar_SetCommand(dvarName, (const char*)v11);
		return 1;
	}
	return result;
}

void dvar::Dvar_TogglePrint_f()
{
	int v0; // eax
	GfxViewParms* v1; // esi
	const dvar_t* v2; // edi
	const char* v3; // edi

	if (Dvar_ToggleInternal())
	{
		if (Cmd_Argc() < 2)
		{
			v0 = Cmd_Argc();
#ifdef _DEBUG
			if (!Assert_MyHandler(
				__FILE__,
				__LINE__,
				0,
				"((Cmd_Argc() >= 2))",
				"(Cmd_Argc()) = %i",
				v0))
				__debugbreak();
#endif //_DEBUG
		}
		v1 = Cmd_Argv(1);
#ifdef _DEBUG
		if (!v1
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				1,
				"(dvarName)",
				(const char*)&scratch))
		{
			__debugbreak();
		}
#endif // _DEBUG
		v2 = Dvar_FindVar((const char*)v1);
#ifdef _DEBUG
		if (!v2
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				1,
				"(dvar)",
				(const char*)&scratch))
		{
			__debugbreak();
		}
#endif // _DEBUG
		v3 = Dvar_DisplayableValue(v2);
#ifdef _DEBUG
		if (!v3
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				1,
				"(string)",
				(const char*)&scratch))
		{
			__debugbreak();
		}
#endif // _DEBUG
		Com_Printf((int)v2, 0, "%s toggled to %s\n", v2, v3);
	}
}

char dvar::Dvar_ToggleSimple(const dvar_t* dvar)
{
	bool v1; // al
	char result; // al
	int value; // ebx
	int v4; // edi
	long long v5; // kr00_8
	long long v6; // rax
	long long v7; // rax
	float v8; // xmm0_4
	const char* v9; // eax
	int v10; // edi
	int v11; // eax
	long long domain_value_max; // [esp+10h] [ebp-Ch]
	float domain_value_min; // [esp+18h] [ebp-4h]
#ifdef _DEBUG
	if (!dvar
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(dvar)", (const char*)&scratch))
	{
		__debugbreak();
	}
#endif // _DEBUG
	switch (Dvar_GetType(dvar))
	{
	case DVAR_TYPE_BOOL:
		v1 = Dvar_GetBool(dvar);
		Dvar_SetBoolFromSource(dvar, v1 == 0, DVAR_SOURCE_EXTERNAL);
		result = 1;
		break;
	case DVAR_TYPE_FLOAT:
		domain_value_min = Dvar_GetDomainFloatMin(dvar);
		*((float*)&domain_value_max + 1) = Dvar_GetDomainFloatMax(dvar);
		if (domain_value_min > 0.0 || *((float*)&domain_value_max + 1) < 1.0)
		{
			if (Dvar_GetFloat(dvar) == domain_value_min)
				v8 = *((float*)&domain_value_max + 1);
			else
				v8 = domain_value_min;
		}
		else if (Dvar_GetFloat(dvar) == 0.0)
		{
			v8 = 1.00000000;
		}
		else
		{
			v8 = 0.0;
		}
		Dvar_SetFloatFromSource(dvar, v8, DVAR_SOURCE_EXTERNAL);
		result = 1;
		break;
	case DVAR_TYPE_FLOAT_2:
	case DVAR_TYPE_FLOAT_3:
	case DVAR_TYPE_FLOAT_4:
	case DVAR_TYPE_STRING:
	case DVAR_TYPE_COLOR:
	case DVAR_TYPE_LINEAR_COLOR_RGB:
	case DVAR_TYPE_COLOR_XYZ:
		v9 = Dvar_GetName(dvar);
		Com_Printf((int)dvar, 0, "'toggle' with no arguments makes no sense for dvar '%s'\n", v9);
		result = 0;
		break;
	case DVAR_TYPE_INT:
		value = Dvar_GetDomainIntMax(dvar);
		v4 = Dvar_GetDomainIntMin(dvar);
		if (v4 > 0 || value < 1)
		{
			if (Dvar_GetInt(dvar) == v4)
				Dvar_SetIntFromSource(dvar, value, DVAR_SOURCE_EXTERNAL);
			else
				Dvar_SetIntFromSource(dvar, v4, DVAR_SOURCE_EXTERNAL);
			result = 1;
		}
		else
		{
			if (Dvar_GetInt(dvar))
				Dvar_SetIntFromSource(dvar, 0, DVAR_SOURCE_EXTERNAL);
			else
				Dvar_SetIntFromSource(dvar, 1, DVAR_SOURCE_EXTERNAL);
			result = 1;
		}
		break;
	case DVAR_TYPE_ENUM:
		v10 = Dvar_GetDomainEnumStringCount(dvar);
		if (v10)
		{
			v11 = Dvar_GetInt(dvar);
			Dvar_SetIntFromSource(dvar, (v11 + 1) % v10, DVAR_SOURCE_EXTERNAL);
		}
		result = 1;
		break;
	case DVAR_TYPE_INT64:
		v5 = Dvar_GetDomainInt64Min(dvar);
		v6 = Dvar_GetDomainInt64Max(dvar);
		domain_value_max = v6;
		if (SHIDWORD(v5) > 0 || v5 >= 0 && (unsigned int)v5 || v6 < 1)
		{
			if (__PAIR__(HIDWORD(v6), Dvar_GetInt64(dvar)) == v5)
				Dvar_SetInt64FromSource(dvar, domain_value_max, DVAR_SOURCE_EXTERNAL);
			else
				Dvar_SetInt64FromSource(dvar, v5, DVAR_SOURCE_EXTERNAL);
			result = 1;
		}
		else
		{
			LODWORD(v7) = Dvar_GetInt64(dvar);
			if (v7)
				Dvar_SetInt64FromSource(dvar, 0i64, DVAR_SOURCE_EXTERNAL);
			else
				Dvar_SetInt64FromSource(dvar, 1i64, DVAR_SOURCE_EXTERNAL);
			result = 1;
		}
		break;
	}
	return result;
}

bool dvar::Dvar_Toggle_f()
{
	return Dvar_ToggleInternal();
}

const char* dvar::Dvar_ValueToString(const dvar_t* dvar, DvarValue value)
{
	const char* result; // eax
	bool v3; // zf

	switch (dvar->type)
	{
	case 1:
		result = "1";
		if (!value.enabled)
			result = "0";
		return result;
	case 2:
		return va("%g", value.value);
	case 3:
		return va("%g %g", value.value, value.vector.v[1]);
	case 4:
	case 0xB:
	case 0xC:
		return va("%g %g %g", value.value, value.vector.v[1], value.vector.v[2]);
	case 5:
		return va("%g %g %g %g", value.value, value.vector.v[1], value.vector.v[2], value.vector.v[3]);
	case 6:
		return va("%i", value.integer);
	case 7:
		v3 = value.integer == 0;
		if (value.integer < 0)
			goto LABEL_15;
		if (value.integer >= dvar->domain.enumeration.stringCount)
		{
			v3 = value.integer == 0;
		LABEL_15:
			if (!v3
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
		if (dvar->domain.enumeration.stringCount)
			result = *(const char**)(dvar->domain.integer.max + 4 * value.integer);
		else
			result = (const char*)&scratch;
		break;
	case 8:
		if (!value.integer
			&& !Assert_MyHandler(
				__FILE__,
				__LINE__,
				value.integer,
				"((value.string))",
				"(dvar->name) = %s",
				dvar->name))
		{
			__debugbreak();
		}
		result = va("%s", value.integer);
		break;
	case 9:
		result = va(
			"%g %g %g %g",
			(float)((float)value.enabled * 0.0039215689),
			(float)((float)(unsigned char)value.color[1] * 0.0039215689),
			(float)((float)(unsigned char)value.color[2] * 0.0039215689),
			(float)((float)(unsigned char)value.color[3] * 0.0039215689));
		break;
	case 0xA:
		result = va("%lli", value.integer64);
		break;
	}
	return result;
}

void dvar::Dvar_VectorDomainToString(int components, char* outBuffer, int outBufferLen, DvarLimits domain)
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
			_snprintf(
				outBuffer,
				outBufferLen,
				"Domain is any %iD vector with components %g or bigger",
				components,
				LODWORD(v4),
				HIDWORD(v4));
		else
			_snprintf(
				outBuffer,
				outBufferLen,
				"Domain is any %iD vector with components from %g to %g",
				components,
				LODWORD(v4),
				HIDWORD(v4),
				domain.value.max);
	}
}

void dvar::Dvar_WeakCopyString(const char* string, DvarValue* value)
{
	if (!string
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(string)", (const char*)&scratch))
	{
		__debugbreak();
	}
	value->integer = (int)string;
}