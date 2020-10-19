#pragma once

struct _iobuf
{
    char* _ptr;
    int _cnt;
    char* _base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char* _tmpfname;
};

struct cspField_t
{
    const char* szName;
    int iOffset;
    int iFieldType;
};

int LongNoSwap(int);
int FloatWriteNoSwap(float);
unsigned char ColorIndex(unsigned char);
char const* Com_GetFilenameSubStr(char const*);
char const* Com_GetExtensionSubString(char const*);
void Com_StripExtension(char const*, char*);
short BigShort(short);
int BigLong(int);
unsigned __int64 BigLong64(unsigned __int64);
short LittleShort(short);
int LittleFloatWrite(float);
short ShortSwap(short);
short ShortNoSwap(short);
int LongSwap(int);
unsigned __int64 Long64Swap(unsigned __int64);
unsigned __int64 Long64NoSwap(unsigned __int64);
float FloatReadNoSwap(int);
void Swap_Init(void);
unsigned int I_atoui(char const*);
__int64 I_atoi64(char const*);
bool I_islower(int);
bool I_isupper(int);
bool I_isdigit(int);
bool I_isalnum(int);
bool I_isforfilename(int);
void I_strncpyz(char* dest, char const* src, int destsize);
int I_strnicmp(char const* s0, char const* s1, int n);
int I_strncmp(char const* s0, char const* s1, int n);
char const* I_stristr(char const* s0, char const* substr);
int I_stricmp(char const* s0, char const s1);
int I_strcmp(char const*, char const*);
int I_stricmpwild(char const*, char const*);
char* I_strlwr(char*);
char* I_strupr(char*);
int I_iscsym(int);
void I_strncat(char*, int, char const*);
int I_DrawStrlen(char const*);
bool I_IsSpecialToken(char const*);
int I_IsSpecialTokenRecursive(char const*);
int I_IsSpecialTokenString(char const*);
char* I_CleanChat(char*);
char* I_CleanStr(char*);
char I_CleanChar(char);
int Com_sprintf(char* dest, int size, const char* fmt, ...);
int Com_sprintfPos(char*, int, int*, char const*, ...);
int CanKeepStringPointer(char const*);
char* I_itoa(int, char*, int);
char* va(char const* format, ...);
void Com_InitThreadData(int);
char const* Info_ValueForKey(char const*, char const*);
void Info_NextPair(char const**, char*, char*);
void Info_RemoveKey(char*, char const*);
void Info_RemoveKey_Big(char*, char const*);
int Info_Validate(char const*);
void Info_SetValueForKey(char*, char const*, char const*);
void Info_SetValueForKey_Big(char*, char const*, char const*);
int KeyValueToField(unsigned char*, cspField_t const*, char const*, int, int (*)(unsigned char*, char const*, int, int), void (*)(unsigned char*, char const*));
int ParseConfigStringToStruct(unsigned char*, cspField_t const*, int, char const*, int, int (*)(unsigned char*, char const*, int, int), void (*)(unsigned char*, char const*));
int ParseConfigStringToStructMerged(unsigned char*, struct cspField_t const*, int, char const*, char const** const, char const** const, char*, int, int (*)(unsigned char*, char const*, int, int), void (*)(unsigned char*, char const*), int (*)(char const*, char** const, char*, int));
float GetLeanFraction(float);
float UnGetLeanFraction(float);
void AddLeanToPosition(union vec3_t&, float, float, float, float);
void OrientationConcatenate(struct orientation_t const*, struct orientation_t const*, struct orientation_t*);
void OrientationInvert(struct orientation_t const*, struct orientation_t*);
int Com_IsLegacyXModelName(char const*);
void Com_DefaultExtension(char*, int, char const*);
float CMD_GetAnalogButtonValue(struct usercmd_s*, int);
