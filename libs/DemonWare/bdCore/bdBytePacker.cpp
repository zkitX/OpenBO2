#include "pch.h"

bool bdBytePacker::removeBuffer(const void* src, const unsigned int srcSize, const unsigned int offset, unsigned int* newOffset, void* const dest, const unsigned int readSize)
{
    bool result; // al

    result = 1;
    *newOffset = offset + readSize;
    if (dest && src)
    {
        if (offset + readSize > srcSize)
        {
            bdLogMessage(
                bdLogMessageType::BD_LOG_WARNING,
                "warn/",
                "byte packer",
                __FILE__,
                __FUNCTION__,
                __LINE__,
                "Not enough data left to read %u bytes.",
                readSize);
            return 0;
        }
        if (offset > srcSize)
            return 0;
        memmove((char*)dest, (char*)src + offset, readSize);
        result = 1;
    }
    return result;
}

bool bdBytePacker::appendBuffer(void* dest, const unsigned int destSize, const unsigned int offset, unsigned int* newOffset, const void* const src, const unsigned int writeSize)
{
    unsigned int v6; // ecx
    char result; // al
    bool v8; // bl

    v6 = offset + writeSize;
    result = 1;
    *newOffset = offset + writeSize;
    if (dest && src)
    {
        v8 = v6 <= destSize;
        if (v6 > destSize)
            bdLogMessage(
                bdLogMessageType::BD_LOG_WARNING,
                "warn/",
                "byte packer",
                __FILE__,
                __FUNCTION__,
                __LINE__,
                "Not enough room left to write %u bytes.",
                writeSize);
        if (offset <= destSize && v8)
        {
            memmove((char*)dest + offset, (char*)src, writeSize);
            result = 1;
        }
        else
        {
            result = 0;
        }
    }
    return result;
}

bool bdBytePacker::skipBytes(const void* const buffer, const unsigned int bufferSize, const unsigned int offset, unsigned int* newOffset, const unsigned int bytes)
{
    bool result; // al

    result = 0;
    *newOffset = offset + bytes;
    if (buffer)
        result = offset + bytes <= bufferSize;
    return result;
}

bool bdBytePacker::rewindBytes(const void* const buffer, const unsigned int __formal, const unsigned int offset, unsigned int* newOffset, const unsigned int bytes)
{
    bool result; // al

    *newOffset = offset - bytes;
    result = 0;
    if (buffer)
        result = offset >= bytes;
    return result;
}

bool bdBytePacker::appendEncodedUInt16(void* buffer, const unsigned int bufferSize, const unsigned int offset, unsigned int* newOffset, int value)
{
    unsigned int v5; // ST08_4
    char result; // al

    if ((unsigned __int16)value <= 0x7Fu)
    {
        HIBYTE_(value) = value;
        result = bdBytePacker::appendBuffer(buffer, bufferSize, offset, newOffset, (char*)&value + 3, 1u);
    }
    else
    {
        HIBYTE_(value) = BYTE1(value) | 0x80;
        result = 0;
        if (bdBytePacker::appendBuffer(buffer, bufferSize, offset, newOffset, (char*)&value + 3, 1u))
        {
            v5 = *newOffset;
            HIBYTE_(value) = value;
            if (bdBytePacker::appendBuffer(buffer, bufferSize, v5, newOffset, (char*)&value + 3, 1u))
                result = 1;
        }
    }
    return result;
}

bool bdBytePacker::removeEncodedUInt16(const char* buffer, const unsigned int bufferSize, const unsigned int offset, unsigned int* newOffset, unsigned __int16* value)
{
    unsigned __int8 v5; // bl
    char result; // al
    unsigned __int8 v7; // bl
    unsigned __int8 firstByte; // [esp+Eh] [ebp-2h]
    unsigned __int8 dest; // [esp+Fh] [ebp-1h]

    v5 = 0;
    if (!bdBytePacker::removeBuffer(buffer, bufferSize, offset, newOffset, &dest, 1u))
    {
        result = 0;
        goto LABEL_6;
    }
    v5 = dest;
    result = 1;
    if (dest < 0x80u)
    {
    LABEL_6:
        *value = v5;
        return result;
    }
    firstByte = dest & 0x7F;
    v7 = 0;
    if (bdBytePacker::removeBuffer(buffer, bufferSize, *newOffset, newOffset, &dest, 1u))
    {
        v7 = dest;
        result = 1;
    }
    else
    {
        result = 0;
    }
    *value = v7 + (firstByte << 8);
    return result;
}
