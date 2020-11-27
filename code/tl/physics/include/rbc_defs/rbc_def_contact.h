#pragma once
#include <rigid_body.h>
#include <rigid_body_constraint.h>
#include <phys_transient_allocator.h>

#include <collision/phys_board_phase.h>

class contact_point_info
{
public:
	class pulse_sum_cache_info
	{
		pulse_sum_cache m_ps_cache_list[3];
	};
	phys_vec3 m_normal;
	float m_fric_coef;
	float m_bounce_coef;
	float m_max_restitution_vel;
	int m_flags;
	int m_point_pair_count;
	phys_vec3* m_list_b1_r_loc;
	phys_vec3* m_list_b2_r_loc;
	contact_point_info::pulse_sum_cache_info* m_list_pulse_sum_cache_info;
	contact_point_info* m_next_link;
	const void* m_rb2_entity;
	float m_translation_lambda;
	phys_collision_pair* m_pcp;
	rigid_body_constraint_contact* m_rbc_contact;

	void check_surface_properties();
	phys_transient_allocator* get_cpi_allocator();
};
