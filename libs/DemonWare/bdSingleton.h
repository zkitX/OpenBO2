#pragma once
#include <pch.h>

template <class T> class bdSingleton {
private:
	bdSingleton<T> m_instance;
public:
	T getInstance() {
		T* result;
		T* v1;

		result = m_instance;
		if (m_instance)
			return result;
		v1 = (T*)bdMemory::allocate(0x4790);
		if (!v1) {
			m_instance = 0;
			DebugBreak();
			return m_instance;
		}

	}
	int destroyInstance() {

	}
};