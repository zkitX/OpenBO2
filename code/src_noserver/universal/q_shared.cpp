#include "q_shared.h"

int LongNoSwap(int)
{
	return 0;
}

int FloatWriteNoSwap(float)
{
	return 0;
}

unsigned char ColorIndex(unsigned char c)
{
	int result; // eax

	result = (unsigned __int8)(c - 48);
	if ((unsigned __int8)(c - 48) >= 0xAu)
		result = 7;
	return result;
}

char const* Com_GetFilenameSubStr(char const* pathname)
{
	const char* namePointer; // edx
	char name; // cl
	const char* result; // eax

	namePointer = pathname;
	name = *pathname;
	for (result = pathname; name; ++namePointer)
	{
		if (name == '/' || name == '\\')
			result = namePointer + 1;
		name = namePointer[1];
	}
	return result;
}

char const* Com_GetExtensionSubString(char const*)
{
	return nullptr;
}

void Com_StripExtension(char const*, char*)
{
}

short BigShort(short)
{
	return 0;
}

int BigLong(int)
{
	return 0;
}

unsigned __int64 BigLong64(unsigned __int64)
{
	return 0;
}

short LittleShort(short)
{
	return 0;
}

int LittleFloatWrite(float)
{
	return 0;
}

short ShortSwap(short)
{
	return 0;
}

short ShortNoSwap(short)
{
	return 0;
}

int LongSwap(int)
{
	return 0;
}

unsigned __int64 Long64Swap(unsigned __int64)
{
	return 0;
}

unsigned __int64 Long64NoSwap(unsigned __int64)
{
	return 0;
}

float FloatReadNoSwap(int)
{
	return 0.0f;
}

void Swap_Init(void)
{
}

unsigned int I_atoui(char const*)
{
	return 0;
}

__int64 I_atoi64(char const*)
{
	return 0;
}

bool I_islower(int)
{
	return false;
}

bool I_isupper(int)
{
	return false;
}

bool I_isdigit(int)
{
	return false;
}

bool I_isalnum(int)
{
	return false;
}

bool I_isforfilename(int)
{
	return false;
}

void I_strncpyz(char* dest, char const* src, int destsize)
{
}

int I_strnicmp(char const* s0, char const* s1, int n)
{
	return 0;
}

int I_strncmp(char const* s0, char const* s1, int n)
{
	return 0;
}

char const* I_stristr(char const* s0, char const* substr)
{
	return nullptr;
}

int I_stricmp(char const* s0, char const s1)
{
	return 0;
}

int I_strcmp(char const*, char const*)
{
	return 0;
}

int I_stricmpwild(char const*, char const*)
{
	return 0;
}

char* I_strlwr(char*)
{
	return nullptr;
}

char* I_strupr(char*)
{
	return nullptr;
}

int I_iscsym(int)
{
	return 0;
}

void I_strncat(char*, int, char const*)
{
}

int I_DrawStrlen(char const*)
{
	return 0;
}

bool I_IsSpecialToken(char const*)
{
	return false;
}

int I_IsSpecialTokenRecursive(char const*)
{
	return 0;
}

int I_IsSpecialTokenString(char const*)
{
	return 0;
}

char* I_CleanChat(char*)
{
	return nullptr;
}

char* I_CleanStr(char*)
{
	return nullptr;
}

char I_CleanChar(char)
{
	return 0;
}

int Com_sprintf(char* dest, int size, const char* fmt, ...)
{
	return 0;
}

int Com_sprintfPos(char*, int, int*, char const*, ...)
{
	return 0;
}

int CanKeepStringPointer(char const*)
{
	return 0;
}

char* I_itoa(int, char*, int)
{
	return nullptr;
}

char* va(char const* format, ...)
{
	return nullptr;
}
