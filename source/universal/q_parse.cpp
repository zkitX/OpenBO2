#include "q_parse.h"

#include "../qcommon/threads.h"

#pragma warning(disable: 6385)

parseInfo_t* Com_Parse(const char** data_p)
{
    ParseThreadInfo* v1; // eax
    int v2; // ecx
    const char* v3; // edx

    v1 = Com_GetParseThreadInfo();
    v2 = (int)v1->parseInfo[v1->parseInfoNum].token;
    if (*(unsigned char*)(v2 + 1032))
    {
        v3 = *(const char**)(v2 + 1056);
        *(unsigned char*)(v2 + 1032) = 0;
        *data_p = v3;
        *(unsigned int*)(v2 + 1028) = *(unsigned int*)(v2 + 1052);
    }
    return Com_ParseExt(data_p, 1);
}

parseInfo_t* Com_ParseCSV(const char** data_p, int allowLineBreaks)
{
    ParseThreadInfo* v2; // edi
    const char* v3; // edx
    unsigned int v4; // ecx
    parseInfo_t* v5; // esi
    char v6; // bl

    v2 = Com_GetParseThreadInfo();
    v3 = *data_p;
    v4 = 0;
    v5 = &v2->parseInfo[v2->parseInfoNum];
    v5->token[0] = 0;
    if (allowLineBreaks)
    {
        while (*v3 == 13 || *v3 == 10)
            ++v3;
    }
    else if (*v3 == 13 || *v3 == 10)
    {
        return v5;
    }
    v2->prevTokenPos = v2->tokenPos;
    v2->tokenPos = v3;
    v6 = *v3;
    if (*v3)
    {
        while (v6 != 44 && v6 != 10)
        {
            if (v6 != 13)
            {
                if (v6 == 34)
                {
                    while (1)
                    {
                        ++v3;
                        while (*v3 == 34)
                        {
                            if (v3[1] != 34)
                                goto LABEL_11;
                            if (v4 < 0x3FF)
                                v5->token[v4++] = 34;
                            v3 += 2;
                        }
                        if (v4 < 0x3FF)
                            v5->token[v4++] = *v3;
                    }
                }
                if (v4 < 0x3FF)
                    v5->token[v4++] = v6;
            }
        LABEL_11:
            v6 = (v3++)[1];
            if (!v6)
                goto LABEL_28;
        }
        if (*v3)
        {
            if (*v3 != 10)
                ++v3;
            if (*v3)
                goto LABEL_30;
        }
    LABEL_28:
        if (v4)
            goto LABEL_30;
    }
    v3 = 0;
LABEL_30:
    *data_p = v3;
    v5->token[v4] = 0;
    return v5;
}

