#include "win_localize.h"

#include <universal/q_parse.h>

_iobuf* Win_InitLocalization(bool isZombies)
{
    _iobuf* v1; // eax
    _iobuf* v2; // esi
    _iobuf* result; // eax
    int v4; // edi
    unsigned int v5; // eax
    unsigned int v6; // edi
    char* v7; // edx
    char v8; // cl

    localization.language = 0;
    localization.strings = 0;
    if (isZombies)
        v1 = FS_FileOpenReadText("localization_zm.txt");
    else
        v1 = FS_FileOpenReadText("localization_mp.txt");
    v2 = v1;
    if (v1 || (result = FS_FileOpenReadText("localization.txt"), (v2 = result) != 0))
    {
        v4 = FS_FileGetFileSize(v2);
#ifdef _DEBUG
        if (v4 >= 4096
            && !Assert_MyHandler(__FILE__, __LINE__, 0, "(size < 4096)", &pBlock))
        {
            __debugbreak();
        }
#endif
        localization.language = language_buffer;
        v5 = v4;
        if ((unsigned int)v4 >= 0xFFF)
            v5 = 4095;
        v6 = FS_FileRead(language_buffer, v5, v2);
        FS_FileClose(v2);
        result = 0;
        if (v6)
        {
            v7 = localization.language;
            localization.language[v6] = 0;
            v8 = *v7;
            if (*v7)
            {
                while (v8 != 10)
                {
                    v8 = *((unsigned char*)&result->_ptr + (unsigned int)v7 + 1);
                    result = (_iobuf*)((char*)result + 1);
                    if (!v8)
                        return 0;
                }
                *((unsigned char*)&result->_ptr + (unsigned int)v7) = 0;
                localization.strings = (char*)&result->_ptr + (unsigned int)v7 + 1;
            }
            result = 0;
        }
        else
        {
            localization.language = 0;
        }
    }
    return result;
}

void Win_ShutdownLocalization()
{
    localization.language = 0;
    localization.strings = 0;
}

char* Win_LocalizeRef(const char* ref)
{
    char* result; // eax
    parseInfo_t* v2; // eax
    int v3; // esi
    parseInfo_t* v4;
    const char* v5; // ST10_4
    bool v6; // al
    const char* v7; // ST10_4
    const char* strings; // [esp+0h] [ebp-4h]

    if (!localization.strings)
        return va("%s", ref);
    Com_BeginParseSession("localization");
    strings = localization.strings;
    v2 = Com_Parse(&strings);
    if (v2->token[0])
    {
        while (1)
        {
            v3 = strcmp(v2->token, ref);
            v4 = Com_Parse(&strings);
            if (!v4->token[0])
            {
                Com_EndParseSession();
                v7 = va("missing value: %s", ref);
                v6 = Assert_MyHandler(
                    __FILE__,
                    __LINE__,
                    0,
                    (const char*)&pBlock,
                    v7);
                goto LABEL_8;
            }
            if (!v3)
                break;
            v2 = Com_Parse(&strings);
            if (!v2->token[0])
                goto LABEL_7;
        }
        Com_EndParseSession();
        result = va("%s", v4);
    }
    else
    {
    LABEL_7:
        Com_EndParseSession();
        v5 = va("unlocalized: %s", ref);
        v6 = Assert_MyHandler(__FILE__, __LINE__, 0, (const char*)&pBlock, v5);
    LABEL_8:
        if (!v6)
            __debugbreak();
        result = va("%s", ref);
    }
    return result;
}

char const* Win_GetLanguage()
{
    return "english"; // more languages based on users localized language later
}
