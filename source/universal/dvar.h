#pragma once

#include "com_memory.h"
#include "assertive.h"

#include "../cgame/cg_main.h"
#include "../gfx_d3d/r_utils.h"
#include "../defs.h"

enum dvarType_t
{
	DVAR_TYPE_INVALID = 0x0,
	DVAR_TYPE_BOOL = 0x1,
	DVAR_TYPE_FLOAT = 0x2,
	DVAR_TYPE_FLOAT_2 = 0x3,
	DVAR_TYPE_FLOAT_3 = 0x4,
	DVAR_TYPE_FLOAT_4 = 0x5,
	DVAR_TYPE_INT = 0x6,
	DVAR_TYPE_ENUM = 0x7,
	DVAR_TYPE_STRING = 0x8,
	DVAR_TYPE_COLOR = 0x9,
	DVAR_TYPE_INT64 = 0xA,
	DVAR_TYPE_LINEAR_COLOR_RGB = 0xB,
	DVAR_TYPE_COLOR_XYZ = 0xC,
	DVAR_TYPE_COUNT = 0xD,
};

enum DvarSetSource
{
	DVAR_SOURCE_INTERNAL = 0x0,
	DVAR_SOURCE_EXTERNAL = 0x1,
	DVAR_SOURCE_SCRIPT = 0x2,
	DVAR_SOURCE_DEVGUI = 0x3,
};

union DvarValue
{
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	long long integer64;
	unsigned long long unsignedInt64;
	float value;
	vec4_t vector;
	const char* string;
	char color[4];
};

union DvarLimits
{
	struct {
		int stringCount;
		const char** strings;
	} enumeration;
	struct {
		int min;
		int max;
	} integer;
	struct {
		long long min;
		long long max;
	} integer64;
	struct {
		float min;
		float max;
	} value;
	struct {
		float min;
		float max;
	} vector;
};

struct dvar_t
{
	const char* name;
	const char* description;
	int hash;
	unsigned int flags;
	dvarType_t type;
	bool modified;
	DvarValue current;
	DvarValue latched;
	DvarValue reset;
	DvarLimits domain;
	dvar_t* hashNext;
};

struct enumDvarDef_s
{
	const char* enumDvarName;
};

void Dvar_AddCommands();
void Dvar_AddConfigFlag_f();
void Dvar_AddFlags(const dvar_t* dvar, int flags);
char Dvar_CanSetConfigDvar(const dvar_t* dvar);
void Dvar_ChangeResetValue(const dvar_t* dvar, DvarValue value);
void Dvar_ClearModified(const dvar_t* dvar);
dvar_t* Dvar_Command();
const char* Dvar_DisplayableLatchedValue(const dvar_t* dvar);
const char* Dvar_DisplayableResetValue(const dvar_t* dvar);
const char* Dvar_DisplayableValue(const dvar_t* dvar);
void Dvar_DoModifiedCallbacks();
const char* Dvar_DomainToString_GetLines(dvarType_t type, DvarLimits domain, char* outBuffer, int outBufferLen, int* outLineCount);
void Dvar_Dump_f();
GfxViewParms* Dvar_EnumToString(const dvar_t* dvar);
dvar_t* Dvar_FindVar(int dvarHash);
dvar_t* Dvar_FindVar(const char* dvarName);
void Dvar_ForEach(void(__cdecl* callback)(const dvar_t*, void*), void* userData);
void Dvar_ForEachName(LocalClientNum_t localClientNum, void(__cdecl* callback)(LocalClientNum_t, const char*));
void Dvar_ForEachName(void(__cdecl* callback)(const char*));
bool Dvar_GetBool(const dvar_t* dvar);
void Dvar_GetColor(const dvar_t* dvar, char* color);

double Dvar_GetColorAlpha(const dvar_t* dvar);
double Dvar_GetColorAlpha(int dvarHash);

double Dvar_GetColorBlue(const dvar_t* dvar);
double Dvar_GetColorBlue(int dvarHash);

double Dvar_GetColorGreen(const dvar_t* dvar);
double Dvar_GetColorGreen(int dvarHash);

