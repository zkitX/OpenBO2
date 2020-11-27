#pragma once
#include <phys_mutex.h>

volatile struct tagged_void_pointer_t
{
	void* m_ptr;
	unsigned int m_tag;
};

class phys_slot_pool
{
public:
	volatile tagged_void_pointer_t m_first_free_slot;
	unsigned int m_map_key;
	phys_slot_pool* m_hash_next;
	int m_total_slot_count;
	int m_allocated_slot_count;

	unsigned int encode_size_alignment(const unsigned int size, const unsigned int alignment) {
		if (size > 0xFFFF && _tlAssert(__FILE__, __LINE__, "size <= 0xFFFF", nullptr))
			__debugbreak();
		if (alignment > 0xFFFF && _tlAssert(__FILE__, __LINE__, "alignment <= 0xFFFF", nullptr))
			__debugbreak();
		return size | (alignment << 16);
	}

	void init(const unsigned int slot_size, const unsigned int slot_alignment)
	{
		this->m_first_free_slot.m_ptr = 0;
		this->m_first_free_slot.m_tag = 0;
		this->m_map_key = encode_size_alignment(slot_size, slot_alignment);
		this->m_total_slot_count = 0;
		this->m_allocated_slot_count = 0;
	}

	void extra_info_init(void* slot);
	void extra_info_allocate(void* slot);
	void extra_info_free(void* slot);
	void validate_slot(void* slot);
	void free_slot(void* slot);
	void* allocate_slot();
};


class phys_memory_manager
{
public:
	char* m_buffer_start;
	char* m_buffer_end;
	char* m_buffer_cur;
	int m_list_slot_pool_count;
	minspec_read_write_mutex m_slot_pool_map_mutex;
	minspec_hash_table<phys_slot_pool, 64> m_slot_pool_map;
	minspec_mutex m_slot_pool_allocate_mutex;
	phys_slot_pool m_list_preallocated_slot_pools[28];
	int m_list_preallocated_slot_pools_count;

	int allocate(const unsigned int size, const unsigned int alignment);
	phys_slot_pool* allocate_slot_pool();
	phys_slot_pool* get_slot_pool(unsigned int slot_size, unsigned int slot_alignment);
	phys_memory_manager(void* const memory_buffer, const int memory_buffer_size);
};

phys_memory_manager* g_phys_memory_manager;

phys_slot_pool* GET_PHYS_SLOT_POOL(const unsigned int size, const unsigned int alignment) {
	if (!size && _tlAssert(__FILE__, 489, "size > 0", nullptr))
		__debugbreak();
	return g_phys_memory_manager->get_slot_pool(size, alignment);
}