parseInfo_t* Com_ParseExt(const char** data_p, int allowLineBreaks)
{
    ParseThreadInfo* v2; // eax
    int v3; // edi
    const char* v4; // esi
    parseInfo_t* result; // eax
    bool v6; // zf
    signed int v7; // eax
    const char* v8; // ecx
    int v9; // ebx
    char v10; // bl
    char v11; // al
    signed int v12; // esi
    const char* v13; // ecx
    char v14; // al
    char v15; // al
    signed int v16; // edx
    const char* j; // ecx
    signed int v18; // eax
    char v19; // al
    char v20; // al
    signed int v21; // eax
    char v22; // cl
    char* v23; // eax
    int v24; // eax
    const char* v25; // ebx
    signed int v26; // esi
    signed int k; // eax
    signed int v28; // ecx
    char* v29; // eax
    int v30; // eax
    ParseThreadInfo* parse; // [esp+Ch] [ebp-Ch]
    const char* i; // [esp+10h] [ebp-8h]
    int len; // [esp+14h] [ebp-4h]
    const char** lena; // [esp+14h] [ebp-4h]

    v2 = Com_GetParseThreadInfo();
    v3 = (int)v2->parseInfo[v2->parseInfoNum].token;
    parse = v2;
    if (!data_p && !assertive::Assert_MyHandler(__FILE__, __LINE__, 0, "(data_p)", &pBlock))
        __debugbreak();
    v4 = *data_p;
    len = 0;
    *(unsigned char*)v3 = 0;
    *(unsigned int*)(v3 + 1024) = 0;
    if (!v4)
    {
        *data_p = 0;
        return (parseInfo_t*)v3;
    }
    v6 = *(unsigned char*)(v3 + 1035) == 0;
    *(unsigned int*)(v3 + 1052) = *(unsigned int*)(v3 + 1028);
    *(unsigned int*)(v3 + 1056) = (unsigned int)*data_p;
    if (!v6)
        return Com_ParseCSV(data_p, allowLineBreaks);
    while (1)
    {
        while (1)
        {
        LABEL_8:
            v7 = *v4;
            v8 = v4;
            v9 = 0;
            for (i = v4; v7 <= 32; i = v8)
            {
                if (!v7)
                {
                    *data_p = 0;
                    return (parseInfo_t*)v3;
                }
                if (v7 == 10)
                    ++v9;
                v7 = (v8++)[1];
            }
            v4 = v8;
            if (v9 > 0 && !allowLineBreaks)
                return (parseInfo_t*)v3;
            *(unsigned int*)(v3 + 1028) += v9;
            v10 = *v8;
            if (*v8 != 47)
                goto LABEL_30;
            v11 = v8[1];
            if (v11 != 47)
                break;
            do
            {
                if (v10 == 10)
                    break;
                v10 = (v4++)[1];
            } while (v10);
        }
        if (v11 != 42)
            break;
        while (*v4 != 42 || v4[1] != 47)
        {
            if (*v4 == 10)
                ++* (unsigned int*)(v3 + 1028);
            if (!*++v4)
                goto LABEL_8;
        }
        if (*v4)
            v4 += 2;
    }
LABEL_30:
    parse->prevTokenPos = parse->tokenPos;
    parse->tokenPos = v8;
    if (v10 == 34)
    {
        if (*(unsigned char*)(v3 + 1034))
        {
            *(unsigned char*)v3 = 34;
            v12 = 1;
        }
        else
        {
            v12 = 0;
        }
        v13 = v8 + 1;
        while (1)
        {
            v14 = *v13++;
            if (v14 == 92)
            {
                if (*v13 == 34 || *v13 == 92)
                {
                    v14 = *v13++;
                    goto LABEL_43;
                }
            }
            else if (v14 == 34 || !v14)
            {
                if (*(unsigned char*)(v3 + 1034))
                    *(unsigned char*)(v12++ + v3) = 34;
                *(unsigned char*)(v12 + v3) = 0;
                *data_p = v13;
                *(unsigned int*)(v3 + 1024) = 2;
                return (parseInfo_t*)v3;
            }
            if (*v13 == 10)
                ++* (unsigned int*)(v3 + 1028);
        LABEL_43:
            if (v12 < 1023)
                *(unsigned char*)(v12++ + v3) = v14;
        }
    }
    if (v10 == -1)
    {
        v15 = v8[1];
        v16 = 0;
        for (j = v8 + 2; v15 != -1; v15 = *j++)
        {
            if (!v15)
                break;
            if (v16 < 1023)
                *(unsigned char*)(v16++ + v3) = v15;
        }
        *(unsigned char*)(v16 + v3) = 0;
        *data_p = j;
        *(unsigned int*)(v3 + 1024) = 4;
        return (parseInfo_t*)v3;
    }
    if (*(unsigned char*)(v3 + 1033))
    {
        v18 = 0;
        do
        {
            if (v18 < 1023)
                *(unsigned char*)(v18++ + v3) = v10;
            v10 = (v4++)[1];
        } while (v10 > 32);
        if (v18 == 1024)
            v18 = 0;
        *(unsigned char*)(v18 + v3) = 0;
        *data_p = v4;
        return (parseInfo_t*)v3;
    }
    if (!*(unsigned char*)(v3 + 1037))
        goto LABEL_135;
    if (v10 >= 48 && v10 <= 57)
        goto LABEL_74;
    if (*(unsigned char*)(v3 + 1036) && v10 == 45)
    {
        v19 = v8[1];
        if (v19 < 48 || v19 > 57)
            goto LABEL_104;
        goto LABEL_74;
    }
    if (v10 != 46)
    {
    LABEL_135:
        if (v10 >= 97 && v10 <= 122 || v10 >= 65 && v10 <= 90 || v10 == 95 || v10 == 47 || v10 == 92)
            goto LABEL_115;
        if (!v10)
            goto LABEL_107;
        goto LABEL_104;
    }
    v20 = v8[1];
    if (v20 < 48 || v20 > 57)
    {
    LABEL_104:
        v23 = *(char**)(v3 + 1040);
        if (v23)
        {
            strchr(v23, v10);
            if (v24)
            {
                do
                {
                LABEL_115:
                    v28 = len;
                    do
                    {
                        if (v28 < 1023)
                        {
                            *(unsigned char*)(v28++ + v3) = v10;
                            len = v28;
                        }
                        v10 = (v4++)[1];
                    } while (v10 >= 97 && v10 <= 122 || v10 >= 65 && v10 <= 90 || v10 == 95 || v10 >= 48 && v10 <= 57);
                    if (!v10)
                        goto LABEL_129;
                    v29 = *(char**)(v3 + 1040);
                    if (!v29)
                        goto LABEL_129;
                    strchr(v29, v10);
                } while (v30);
                v28 = len;
            LABEL_129:
                if (v28 == 1024)
                    v28 = 0;
                *(unsigned char*)(v28 + v3) = 0;
                *(unsigned int*)(v3 + 1024) = 3;
                *data_p = v4;
                return (parseInfo_t*)v3;
            }
            v8 = i;
        }
    LABEL_107:
        *(unsigned int*)(v3 + 1024) = 5;
        v25 = punctuation[0];
        for (lena = punctuation; v25; ++lena)
        {
            v26 = strlen(v25);
            for (k = 0; k < v26; ++k)
            {
                if (v8[k] != v25[k])
                    break;
            }
            if (k == v26)
            {
                memcpy((char*)v3, (char*)*lena, v26);
                *(unsigned char*)(v26 + v3) = 0;
                *data_p = &i[v26];
                return (parseInfo_t*)v3;
            }
            v25 = lena[1];
        }
        *(unsigned char*)v3 = *v8;
        *(unsigned char*)(v3 + 1) = 0;
        *data_p = v8 + 1;
        return (parseInfo_t*)v3;
    }
LABEL_74:
    v21 = 0;
    do
    {
        if (v21 < 1023)
            *(unsigned char*)(v21++ + v3) = v10;
        v10 = (v4++)[1];
    } while (v10 >= 48 && v10 <= 57 || v10 == 46);
    if (v10 == 101 || v10 == 69)
    {
        if (v21 < 1023)
            *(unsigned char*)(v21++ + v3) = v10;
        v22 = (v4++)[1];
        if (v22 == 45 || v22 == 43)
        {
            if (v21 < 1023)
                *(unsigned char*)(v21++ + v3) = v22;
            v22 = (v4++)[1];
        }
        do
        {
            if (v21 < 1023)
                *(unsigned char*)(v21++ + v3) = v22;
            v22 = (v4++)[1];
        } while (v22 >= 48 && v22 <= 57);
    }
    if (v21 == 1024)
        v21 = 0;
    *(unsigned char*)(v21 + v3) = 0;
    *(unsigned int*)(v3 + 1024) = 1;
    result = (parseInfo_t*)v3;
    *data_p = v4;
    return result;
}

