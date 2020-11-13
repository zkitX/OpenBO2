#pragma once
#include "../bdReferenceable.h"

enum bdBitBufferDataType
{
	BD_BB_NO_TYPE = 0x0,
	BD_BB_BOOL_TYPE = 0x1,
	BD_BB_SIGNED_CHAR8_TYPE = 0x2,
	BD_BB_UNSIGNED_CHAR8_TYPE = 0x3,
	BD_BB_WCHAR16_TYPE = 0x4,
	BD_BB_SIGNED_INTEGER16_TYPE = 0x5,
	BD_BB_UNSIGNED_INTEGER16_TYPE = 0x6,
	BD_BB_SIGNED_INTEGER32_TYPE = 0x7,
	BD_BB_UNSIGNED_INTEGER32_TYPE = 0x8,
	BD_BB_SIGNED_INTEGER64_TYPE = 0x9,
	BD_BB_UNSIGNED_INTEGER64_TYPE = 0xA,
	BD_BB_RANGED_SIGNED_INTEGER32_TYPE = 0xB,
	BD_BB_RANGED_UNSIGNED_INTEGER32_TYPE = 0xC,
	BD_BB_FLOAT32_TYPE = 0xD,
	BD_BB_FLOAT64_TYPE = 0xE,
	BD_BB_RANGED_FLOAT32_TYPE = 0xF,
	BD_BB_SIGNED_CHAR8_STRING_TYPE = 0x10,
	BD_BB_UNSIGNED_CHAR8_STRING_TYPE = 0x11,
	BD_BB_MBSTRING_TYPE = 0x12,
	BD_BB_BLOB_TYPE = 0x13,
	BD_BB_NAN_TYPE = 0x14,
	BD_BB_FULL_TYPE = 0x15,
	BD_BB_MAX_TYPE = 0x20,
};

template<typename T>
class bdFastArray {
public:
	T* m_data;
	unsigned int m_capacity;
	unsigned int m_size;

	static void ensureCapacity(bdFastArray<T>* m_instance, const unsigned int capacity) {
		bdFastArray<T>* instance; // esi
		unsigned int instanceCapacity; // eax
		unsigned int v4; // ecx
		int v5; // edi
		char* v6; // ebx

		instance = m_instance;
		instanceCapacity = m_instance->m_capacity;
		if (instanceCapacity < capacity)
		{
			v4 = capacity - instanceCapacity;
			if (capacity - instanceCapacity <= instanceCapacity)
				v4 = instanceCapacity;
			v5 = instanceCapacity + v4;
			v6 = 0;
			if (instanceCapacity + v4)
			{
				v6 = bdMemory::allocate(instanceCapacity + v4);
				if (instance->m_size)
					memcpy(v6, instance->m_data, instance->m_size);
			}
			bdMemory::deallocate(instance->m_data);
			instance->m_capacity = v5;
			instance->m_data = v6;
		}
	}
};

class bdBitBuffer : bdReferenceable
{
private:
	bdFastArray<unsigned char> m_data;
	unsigned int m_writePosition;
	unsigned int m_maxWritePosition;
	unsigned int m_readPosition;
	bool m_failedRead;
	bool m_typeChecked;
public:
	void setTypeCheck(const bool flag);
	bool getTypeCheck();
	void typeToString(const bdBitBufferDataType type, char* const strBuffer, const unsigned int strLength);
	bool readBits(void* bits, unsigned int numBits);
	bool readDataType(const bdBitBufferDataType expectedDataType);
	char readInt32(int* i);
	char readUInt32(unsigned int* u);
	~bdBitBuffer();
	bool readRangedUInt32(unsigned int* u, const unsigned int begin, const unsigned int end, const bool typeChecked);
	void writeBits(const void* bits, const unsigned int numBits);
	bdBitBuffer(unsigned int capacityBits, const bool typeChecked);
	bdBitBuffer(const char* bits, const unsigned int numBits, const bool dataHasTypeCheckedBit);
	void writeRangedUInt32(const unsigned int u, const unsigned int begin, const unsigned int end, const bool typeChecked);
	void writeDataType(const bdBitBufferDataType dataType);
};