double Dvar_GetColorRed(const dvar_t* dvar);
double Dvar_GetColorRed(int dvarHash);

DvarValue* Dvar_GetCurrent(DvarValue* result, const dvar_t* dvar);
const char* Dvar_GetDescription(const dvar_t* dvar);
DvarLimits* Dvar_GetDomain(DvarLimits* result, const dvar_t* dvar);

int Dvar_GetDomainEnumStringCount(const dvar_t* dvar);
const char** Dvar_GetDomainEnumStrings(const char** result);

double Dvar_GetDomainFloatMax(const dvar_t* dvar);
double Dvar_GetDomainFloatMin(const dvar_t* dvar);

long long Dvar_GetDomainInt64Max(const dvar_t* dvar);
long long Dvar_GetDomainInt64Min(const dvar_t* dvar);

int Dvar_GetDomainIntMax(const dvar_t* dvar);
int Dvar_GetDomainIntMin(const dvar_t* dvar);

double Dvar_GetDomainVecMax(const dvar_t* dvar);
double Dvar_GetDomainVecMin(const dvar_t* dvar);

const dvar_t* Dvar_GetFlags(const dvar_t* dvar);

double Dvar_GetFloat(const dvar_t* dvar);
double Dvar_GetFloat(int dvarHash);

int Dvar_GetInt(const dvar_t* dvar);
int Dvar_GetInt(int dvarHash);

long long Dvar_GetInt64(const dvar_t* dvar);
long long Dvar_GetInt64(int dvarHash);

bool Dvar_GetLatchedBool(const dvar_t* dvar);
void Dvar_GetLatchedColor(const dvar_t* dvar, char* color);
double Dvar_GetLatchedFloat(const dvar_t* dvar);
int Dvar_GetLatchedInt(const dvar_t* dvar);
void Dvar_GetLatchedVec2(const dvar_t* dvar, vec2_t* result);
void Dvar_GetLatchedVec3(const dvar_t* dvar, vec3_t* result);
void Dvar_GetLatchedVec4(const dvar_t* dvar, vec4_t* result);

bool Dvar_GetModified(const dvar_t* dvar);
const char* Dvar_GetName(const dvar_t* dvar);
int Dvar_GetResetInt(const dvar_t* dvar);
const char* Dvar_GetResetString(const dvar_t* dvar);
void Dvar_GetResetVec3(const dvar_t* dvar, vec3_t* result);
const char* Dvar_GetString(const dvar_t* dvar);
dvarType_t Dvar_GetType(const dvar_t* dvar);
void Dvar_GetUnpackedColor(const dvar_t* dvar, vec4_t* expandedColor);
const dvar_t* Dvar_GetUnsignedInt(const dvar_t* dvar);

const char* Dvar_GetVariantString(const dvar_t* dvar);
const char* Dvar_GetVariantString(int dvarHash);

void Dvar_GetVec2(const dvar_t* dvar, vec2_t* result);
void Dvar_GetVec3(const dvar_t* dvar, vec3_t* result);
void Dvar_GetVec4(const dvar_t* dvar, vec4_t* result);

bool Dvar_HasLatchedValue(const dvar_t* dvar);
GfxViewParms* Dvar_IndexStringToEnumString(const dvar_t* dvar, const char* indexString);
char* Dvar_InfoString(LocalClientNum_t localClientNum, int bit);
void Dvar_InfoStringSingle(const dvar_t* dvar, void* userData);
void Dvar_InfoStringSingle_Big(const dvar_t* dvar, void* userData);
char* Dvar_InfoString_Big(int bit);

