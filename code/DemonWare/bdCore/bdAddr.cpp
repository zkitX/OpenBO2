#include "pch.h"
#include "bdAddr.h"

int bdSnprintf(char* buf, unsigned int maxlen, char const *format, ...)
{
    int v3; // edi
    va_list ap; // [esp+1Ch] [ebp+14h]

    va_start(ap, format);
    v3 = _vscprintf(format, ap);
    vsnprintf_s(buf, maxlen, 0xFFFFFFFF, format, ap);
    return v3;
}
