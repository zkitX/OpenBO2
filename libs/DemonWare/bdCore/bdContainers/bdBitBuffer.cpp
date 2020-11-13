#include "pch.h"
#include "bdBitBuffer.h"

void bdBitBuffer::setTypeCheck(const bool flag)
{
    this->m_typeChecked = flag;
}

bool bdBitBuffer::getTypeCheck()
{
    return this->m_typeChecked;
}

void bdBitBuffer::typeToString(const bdBitBufferDataType type, char* const strBuffer, const unsigned int strLength)
{
    signed int dataType;
    char* outType;
    unsigned int typeLen;
    unsigned int bufferLen;
    const char* dataTypeDescs[23];

    dataTypeDescs[0] = "NoType";
    dataTypeDescs[1] = "Bool";
    dataTypeDescs[2] = "Char8";
    dataTypeDescs[3] = "UChar8";
    dataTypeDescs[4] = "WChar16";
    dataTypeDescs[5] = "Int16";
    dataTypeDescs[6] = "UInt16";
    dataTypeDescs[7] = "Int32";
    dataTypeDescs[8] = "UInt32";
    dataTypeDescs[9] = "Int64";
    dataTypeDescs[10] = "UInt64";
    dataTypeDescs[11] = "RangedInt32";
    dataTypeDescs[12] = "RangeUInt32";
    dataTypeDescs[13] = "Float32";
    dataTypeDescs[14] = "Float64";
    dataTypeDescs[15] = "RangeFloat32";
    dataTypeDescs[16] = "String";
    dataTypeDescs[17] = "String";
    dataTypeDescs[18] = "MultiByteString";
    dataTypeDescs[19] = "Blob";
    dataTypeDescs[20] = "NaN";
    dataTypeDescs[21] = "FullType";
    dataTypeDescs[22] = "Unknown Type";

    if (type <= BD_BB_MAX_TYPE)
        dataType = type < 0 ? 0 : type;
    else
        dataType = 22;
    outType = (char*)dataTypeDescs[dataType];
    typeLen = strlen(dataTypeDescs[dataType]);
    if (strLength && strBuffer) {
        bufferLen = strLength - 1;
        if (typeLen < strLength - 1)
            bufferLen = typeLen;
        memcpy(strBuffer, outType, bufferLen);
        strBuffer[bufferLen] = 0;
    }
}

bool bdBitBuffer::readBits(void* bits, unsigned int numBits)
{
    unsigned int c_numBits;
    unsigned int c_readPosition;
    unsigned int shiftedReadPosition;
    bdBitBuffer* instance;
    bdBitBuffer* rollbackInstance;
    unsigned int nextByteIndex;
    unsigned __int8 v8; // dl
    int v9; // ebx
    unsigned int v10; // eax
    char v11; // al
    char v12; // cl
    bool v13; // zf
    bool result;
    char* dest;

    c_numBits = numBits;
    instance = this;
    rollbackInstance = this;
    if (!numBits)
        return 1;
    c_readPosition = this->m_readPosition;
    if (c_readPosition + numBits <= this->m_maxWritePosition)
    {
        dest = (char*)bits;
        shiftedReadPosition = c_readPosition >> 3;
        while (shiftedReadPosition < instance->m_data.m_size)
        {
            if (c_numBits >= 8)
                c_numBits = 8;
            v8 = instance->m_data.m_data[shiftedReadPosition];
            v9 = instance->m_readPosition & 7;
            v10 = shiftedReadPosition + 1;
            nextByteIndex = v10;
            if (v9 + c_numBits <= 8)
            {
                v12 = (v8 >> v9) & (255 >> (8 - c_numBits));
            }
            else
            {
                if (v10 >= instance->m_data.m_size)
                    break;
                v11 = instance->m_data.m_data[v10];
                instance = rollbackInstance;
                v12 = (255 >> (8 - c_numBits)) & ((v8 >> v9) | (v11 << (8 - v9)));
            }
            *dest = v12;
            instance->m_readPosition += c_numBits;
            v13 = numBits == c_numBits;
            numBits -= c_numBits;
            ++dest;
            if (v13)
                return 1;
            shiftedReadPosition = nextByteIndex;
            c_numBits = numBits;
        }
        instance->m_failedRead = 1;
        result = 0;
    }
    else
    {
        this->m_failedRead = 1;
        result = 0;
    }
    return result;

}

