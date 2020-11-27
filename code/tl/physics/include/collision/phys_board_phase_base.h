#pragma once
#include <rigid_body.h>
#include <rigid_body_constraint.h>

class broad_phase_base
{
public:
	phys_vec3 m_trace_aabb_min_whace;
	phys_vec3 m_trace_aabb_max_whace;
	phys_vec3 m_trace_translation;
	unsigned int m_flags;
	broad_phase_base* m_list_bpb_next;
	broad_phase_base* m_list_bpb_cluster_next;
	void* m_sap_node;
	void* m_user_data;
	unsigned int m_env_collision_flags;
	unsigned int m_my_collision_type_flags;
};