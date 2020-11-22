#pragma once

#include <qcommon/com_clients.h>
#include <universal/dvar.h>

enum msgLocErrType_t
{
	LOCMSG_SAFE = 0x0,
	LOCMSG_NOERR = 0x1,
};

struct languageInfo_t
{
	const char* pszName;
	const char* pszNameAbbr;
	int bPresent;
};

languageInfo_t g_languages[16] = { {"French", "Fr", 0}, {"Frenchcan", "Fc", 0}, { "German", "Ge", 0 }, {"Austrian", "As", 0}, {"Italian", "It", 0}, {"Spanish", "Sp", 0}, {"British", "Br", 0}, {"Russian", "Ru", 0}, {"Polish", "Po", 0}, {"Korean", "Kr", 0}, {"Japanese", "Ja", 0}, {"Czech", "Cz", 0}, {"Fulljap", "Fj", 0}, {"Portuguese", "Bp", 0}, {"Mexicanspanish", "Ms", 0} };

int g_currentAsian;

const dvar_t* language;
const dvar_t* loc_forceEnglish;
const dvar_t* loc_language;
const dvar_t* loc_warnings;
const dvar_t* loc_warningsAsErrors;

void SEH_UpdateCurrentLanguage();
int SEH_GetCurrentLanguage();
void SEH_InitLanguage();
void SEH_Init_StringEd();
void SEH_Shutdown_StringEd();
char* SE_GetString_FastFile(const char*);
char* SEH_StringEd_GetString(const char*);
char* SEH_SafeTranslateString(const char*);
int SEH_GetLocalizedTokenReference(char*, const char*, const char*, msgLocErrType_t);
void SEH_ParseDirective(char const*, char* const, char* const);
void SEH_GetBindingForDirective(LocalClientNum_t, char const*, char* const);
bool SEH_ReplaceDirective(LocalClientNum_t, unsigned int*, unsigned int*, char* const, unsigned int);
int SEH_ReplaceDirectiveInStringWithBinding(LocalClientNum_t, char const*, char*);
char* SEH_LocalizeTextMessage(char const*, char const*, msgLocErrType_t);
unsigned int SEH_DecodeLetter(unsigned char, unsigned char, unsigned char, int*);
unsigned int SEH_ReadCharFromString(char const**);
int SEH_PrintStrlen(char const*);
char* SEH_GetLanguageName(int);
char* SEH_GetLanguageNameAbbr(int);
int SEH_StringEd_SetLanguageStrings(int);
void SEH_TranslateMessage(LocalClientNum_t, char const*, char const*, char*, unsigned int);
void SEH_UpdateLanguageInfo(void);