ParseThreadInfo* Com_GetParseThreadInfo()
{
    if (Sys_IsMainThread())
        return g_parse;
    if (Sys_IsRenderThread())
        return g_parse + 18060;
    if (Sys_IsServerThread())
        return g_parse + 36120;
    if (Sys_IsDatabaseThread())
        return g_parse + 54180;
    if (!assertive::Assert_MyHandler(
        __FILE__,
        __LINE__,
        0,
        &pBlock,
        "No parse context for current thread."))
        __debugbreak();
    return 0;
}

void Com_BeginParseSession(const char* filename)
{
    ParseThreadInfo* v1; // esi
    int v2; // edi
    const char** v3; // ebx
    unsigned int v4; // eax

    v1 = Com_GetParseThreadInfo();
    if (v1->parseInfoNum == 15)
    {
        Com_Printf(21, "Already parsing:\n");
        v2 = 0;
        if (v1->parseInfoNum > 0)
        {
            v3 = &v1->parseInfo[0].parseFile;
            do
            {
                Com_Printf(21, "%i. %s\n", v2++, *v3);
                v3 += 266;
            } while (v2 < v1->parseInfoNum);
        }
        Com_Error(ERR_FATAL, "Com_BeginParseSession: session overflow trying to parse %s", filename);
    }
    v4 = (unsigned int)v1->parseInfo[++v1->parseInfoNum].token;
    *(unsigned int*)(v4 + 1028) = 1;
    *(unsigned int*)(v4 + 1040) = (unsigned int)&pBlock;
    *(unsigned int*)(v4 + 1044) = (unsigned int)&pBlock;
    *(unsigned int*)(v4 + 1048) = (unsigned int)&pBlock;
    *(unsigned int*)(v4 + 1032) = 256;
    *(unsigned short*)(v4 + 1036) = 256;
    *(unsigned int*)(v4 + 1052) = 0;
    *(unsigned int*)(v4 + 1056) = 0;
    *(unsigned int*)(v4 + 1060) = (unsigned int)filename;
}

void Com_EndParseSession()
{
    ParseThreadInfo* v0; // esi

    v0 = Com_GetParseThreadInfo();
    if (!v0->parseInfoNum)
        Com_Error(ERR_FATAL, "Com_EndParseSession: session underflow");
    --v0->parseInfoNum;
}
