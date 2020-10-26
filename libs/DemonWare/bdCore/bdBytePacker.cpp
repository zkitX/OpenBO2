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
