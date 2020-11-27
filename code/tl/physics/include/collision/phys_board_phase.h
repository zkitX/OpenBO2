#pragma once
#include <rigid_body.h>
#include <rigid_body_constraint.h>

#include "phys_board_phase_base.h"
#include "phys_gjk.h"

class broad_phase_info : broad_phase_base
{
public:
	rigid_body* m_rb;
	const phys_mat44* m_rb_to_world_xform;
	const phys_mat44* m_cg_to_world_xform;
	const phys_mat44* m_cg_to_rb_xform;
	const phys_gjk_geom* m_gjk_geom;
	unsigned int m_gjk_geom_id;
	int m_surface_type;
};