bool bdBitBuffer::readDataType(const bdBitBufferDataType expectedDataType)
{
    bool result; // al
    signed int v3; // esi
    bool v4; // bl
    unsigned int dataType32; // [esp+0h] [ebp-58h]
    char string1[40]; // [esp+4h] [ebp-54h]
    char string2[40]; // [esp+2Ch] [ebp-2Ch]

    result = 1;
    if (this->m_typeChecked)
    {
        dataType32 = 0;
        result = bdBitBuffer::readRangedUInt32(&dataType32, 0, 0x1Fu, 0);
        if (result)
        {
            v3 = dataType32;
            v4 = dataType32 == expectedDataType;
            if (dataType32 != expectedDataType)
            {
                bdBitBuffer::typeToString(expectedDataType, string1, 0x28u);
                bdBitBuffer::typeToString((bdBitBufferDataType)v3, string2, 0x28u);
                bdLogMessage(
                    bdLogMessageType::BD_LOG_ERROR,
                    "err/",
                    "bdCore/bitBuffer",
                    __FILE__,
                    __FUNCTION__,
                    __LINE__,
                    "Expected: %s , read: %s ",
                    string1,
                    string2);
            }
            result = v4;
        }
    }
    return result;
}

char bdBitBuffer::readInt32(int* i)
{
    char result; // al
    int ni; // [esp+4h] [ebp-4h]

    if (!bdBitBuffer::readDataType(BD_BB_SIGNED_INTEGER32_TYPE) || !bdBitBuffer::readBits(&ni, 0x20u))
        return 0;
    result = 1;
    *i = ni;
    return result;
}

char bdBitBuffer::readUInt32(unsigned int* u)
{
    char result; // al
    unsigned int nu; // [esp+4h] [ebp-4h]

    if (!bdBitBuffer::readDataType(BD_BB_UNSIGNED_INTEGER32_TYPE) || !bdBitBuffer::readBits(&nu, 0x20u))
        return 0;
    result = 1;
    *u = nu;
    return result;
}

bdBitBuffer::~bdBitBuffer()
{
    bdBitBuffer* instance;

    instance = this;
    delete this;
    bdMemory::deallocate(this->m_data.m_data);
    instance->m_data.m_data = 0;
    instance->m_data.m_size = 0;
    instance->m_data.m_capacity = 0;
    bdReferenceable::~bdReferenceable();
}

bool bdBitBuffer::readRangedUInt32(unsigned int* u, const unsigned int begin, const unsigned int end, const bool typeChecked)
{
    unsigned int v5; // ebx
    bdBitBuffer* v6; // esi
    bool result; // al
    unsigned int v8; // eax
    unsigned int v9; // ecx

    v5 = end;
    v6 = this;
    if (typeChecked)
    {
        result = bdBitBuffer::readDataType(BD_BB_RANGED_UNSIGNED_INTEGER32_TYPE);
        if (v6->m_typeChecked)
        {
            if (!result
                || !bdBitBuffer::readDataType(BD_BB_UNSIGNED_INTEGER32_TYPE)
                || !bdBitBuffer::readBits((void*)&typeChecked, 0x20u)
                || !bdBitBuffer::readDataType(BD_BB_UNSIGNED_INTEGER32_TYPE)
                || !bdBitBuffer::readBits((void*)&end, 0x20u))
            {
                return 0;
            }
            if (begin != typeChecked || v5 != end)
                bdLogMessage(
                    bdLogMessageType::BD_LOG_ERROR,
                    "err/",
                    "bdCore/bitBuffer",
                    __FILE__,
                    __FUNCTION__,
                    __LINE__,
                    "Range error. Expected: (%u,%u), read: (%u,%u)",
                    begin,
                    v5,
                    typeChecked,
                    end);
        }
        else if (!result)
        {
            return result;
        }
    }
    v8 = 0;
    if (v5 != begin)
        v8 = bdBitOperations::highBitNumber(v5 - begin) + 1;
    *&typeChecked = 0;
    if (!bdBitBuffer::readBits((void*)typeChecked, v8))
        return 0;
    v9 = begin + typeChecked;
    result = 1;
    *u = begin + typeChecked;
    if (v9 <= v5)
    {
        if (v9 < begin)
            v9 = begin;
        *u = v9;
    }
    else
    {
        *u = v5;
    }
    return result;
}

