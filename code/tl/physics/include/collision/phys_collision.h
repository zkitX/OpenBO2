#pragma once
#include <rigid_body.h>
#include <rigid_body_constraint.h>

#include <collision/phys_board_phase.h>

class phys_collision_pair : phys_link_list_base<phys_collision_pair>
{
	broad_phase_info* m_bpi1;
	broad_phase_info* m_bpi2;
	float m_hit_time;
	phys_gjk_cache_info* m_gjk_ci;
};