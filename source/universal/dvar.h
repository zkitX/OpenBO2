#pragma once

#include <gfx_d3d/r_utils.h>

#include <qcommon/com_clients.h>
#include <universal/memfile.h>

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

struct FastCriticalSection
{
	volatile int readCount;
	volatile int writeCount;
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

struct dvarCallBack_t
{
	bool needsCallback;
	void(* callback)(const dvar_t*);
	const dvar_t* dvar;
};

struct enumDvarDef_s
{
	const char* enumDvarName;
};

dvarCallBack_t s_dvarCallbackPool[64];

bool s_isLoadingAutoExecGlobalFlag;
bool s_areDvarsSorted;
bool s_canSetConfigDvars;
bool s_isDvarSystemActive;
int s_nextFreeCallback;

FastCriticalSection g_dvarCritSect;

dvar_t* s_dvarHashTable[1080];

const dvar_t* ClickToContinue;
const dvar_t* cl_socketpool_enabled;
const dvar_t* cl_socketpool_size;
const dvar_t* loc_language;
const dvar_t* net_broadcast;
const dvar_t* useFastFile;

void Dvar_SetInAutoExec(bool);
bool Dvar_IsSystemActive(void);
bool Dvar_IsValidName(const char*);
void Dvar_CopyString(const char*, DvarValue*);
void Dvar_WeakCopyString(const char*, DvarValue*);
bool Dvar_ShouldFreeCurrentString(dvar_t*);
bool Dvar_ShouldFreeLatchedString(dvar_t*);
bool Dvar_ShouldFreeResetString(dvar_t*);
void Dvar_FreeString(DvarValue*);
void Dvar_AssignCurrentStringValue(dvar_t*, DvarValue*, const char*);
void Dvar_AssignLatchedStringValue(dvar_t*, DvarValue*, const char*);
void Dvar_AssignResetStringValue(dvar_t*, DvarValue*, const char*);
char* Dvar_EnumToString(dvar_t const*);
char* Dvar_IndexStringToEnumString(dvar_t const*, const char*);
char* Dvar_ValueToString(dvar_t const*, DvarValue);
bool Dvar_StringToBool(const char*);
int Dvar_StringToInt(const char*);
__int64 Dvar_StringToInt64(const char*);
float Dvar_StringToFloat(const char*);
void Dvar_StringToVec2(const char*, vec2_t*);
void Dvar_StringToVec3(const char*, vec3_t*);
void Dvar_StringToVec4(const char*, vec4_t*);
int Dvar_StringToEnum(DvarLimits const*, const char*);
void Dvar_StringToColor(char*, const char*);
DvarValue* Dvar_StringToValue(dvarType_t, const char*, DvarLimits);
char* Dvar_DisplayableValue(dvar_t const*);
char* Dvar_DisplayableResetValue(dvar_t const*);
char* Dvar_DisplayableLatchedValue(dvar_t const*);
DvarValue Dvar_ClampValueToDomain(dvarType_t, DvarValue, DvarValue, DvarLimits);
bool Dvar_ValueInDomain(dvarType_t, DvarValue, DvarLimits);
void Dvar_VectorDomainToString(int, char*, int, DvarLimits);
char* Dvar_DomainToString_Internal(char*, int, dvarType_t, DvarLimits, int*);
char* Dvar_DomainToString_GetLines(dvarType_t, DvarLimits, char*, int, int*);
void Dvar_PrintDomain(dvarType_t, DvarLimits);
bool Dvar_ValuesEqual(DvarValue, DvarValue);
void Dvar_SetLatchedValue(dvar_t*, DvarValue);
bool Dvar_HasLatchedValue(dvar_t const*);
dvarCallBack_t* findCallBackForDvar(dvar_t const*);
dvar_t* Dvar_FindMalleableVar(int);
dvar_t* Dvar_FindMalleableVar(const char*);
dvar_t* Dvar_FindVar(const char*);
dvar_t* Dvar_FindVar(int);
void Dvar_ClearModified(dvar_t*);
void Dvar_SetModified(dvar_t*);
bool Dvar_GetModified(const dvar_t*);
int Dvar_GetInt(int);
unsigned int Dvar_GetUnsignedInt(dvar_t const*);
float Dvar_GetFloat(long);
void Dvar_GetVec2(const dvar_t*, vec2_t*);
void Dvar_GetVec3(const dvar_t*, vec3_t*);
void Dvar_GetVec4(const dvar_t*, vec4_t*);
char* Dvar_GetString(const dvar_t*);
char* Dvar_GetVariantString(int);
char* Dvar_GetVariantString(const dvar_t*);
void Dvar_GetUnpackedColor(dvar_t const*, vec4_t&);
void Dvar_GetColor(const dvar_t*, char*);
float Dvar_GetColorRed(dvar_t const*);
float Dvar_GetColorRed(int);
float Dvar_GetColorGreen(dvar_t const*);
float Dvar_GetColorGreen(int);
float Dvar_GetColorBlue(dvar_t const*);
float Dvar_GetColorBlue(int);
float Dvar_GetColorAlpha(dvar_t const*);
float Dvar_GetColorAlpha(int);
bool Dvar_GetLatchedBool(dvar_t const*);
int Dvar_GetLatchedInt(dvar_t const*);
float Dvar_GetLatchedFloat(dvar_t const*);
void Dvar_GetLatchedVec2(dvar_t const*, vec2_t*);
void Dvar_GetLatchedVec3(dvar_t const*, vec3_t*);
void Dvar_GetLatchedVec4(dvar_t const*, vec4_t*);
void Dvar_GetLatchedColor(dvar_t const*, char*);
int Dvar_GetResetInt(dvar_t const*);
const char* Dvar_GetResetString(dvar_t const*);
void Dvar_GetResetVec3(dvar_t const*, vec3_t&);
char** Dvar_GetDomainEnumStrings(dvar_t const*);
int Dvar_GetDomainEnumStringCount(dvar_t const*);
int Dvar_GetDomainIntMin(dvar_t const*);
int Dvar_GetDomainIntMax(dvar_t const*);
__int64 Dvar_GetDomainInt64Min(dvar_t const*);
__int64 Dvar_GetDomainInt64Max(dvar_t const*);
float Dvar_GetDomainFloatMin(dvar_t const*);
float Dvar_GetDomainFloatMax(dvar_t const*);
float Dvar_GetDomainVecMin(dvar_t const*);
float Dvar_GetDomainVecMax(dvar_t const*);
dvarType_t Dvar_GetType(dvar_t const*);
DvarValue Dvar_GetCurrent(dvar_t const*);
DvarLimits Dvar_GetDomain(dvar_t const*);
char* Dvar_GetDescription(dvar_t const*);
unsigned int Dvar_GetFlags(dvar_t const*);
char* Dvar_GetName(dvar_t const*);
void Dvar_Shutdown(void);
void Dvar_PerformUnregistration(dvar_t*);
void Dvar_UpdateResetValue(dvar_t*, DvarValue);
void Dvar_ChangeResetValue(dvar_t const*, DvarValue);
void Dvar_UpdateValue(dvar_t*, DvarValue);
void Dvar_MakeExplicitType(dvar_t*, const char*, dvarType_t, unsigned int, DvarValue, DvarLimits);
void Dvar_ReinterpretDvar(dvar_t*, const char*, dvarType_t, unsigned int, DvarValue, DvarLimits);
dvar_t* Dvar_RegisterNew(const char*, dvarType_t, unsigned int, DvarValue, DvarLimits, const char*);
void Dvar_AddFlags(dvar_t const*, int);
int Com_SaveDvarsToBuffer(const char** const, unsigned int, char*, unsigned int);
void Dvar_SetModifiedCallback(dvar_t const*, void (*)(dvar_t const*));
void Dvar_SetCanSetConfigDvars(bool);
bool Dvar_CanSetConfigDvar(dvar_t const*);
bool Dvar_CanChangeValue(dvar_t const*, DvarValue, DvarSetSource);
void Dvar_SetVariant(dvar_t*, DvarValue, DvarSetSource);
void Dvar_UpdateEnumDomain(dvar_t const*, const char** const);
bool Dvar_GetBool(dvar_t const*);
int Dvar_GetInt(dvar_t const*);
__int64 Dvar_GetInt64(dvar_t const*);
float Dvar_GetFloat(dvar_t const*);
void Dvar_MakeLatchedValueCurrent(dvar_t const*);
void Dvar_Reregister(dvar_t*, const char*, dvarType_t, unsigned int, DvarValue, DvarLimits, const char*);
dvar_t* Dvar_RegisterVariant(const char*, dvarType_t, unsigned int, DvarValue, DvarLimits, const char*);
dvar_t* _Dvar_RegisterBool(const char*, bool, unsigned int, const char*);
dvar_t* _Dvar_RegisterInt(const char*, int, int, int, unsigned int, const char*);
dvar_t* _Dvar_RegisterInt64(const char*, __int64, __int64, __int64, unsigned int, const char*);
dvar_t* _Dvar_RegisterFloat(const char*, float, float, float, unsigned int, const char*);
dvar_t* _Dvar_RegisterVec2(const char*, float, float, float, float, unsigned int, const char*);
dvar_t* _Dvar_RegisterVec3(const char*, float, float, float, float, float, unsigned int, const char*);
dvar_t* _Dvar_RegisterVec4(const char*, float, float, float, float, float, float, unsigned int, const char*);
dvar_t* _Dvar_RegisterString(const char*, const char*, unsigned int, const char*);
dvar_t* _Dvar_RegisterEnum(const char*, const char** const, int, unsigned int, const char*);
dvar_t* _Dvar_RegisterColor(const char*, float, float, float, float, unsigned int, const char*);
dvar_t* _Dvar_RegisterLinearRGB(const char*, float, float, float, float, float, unsigned int, const char*);
dvar_t* _Dvar_RegisterColorXYZ(const char*, float, float, float, float, float, unsigned int, const char*);
void Dvar_SetBoolFromSource(dvar_t const*, bool, DvarSetSource);
void Dvar_SetIntFromSource(dvar_t const*, int, DvarSetSource);
void Dvar_SetInt64FromSource(dvar_t const*, __int64, DvarSetSource);
void Dvar_SetFloatFromSource(dvar_t const*, float, DvarSetSource);
void Dvar_SetVec2FromSource(dvar_t const*, float, float, DvarSetSource);
void Dvar_SetVec3FromSource(dvar_t const*, float, float, float, DvarSetSource);
void Dvar_SetVec4FromSource(dvar_t const*, float, float, float, float, DvarSetSource);
void Dvar_SetStringFromSource(dvar_t const*, const char*, DvarSetSource);
void Dvar_SetColorFromSource(dvar_t const*, float, float, float, float, DvarSetSource);
void Dvar_SetBool(dvar_t const*, bool);
void Dvar_SetInt(dvar_t const*, int);
void Dvar_SetFloat(dvar_t const*, float);
void Dvar_SetVec2(dvar_t const*, float, float);
void Dvar_SetVec3(dvar_t const*, float, float, float);
void Dvar_SetVec4(dvar_t const*, float, float, float, float);
void Dvar_SetVec4FromVec4(dvar_t const*, vec4_t&);
void Dvar_SetString(dvar_t const*, const char*);
dvar_t* Dvar_SetOrRegisterString(dvar_t const*, const char*, const char*);
void Dvar_SetColor(dvar_t const*, float, float, float, float);
void Dvar_SetFromStringFromSource(dvar_t const*, const char*, DvarSetSource);
void Dvar_SetFromString(dvar_t const*, const char*);
dvar_t* Dvar_SetFromStringByNameFromSource(const char*, const char*, DvarSetSource, unsigned int);
void Dvar_SetFromStringByName(const char*, const char*);
void Dvar_SetCommand(const char*, const char*);
void Dvar_Reset(dvar_t const*, DvarSetSource);
void Dvar_SetCheatState(void);
void Dvar_Init(void);
void Dvar_LoadDvarsAddFlags(MemoryFile*, unsigned short);
void Dvar_LoadDvars(MemoryFile*);
void Dvar_LoadScriptInfo(MemoryFile*);
void Dvar_ResetDvars(unsigned int, DvarSetSource);
int Com_LoadDvarsFromBufferOptional(const char** const, bool*, unsigned int, const char*, const char*);
void Dvar_SetBoolIfChanged(dvar_t const*, bool);
void Dvar_SetIntIfChanged(dvar_t const*, int);
void Dvar_SetFloatIfChanged(dvar_t const*, float);
void Dvar_SetStringIfChanged(dvar_t const*, const char*);
void Dvar_DoModifiedCallbacks(void);
int Com_LoadDvarsFromBuffer(const char** const, unsigned int, const char*, const char*);
void Dvar_Sort(void);
void Dvar_ForEach(void (*)(dvar_t const*, void*), void*);
void Dvar_ForEachName(void (*)(const char*));
void Dvar_ForEachName(LocalClientNum_t, void (*)(LocalClientNum_t, const char*));
char* Dvar_GetResetString(struct dvar_t const*);