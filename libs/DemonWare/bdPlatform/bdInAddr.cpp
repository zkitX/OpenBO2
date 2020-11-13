#include "pch.h"
#include "bdInAddr.h"

bdInAddr::bdInAddr(unsigned int addr)
{
    this->inUn.m_iaddr = addr;
}

bdInAddr::bdInAddr()
{
    this->inUn.m_iaddr = 0xFF00FF00;
}

void bdInAddr::fromString(char const* addr)
{
    this->inUn.m_iaddr = inet_addr(addr);
}

bdInAddr::bdInAddr(char const* addr)
{
    fromString(addr);
}

unsigned int bdInAddr::toString(char* const str, unsigned int size) const
{
    char v3; // bl
    char* v4; // eax
    unsigned int v5; // ecx
    unsigned int v6; // esi
    unsigned int result; // eax

    v3 = 0;
    v4 = inet_ntoa((struct in_addr)*this);
    if (v4)
    {
        v5 = strlen(v4);
        if (size && str)
        {
            v6 = size - 1;
            if (v5 < size - 1)
                v6 = v5;
            memcpy(str, v4, v6);
            str[v6] = 0;
        }
        v3 = 1;
    }
    result = 0;
    if (v3)
        result = strlen(str);
    return result;
}
