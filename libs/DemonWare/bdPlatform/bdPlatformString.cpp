#include <pch.h>

unsigned int bdStrGetToken(const char* const str, const char* const delimeters, char* const tokenBuffer, const unsigned int tokenBufferSize, const char** end)
{
    const char* v5; // ebx
    unsigned int result; // eax
    unsigned int v7; // eax
    char* v8; // edi
    unsigned int v9; // eax
    unsigned int v10; // edx
    unsigned int v11; // esi
    unsigned int v12; // eax
    unsigned int v13; // esi
    unsigned int offset; // [esp+4h] [ebp-Ch]
    unsigned int tokenSize; // [esp+8h] [ebp-8h]

    v7 = 0;
    v9 = 0;
    v5 = str;
    result = 0;
    if (str)
    {
        strspn((char*)str, (char*)delimeters);
        v8 = (char*)&str[v7];
        offset = v7;
        strcspn((char*)&str[v7], (char*)delimeters);
        v10 = v9;
        tokenSize = v9;
        v11 = v9 + 1;
        if (v9 + 1 >= tokenBufferSize)
            v11 = tokenBufferSize;
        v12 = strlen(v8);
        if (v11)
        {
            if (tokenBuffer)
            {
                v13 = v11 - 1;
                if (v12 < v13)
                    v13 = v12;
                memcpy(tokenBuffer, v8, v13);
                v10 = tokenSize;
                tokenBuffer[v13] = 0;
            }
            v5 = str;
        }
        if (end)
            *end = &v5[v10 + offset];
        result = v10;
    }
    return result;
}

bool bdDelimSubstr(const char* str, const char* substr, const char* const delimiters)
{
    unsigned int bdStrToken; // eax
    bool v4; // bl
    bool result; // al
    char* end; // [esp+8h] [ebp-90h]
    char* v7; // [esp+Ch] [ebp-8Ch]
    bool isSubStr; // [esp+13h] [ebp-85h]
    char subStrToken[64]; // [esp+14h] [ebp-84h]
    char strToken[64]; // [esp+54h] [ebp-44h]

    v7 = (char*)str;
    end = (char*)substr;
    isSubStr = 1;
    do
    {
        memset(subStrToken, 0, 0x40u);
        bdStrToken = bdStrGetToken(end, delimiters, subStrToken, 64U, (const char**)&end);
        v4 = bdStrToken != 0;
        if (bdStrToken < 64 && v4)
        {
            memset(strToken, 0, 64U);
            if (bdStrGetToken(v7, delimiters, strToken, 64U, (const char**)&v7) < 64U)
                isSubStr = _stricmp(subStrToken, strToken) == 0;
        }
        result = isSubStr;
    } while (isSubStr && v4);
    return result;
}
