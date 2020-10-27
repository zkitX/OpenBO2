#include "pch.h"

#include <tomcrypt.h>

#pragma comment(lib, "tomcrypt.lib")

bdHashMD5::bdHashMD5()
{
    if (register_hash(&md5_desc) == -1)
        bdLogMessage(
            bdLogMessageType::BD_LOG_ERROR,
            "err/",
            "hashmd5",
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "Unable to register hash.");
}

bool bdHashMD5::hash(const char* data, const unsigned int dataSize, char* result, unsigned int* resultSize)
{
    unsigned int* v5; // esi
    int v6; // eax
    int v7; // eax
    char v8; // al

    v5 = resultSize;
    if (*resultSize > 0x10)
        bdLogMessage(
            bdLogMessageType::BD_LOG_WARNING,
            "warn/",
            "hashmd5",
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "MD5 hash only requires result buffer of 16 bytes. Remainder of buffer will not be used");
    resultSize = (unsigned int*)*v5;
    v6 = find_hash("md5");
    v7 = hash_memory(v6, (const unsigned char*)data, dataSize, (unsigned char*)result, (unsigned long*)&resultSize);
    if (v7 == 6)
    {
        bdLogMessage(
            bdLogMessageType::BD_LOG_ERROR,
            "err/",
            "hashmd5",
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "Unable to create md5 hash of less than 16 bytes in length!.");
        v8 = 0;
    }
    else if (v7)
    {
        bdLogMessage(
            bdLogMessageType::BD_LOG_ERROR,
            "err/",
            "hashmd5",
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "Unable to create hash.");
        v8 = 0;
    }
    else
    {
        *v5 = (unsigned int)resultSize;
        v8 = 1;
    }
    return v8;
}