bool Dvar_IsSystemActive();
char Dvar_IsValidName(const char* dvarName);
void Dvar_ListSingle(const dvar_t* dvar, void* userData);
void Dvar_MakeLatchedValueCurrent(const dvar_t* dvar);
void Dvar_RegisterColor_f();
void Dvar_Reset(const dvar_t* dvar, DvarSetSource setSource);
void Dvar_ResetDvars(unsigned int filter, DvarSetSource setSource);
void Dvar_SetBool(const dvar_t* dvar, bool value);
void Dvar_SetBoolFromSource(const dvar_t* dvar, bool value, DvarSetSource source);
void Dvar_SetBoolIfChanged(const dvar_t* dvar, bool value);
void Dvar_SetCanSetConfigDvars(bool canSetConfigDvars);
void Dvar_SetCheatState();
void Dvar_SetColor(const dvar_t* dvar, float r, float g, float b, float a);
void Dvar_SetColorFromSource(const dvar_t* dvar, float r, float g, float b, float a, DvarSetSource source);
void Dvar_SetCommand(const char* dvarName, const char* string);
void Dvar_SetFloat(const dvar_t* dvar, float value);
void Dvar_SetFloatFromSource(const dvar_t* dvar, float value, DvarSetSource source);
void Dvar_SetFloatIfChanged(const dvar_t* dvar, float value);
void Dvar_SetFromString(const dvar_t* dvar, const char* string);
void Dvar_SetFromStringByName(const char* dvarName, const char* string);
const dvar_t* Dvar_SetFromStringByNameFromSource(const char* dvarName, const char* string, DvarSetSource source, unsigned int flags);
void Dvar_SetFromStringFromSource(const dvar_t* dvar, const char* string, DvarSetSource source);
void Dvar_SetInAutoExec(bool inAutoExec);
void Dvar_SetInt(const dvar_t* dvar, int value);
void Dvar_SetInt64FromSource(const dvar_t* dvar, long long value, DvarSetSource source);
void Dvar_SetIntFromSource(const dvar_t* dvar, int value, DvarSetSource source);
void Dvar_SetIntIfChanged(const dvar_t* dvar, int value);
void Dvar_SetLatchedValue(DvarValue value);
void Dvar_SetModified(const dvar_t* dvar);
void Dvar_SetModifiedCallback(const dvar_t* dvar, void(__cdecl* callback)(const dvar_t*));
const dvar_t* Dvar_SetOrRegisterString(const dvar_t* dvar, const char* dvarName, const char* value);
void Dvar_SetString(const dvar_t* dvar, const char* value);
void Dvar_SetStringIfChanged(const dvar_t* dvar, const char* newString);
void Dvar_SetVec2(const dvar_t* dvar, float x, float y);
void Dvar_SetVec2FromSource(const dvar_t* dvar, float x, float y, DvarSetSource source);
void Dvar_SetVec3(const dvar_t* dvar, float x, float y, float z);
void Dvar_SetVec3FromSource(const dvar_t* dvar, float x, float y, float z, DvarSetSource source);
void Dvar_SetVec4(const dvar_t* dvar, float x, float y, float z, float w);
void Dvar_SetVec4FromSource(const dvar_t* dvar, float x, float y, float z, float w, DvarSetSource source);
void Dvar_SetVec4FromVec4(const dvar_t* dvar, vec4_t* vecin);
bool Dvar_ShouldFreeCurrentString(dvar_t* dvar);
bool Dvar_ShouldFreeLatchedString(dvar_t* dvar);
bool Dvar_ShouldFreeResetString(dvar_t* dvar);
void Dvar_Shutdown();
void Dvar_Sort();
void Dvar_UpdateEnumDomain(const dvar_t* dvar, const char** stringTable);
void Dvar_UpdateResetValue(DvarValue value);
void Dvar_UpdateValue(dvar_t* dvar, DvarValue value);
bool Dvar_ValueInDomain(dvarType_t type, DvarValue value, DvarLimits domain);
int Dvar_ValuesEqual(DvarValue val0, DvarValue val1);
void Dvar_WriteDefaults(int f);
void Dvar_WriteSingleDefault(const dvar_t* dvar, void* userData);
void Dvar_WriteSingleVariable(const dvar_t* dvar, void* userData);
void Dvar_WriteVariables(int f);

dvar_t* _Dvar_RegisterBool(const char* dvarName, bool value, unsigned int flags, const char* description);
dvar_t* _Dvar_RegisterFloat(const char* dvarName, float value, float min, float max, unsigned int flags, const char* description);
dvar_t* Dvar_RegisterVariant(const char* dvarName, dvarType_t type, unsigned int flags, DvarValue value, DvarLimits domain, const char* description);

