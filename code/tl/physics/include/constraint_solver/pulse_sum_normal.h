#pragma once
#include <rigid_body.h>

class pulse_sum_node : phys_link_list_base<pulse_sum_node>
{
	phys_mat44 m_world_inv_inertia;
	phys_vec3 t_vel;
	phys_vec3 a_vel;
	float m_inv_mass;
	rigid_body* m_rb;
};

class pulse_sum_cache
{
public:
	float m_pulse_sum;
};

class pulse_sum_normal : phys_link_list_base<pulse_sum_normal>
{
	phys_vec3 m_ud;
	phys_vec3 m_b1_r;
	phys_vec3 m_b2_r;
	phys_vec3 m_b1_ap;
	phys_vec3 m_b2_ap;
	float m_pulse_sum_min;
	float m_pulse_sum_max;
	float m_pulse_sum;
	float m_right_side;
	float m_big_dirt;
	float m_cfm;
	float m_denom;
	float m_pulse_limit_ratio;
	unsigned int m_flags;
	pulse_sum_normal* m_pulse_parent;
	pulse_sum_node* m_b1;
	pulse_sum_node* m_b2;
	pulse_sum_cache* m_pulse_sum_cache;
};
