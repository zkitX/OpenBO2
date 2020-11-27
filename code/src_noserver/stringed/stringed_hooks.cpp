#include "stringed_hooks.h"

#include <universal/assertive.h>

void SEH_UpdateCurrentLanguage()
{
    int v0; // eax

    if (!loc_language
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(loc_language)", nullptr))
    {
        __debugbreak();
    }
    v0 = Dvar_GetInt(loc_language);
    switch (v0)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 8:
        break;
    case 10:
    case 11:
    case 13:
        g_currentAsian = 1;
        break;
    default:
        g_currentAsian = 0;
        break;
    }
    if (v0 == 13)
        Dvar_SetString(language, g_languages[11].pszName);
    else
        Dvar_SetString(language, g_languages[v0].pszName);
}

int SEH_GetCurrentLanguage()
{
    if (loc_language)
        return Dvar_GetInt(loc_language);
    Com_PrintWarning(10, "SEH_GetCurrentLanguage called before it is initialized, returning ENGLISH\n");
    return 0;
}

void SEH_InitLanguage()
{
    if (!language)
    {
        language = _Dvar_RegisterString("language", nullptr, 0, nullptr);
        loc_language = _Dvar_RegisterInt("loc_language", 0, 0, 15, 0x21u, "Language");
        loc_forceEnglish = _Dvar_RegisterBool("loc_forceEnglish", 0, 0x21u, "Force english localized strings");
        loc_warnings = _Dvar_RegisterBool("loc_warnings", 0, 0, "Enable localization warnings");
        loc_warningsAsErrors = _Dvar_RegisterBool("loc_warningsAsErrors", 0, 0, "Throw an error for any unlocalized string");
    }
    SEH_UpdateCurrentLanguage();
}

void SEH_Init_StringEd()
{
}

void SEH_Shutdown_StringEd()
{
}

char* SE_GetString_FastFile(const char*)
{
    return nullptr;
}

char* SEH_StringEd_GetString(const char*)
{
    return nullptr;
}

char* SEH_SafeTranslateString(const char*)
{
    return nullptr;
}

int SEH_GetLocalizedTokenReference(char*, const char*, const char*, msgLocErrType_t)
{
    return 0;
}

void SEH_ParseDirective(char const*, char* const, char* const)
{
}

void SEH_GetBindingForDirective(LocalClientNum_t, char const*, char* const)
{
}

bool SEH_ReplaceDirective(LocalClientNum_t, unsigned int*, unsigned int*, char* const, unsigned int)
{
    return false;
}

int SEH_ReplaceDirectiveInStringWithBinding(LocalClientNum_t, char const*, char*)
{
    return 0;
}

char* SEH_LocalizeTextMessage(char const*, char const*, msgLocErrType_t)
{
    return nullptr;
}

unsigned int SEH_DecodeLetter(unsigned char, unsigned char, unsigned char, int*)
{
    return 0;
}

unsigned int SEH_ReadCharFromString(char const**)
{
    return 0;
}

int SEH_PrintStrlen(char const*)
{
    return 0;
}

char* SEH_GetLanguageName(int)
{
    return nullptr;
}

char* SEH_GetLanguageNameAbbr(int)
{
    return nullptr;
}

int SEH_StringEd_SetLanguageStrings(int)
{
    return 0;
}

void SEH_TranslateMessage(LocalClientNum_t, char const*, char const*, char*, unsigned int)
{
}

void SEH_UpdateLanguageInfo(void)
{
}