class dvar
{
public:
	static void Dvar_AssignCurrentStringValue(dvar_t* dvar, DvarValue* dest, const char* string);
	static void Dvar_AssignLatchedStringValue(dvar_t* dvar, DvarValue* dest, const char* string);
	static void Dvar_AssignResetStringValue(dvar_t* dvar, DvarValue* dest, const char* string);
	static char Dvar_CanChangeValue(const dvar_t* dvar, DvarSetSource source, DvarValue value);
	static DvarValue* Dvar_ClampValueToDomain(dvarType_t type, int a2, DvarValue value, const DvarValue resetValue, const DvarLimits domain);
	static void Dvar_CopyString(const char* string, DvarValue* value);
	static const char* Dvar_DomainToString_Internal(char* outBuffer, int outBufferLen, dvarType_t type, DvarLimits domain, int* outLineCount);
	static dvar_t* Dvar_FindMalleableVar(int dvarHash);
	static dvar_t* Dvar_FindMalleableVar(const char* dvarName);
	static void Dvar_FreeString(DvarValue* value);
	static void Dvar_Init(int a1);
	static void Dvar_List_f();
	static void Dvar_LoadDvars(MemoryFile* memFile);
	static void Dvar_LoadDvarsAddFlags(MemoryFile* memFile, unsigned short flags);
	static void Dvar_LoadScriptInfo(MemoryFile* memFile);
	static void Dvar_MakeExplicitType(dvar_t* dvar, dvarType_t type, const char* dvarName, unsigned int flags, DvarValue resetValue, DvarLimits domain);
	static void Dvar_PerformUnregistration(dvar_t* dvar);
	static void Dvar_PrintDomain(dvarType_t type, DvarLimits domain);
	static void Dvar_RegisterBool_f();
	static void Dvar_RegisterFloat_f();
	static void Dvar_RegisterInt_f();
	static dvar_t* Dvar_RegisterNew(const char* dvarName, dvarType_t type, unsigned int flags, DvarValue value, DvarLimits domain, const char* description);
	static dvar_t* Dvar_RegisterVariant(const char* dvarName, dvarType_t type, unsigned int flags, DvarValue value, DvarLimits domain, const char* description);

	static void Dvar_ReinterpretDvar(dvar_t* dvar, unsigned int flags, const char* dvarName, dvarType_t type, DvarValue value, DvarLimits domain);
	static void Dvar_Reregister(dvar_t* dvar, const char* dvarName, dvarType_t type, unsigned int flags, DvarValue resetValue, DvarLimits domain, const char* description);
	static void Dvar_Reset_f();
	static void Dvar_SetA_f();
	static void Dvar_SetStringFromSource(const dvar_t* dvar, const char* string, DvarSetSource source);
	static void Dvar_SetToTime_f();
	static void Dvar_SetVariant(dvar_t* dvar, DvarValue value, DvarSetSource source);
	static void Dvar_Set_f();
	static bool Dvar_StringToBool(const char* string);
	static void Dvar_StringToColor(char* color, const char* string);
	static int Dvar_StringToEnum(const DvarLimits* domain, const char* string);
	static double Dvar_StringToFloat(const char* string);
	static int Dvar_StringToInt(const char* string);
	static long long Dvar_StringToInt64(const char* string);
	static DvarValue* Dvar_StringToValue(const dvarType_t type, const char* string, double a3, const DvarLimits domain);
	static void Dvar_StringToVec2(const char* string, vec2_t* vector);
	static void Dvar_StringToVec3(const char* string, vec3_t* vector);
	static void Dvar_StringToVec4(const char* string, vec4_t* vector);
	static bool Dvar_ToggleInternal();
	static void Dvar_TogglePrint_f();
	static char Dvar_ToggleSimple(const dvar_t* dvar);
	static bool Dvar_Toggle_f();
	static const char* Dvar_ValueToString(const dvar_t* dvar, DvarValue value);
	static void Dvar_VectorDomainToString(int components, char* outBuffer, int outBufferLen, DvarLimits domain);
	static void Dvar_WeakCopyString(const char* string, DvarValue* value);
};