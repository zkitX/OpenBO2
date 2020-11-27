#pragma once

template <typename T>
struct phys_inplace_avl_tree_node
{
	T* m_left;
	T* m_right;
	int m_balance;
};