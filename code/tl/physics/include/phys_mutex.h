#pragma once
#include <Windows.h>

#include <tl_system.h>
#include <phys_mem_new.h>

class minspec_mutex
{
public:
	volatile unsigned int m_token;

	void Unlock() {
		LONG Target;

		Target = 0;
		InterlockedExchange(&Target, 0);
		if (this->m_token != 1
			&& _tlAssert(
				__FILE__,
				__LINE__,
				"GetStuff32(&m_token) == 1",
				nullptr))
		{
			__debugbreak();
		}
		if (_InterlockedCompareExchange((volatile LONG*)this, 0, 1) != 1
			&& _tlAssert(__FILE__, __LINE__, "retv", nullptr))
		{
			__debugbreak();
		}
	}
};

class minspec_read_write_mutex
{
public:
	volatile unsigned int m_count;

	void ReadLock() {
		volatile unsigned int count;
		LONG Target;
		do
		{
			while (this->m_count < 1)
				;
			count = this->m_count;
		} while (_InterlockedCompareExchange((volatile LONG*)this, count + 1, this->m_count) != count);
		Target = 0;
		InterlockedExchange(&Target, 0);
	}
	void ReadUnlock() {
		signed __int32 count;

		do
		{
			count = this->m_count;
			if (this->m_count <= 1
				&& _tlAssert(__FILE__, __LINE__, "count > 1", nullptr))
			{
				__debugbreak();
			}
		}
		while (_InterlockedCompareExchange((volatile LONG*)this, count - 1, count) != count);
	}
	void WriteUnlock() {
		LONG Target;

		Target = 0;
		InterlockedExchange(&Target, 0);
		if (this->m_count
			&& _tlAssert(
				__FILE__,
				__LINE__,
				"GetStuff32(&m_count) == 0",
				nullptr))
		{
			__debugbreak();
		}
		if (_InterlockedCompareExchange((volatile LONG*)this, 1, 0) != 0
			&& _tlAssert(__FILE__, __LINE__, "retv", nullptr))
		{
			__debugbreak();
		}
	}
};

template <typename T, int N>
class minspec_hash_table {
	T* m_hash_table[N];
	unsigned int m_mod;
	unsigned int m_highest_collision;
	unsigned int m_total_collisions;

	void add(unsigned int key, phys_slot_pool* entry_to_add) {
		phys_slot_pool* v4; // edx
		unsigned int v5; // ebx
		unsigned int i; // ecx
		phys_slot_pool* j; // eax
		unsigned int v8; // edx
		unsigned int v9; // eax
		unsigned int v10; // edi
		unsigned int v11; // ecx
		unsigned int v12; // edx
		unsigned int v13; // eax
		unsigned int v14; // eax
		unsigned int k; // edi
		phys_slot_pool* v16; // ecx
		int v17; // edx
		phys_slot_pool* entry_list[64]; // [esp+Ch] [ebp-210h]
		unsigned int v19; // [esp+10Ch] [ebp-110h]
		unsigned int total_collisions; // [esp+110h] [ebp-10Ch]
		unsigned int mod_i; // [esp+114h] [ebp-108h]
		unsigned int collision_counts[64]; // [esp+118h] [ebp-104h]

		v4 = this->m_hash_table[key % this->m_mod];
		if (v4)
		{
			while (v4->m_map_key != key)
			{
				v4 = v4->m_hash_next;
				if (!v4)
					goto LABEL_7;
			}
			if (_tlAssert(
				__FILE__,
				__LINE__,
				"find(key) == NULL",
				nullptr))
			{
				__debugbreak();
			}
		}
	LABEL_7:
		entry_list[0] = entry_to_add;
		v5 = 1;
		for (i = 0; i < 0x40; ++i)
		{
			for (j = this->m_hash_table[i]; j; ++v5)
			{
				entry_list[v5] = j;
				j = j->m_hash_next;
			}
			this->m_hash_table[i] = 0;
		}
		this->m_mod = v5;
		this->m_highest_collision = 100000;
		this->m_total_collisions = 100000;
		v8 = v5;
		mod_i = v5;
		if (v5 < 0x40)
		{
			v19 = 100000;
			do
			{
				if (v8)
					memset(collision_counts, 0, 4 * v8);
				v9 = 0;
				v10 = 0;
				v11 = 0;
				total_collisions = 0;
				if (v5)
				{
					do
					{
						v12 = entry_list[v11]->m_map_key % mod_i;
						v13 = ++collision_counts[v12];
						if (v13 > v10)
							v10 = collision_counts[v12];
						total_collisions += v13;
						++v11;
					} while (v11 < v5);
					v8 = mod_i;
					v9 = total_collisions;
				}
				if (v9 < v19)
				{
					this->m_mod = v8;
					this->m_highest_collision = v10;
					v19 = v9;
					this->m_total_collisions = v9;
				}
				mod_i = ++v8;
			} while (v8 < 0x40);
		}
		v14 = this->m_mod;
		if ((!v14 || v14 >= 0x40)
			&& _tlAssert(
				__FILE__,
				__LINE__,
				"m_mod > 0 && m_mod < TABLE_SIZE",
				nullptr))
		{
			__debugbreak();
		}
		for (k = 0; k < v5; this->m_hash_table[v17] = v16)
		{
			v16 = entry_list[k];
			v17 = v16->m_map_key % this->m_mod;
			++k;
			v16->m_hash_next = this->m_hash_table[v17];
		}
	}
};