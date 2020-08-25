#include "win_localize.h"

#include "../qcommon/common.h"
#include "../universal/q_parse.h"

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
                v6 = assertive::Assert_MyHandler(
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
        v6 = assertive::Assert_MyHandler(__FILE__, __LINE__, 0, (const char*)&pBlock, v5);
    LABEL_8:
        if (!v6)
            __debugbreak();
        result = va("%s", ref);
    }
    return result;
}
