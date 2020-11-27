#pragma once
#include <rigid_body.h>

class phys_gjk_geom_id_pair_key
{
public:
	unsigned int m_id1;
	unsigned int m_id2;
};

const class cached_simplex_info
{
public:
	phys_vec3 m_indices[3];
};

class phys_gjk_cache_info
{
public:
	phys_vec3 m_support_dir;
	cached_simplex_info m_support_a;
	cached_simplex_info m_support_b;
	int m_support_count;
	phys_gjk_geom_id_pair_key m_key;
	unsigned int m_flags;
};


const class phys_gjk_geom
{
public:
	virtual void set_simplex(const phys_vec3* simplex_inds, const int w_set, const phys_vec3* normal, cached_simplex_info* cache_info);
	virtual float get_geom_radius();
	virtual const phys_vec3* support_only(const phys_mat44* xform, const phys_vec3* v);
};