void bdBitBuffer::writeBits(const void* bits, const unsigned int numBits)
{
    unsigned int v3; // eax
    bdBitBuffer* v4; // esi
    unsigned int v5; // edi
    char* v6; // edx
    unsigned int v7; // ebx
    int v8; // eax
    char v9; // cl
    char v10; // dl
    char* v11; // ST10_4
    unsigned int v12; // edi
    char v13; // cl
    char v14; // bl
    unsigned int v15; // eax
    int upShift; // [esp+10h] [ebp-1Ch]
    unsigned int v17; // [esp+1Ch] [ebp-10h]
    unsigned int bitsToWrite; // [esp+20h] [ebp-Ch]
    unsigned int bitsWritten; // [esp+24h] [ebp-8h]

    v3 = numBits;
    v4 = this;
    v5 = (numBits + this->m_writePosition - 1) >> 3;
    if (v5 >= this->m_data.m_size)
    {
        bdFastArray<unsigned char>::ensureCapacity(&this->m_data, v5 + 1);
        v6 = (char*)v4->m_data.m_data;
        v4->m_data.m_size = v5 + 1;
        v3 = numBits;
        v6[v5] = 0;
    }
    v7 = v3;
    bitsToWrite = v3;
    if (v3)
    {
        v17 = (v3 - 1) >> 3;
        do
        {
            v8 = v4->m_writePosition & 7;
            upShift = v4->m_writePosition & 7;
            if (v7 >= 8 - v8)
            {
                v9 = 8 - v8;
                bitsWritten = 8 - v8;
            }
            else
            {
                v9 = v7;
                bitsWritten = v7;
            }
            v10 = (255 >> (8 - v8)) | (-1 << (v8 + v9));
            v12 = (numBits - v7) >> 3;
            v13 = (numBits - v7) & 7;
            if (v17 <= v12)
                v14 = 0;
            else
                v14 = *((_BYTE *)bits + v12 + 1);
            v11 = (char*)&v4->m_data.m_data[v4->m_writePosition >> 3];
            *v11 = v10 & *v11 | ~v10 & (((v14 << (8 - v13)) | (*((_BYTE *)bits + v12) >> v13)) << upShift);
            v4->m_writePosition += bitsWritten;
            v15 = v4->m_writePosition;
            v7 = bitsToWrite - bitsWritten;
            bitsToWrite -= bitsWritten;
            if (v4->m_maxWritePosition > v15)
                v15 = v4->m_maxWritePosition;
            v4->m_maxWritePosition = v15;
        } while (v7);
    }
}

bdBitBuffer::bdBitBuffer(unsigned int capacityBits, const bool typeChecked)
{
    bdBitBuffer* instance; // esi

    instance = this;
    
}

bdBitBuffer::bdBitBuffer(const char* bits, const unsigned int numBits, const bool dataHasTypeCheckedBit)
{
}

void bdBitBuffer::writeRangedUInt32(const unsigned int u, const unsigned int begin, const unsigned int end, const bool typeChecked)
{
}

void bdBitBuffer::writeDataType(const bdBitBufferDataType dataType)
{
}
