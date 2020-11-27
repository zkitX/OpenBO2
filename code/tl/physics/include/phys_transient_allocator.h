#pragma once
#include <Windows.h>
#include <phys_mutex.h>

class phys_transient_allocator
{
public:
	enum {
		BLOCK_SIZE = 0x4000,
		BLOCK_ALIGNMENT = 0x4,
	};

	struct block_header
	{
		unsigned int m_block_size;
		unsigned int m_block_alignment;
		block_header* m_next_block;
	};

	struct allocator_state
	{
		block_header* m_first_block;
		char* m_cur;
		char* m_end;
		unsigned int m_total_memory_allocated;
	};

	block_header* m_first_block;
	char* m_cur;
	char* m_end;
	unsigned int m_total_memory_allocated;
	minspec_read_write_mutex m_mutex;
	void* m_slot_pool;

	void resize() {

		if (!this->m_slot_pool)
		{
			this->m_slot_pool = GET_PHYS_SLOT_POOL(0x4000u, 4u);
		}
	}

	int mt_allocate_internal(const int size, const int alignment) {

		int v3;
		int v4;
		char* v5;
		int v6;
		int v7;
		signed __int32 v8;
		int v10;
		int alignmenta;

		v3 = alignment;
		v4 = alignment - 1;
		v5 = this->m_cur;
		alignmenta = v4;
		v6 = ~v4;
		v7 = v6 & (unsigned int)&v5[v3 - 1];
		v10 = v6;
		v8 = v7 + size;
		if ((char*)(v7 + size) > this->m_end)
			return 0;
		while ((char*)_InterlockedCompareExchange((volatile LONG*)&this->m_cur, v8, (signed __int32)v5) != v5)
		{
			v5 = this->m_cur;
			v7 = v10 & (unsigned int)&v5[alignmenta];
			v8 = v7 + size;
			if ((char*)(v7 + size) > this->m_end)
				return 0;
		}
		return v7;
	}

	void* mt_allocate(const int size, const int alignment, const int no_error, const char* error_msg) {
		void* result; // eax
		int m_size;
		int v9; // edi
		int v10; // eax
		char* current; // edx
		int v12; // eax

		this->m_mutex.ReadLock();
		m_size = mt_allocate_internal(size, alignment);
		this->m_mutex.ReadUnlock();
		result = (void*)size;
		if (size)
			return result;
		while (_InterlockedCompareExchange((volatile LONG*)&this->m_mutex, 0, 1) != 1)
			;
		size = 0;
		InterlockedExchange((volatile LONG*)&size, 0);
		v9 = ~(alignment - 1);
		v10 = v9 & (int)&this->m_cur[alignment - 1];
		if ((char*)(v10 + size) > this->m_end || (this->m_cur = (char*)(v10 + size), (size = v10) == 0))
		{
			phys_transient_allocator::resize(this);
			current = this->m_cur;
			v12 = v9 & (unsigned int)&current[alignment - 1];
			if ((char*)(v12 + size) > this->m_end)
			{
				size = 0;
				goto LABEL_8;
			}
			this->m_cur = (char*)(v12 + size);
			size = v9 & (unsigned int)&current[alignment - 1];
			if (!v12)
			{
			LABEL_8:
				if (_tlAssert(
					"c:\\t6\\code\\tl\\physics\\include\\phys_transient_allocator.h",
					99,
					"ptr",
					"transient allocation too large, increase block_size."))
				{
					__debugbreak();
				}
				goto LABEL_10;
			}
		}
	LABEL_10:
		minspec_read_write_mutex::WriteUnlock(&this->m_mutex);
		if (!size
			&& !no_error
			&& _tlAssert("c:\\t6\\code\\tl\\physics\\include\\phys_transient_allocator.h", 103, "ptr || no_error", error_msg))
		{
			__debugbreak();
		}
		return (void*)size;
	}
};
