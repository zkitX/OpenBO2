#pragma once
class bdBytePacker
{
	bool removeBuffer(const void* src, const unsigned int srcSize, const unsigned int offset, unsigned int* newOffset, void* const dest, const unsigned int readSize);

	bool appendBuffer(void* dest, const unsigned int destSize, const unsigned int offset, unsigned int* newOffset, const void* const src, const unsigned int writeSize);

	bool skipBytes(const void* const buffer, const unsigned int bufferSize, const unsigned int offset, unsigned int* newOffset, const unsigned int bytes);

	bool rewindBytes(const void* const buffer, const unsigned int __formal, const unsigned int offset, unsigned int* newOffset, const unsigned int bytes);

	bool appendEncodedUInt16(void* buffer, const unsigned int bufferSize, const unsigned int offset, unsigned int* newOffset, int value);

	bool removeEncodedUInt16(const char* buffer, const unsigned int bufferSize, const unsigned int offset, unsigned int* newOffset, unsigned __int16* value);

	template <typename T>
	bool removeBasicType(const void* buffer, const unsigned int bufferSize, const unsigned int offset, unsigned int* newOffset, T* var) {
		bool result;
		T nvar;

		result = bdBytePacker::removeBuffer(buffer, bufferSize, offset, newOffset, &nvar, sizeof(T));
		if (result)
			*var = nvar;
		return result;
	}

	template <typename T>
	bool appendBasicType(void* buffer, const unsigned int bufferSize, const unsigned int offset, unsigned int* newOffset, T* var) {
		var = (const T)*var;
		return bdBytePacker::appendBuffer(buffer, bufferSize, offset, newOffset, &var, sizeof(T));
	}
};

