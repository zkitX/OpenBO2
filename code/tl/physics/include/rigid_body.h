#pragma once
#include <rigid_body_constraint.h>
#include <constraint_solver/pulse_sum_normal.h>

struct rigid_body;

template <typename T>
struct phys_link_list_base {
public:
	T* m_next_link;
};

template <typename T>
class phys_simple_link_list {
public:
	T* m_first;
};

class phys_vec2 {
public:
	float x;
	float y;
};

class phys_vec3 {
public:
	float x;
	float y;
	float z;
	float w;
};

class phys_mat44
{
public:
	phys_vec3 x;
	phys_vec3 y;
	phys_vec3 z;
	phys_vec3 w;
};

class ragdoll_joint_limit_info
{
public:
	phys_vec3 m_b1_ud_loc;
	float m_b1_ud_limit_co_;
	float m_b1_ud_limit_si_;
	float m_b1_ud_active_limit_co_;
};

struct rb_inplace_partition_node
{
	rigid_body_constraint_point* m_rbc_point_first;
	rigid_body_constraint_hinge* m_rbc_hinge_first;
	rigid_body_constraint_distance* m_rbc_dist_first;
	rigid_body_constraint_ragdoll* m_rbc_ragdoll_first;
	rigid_body_constraint_wheel* m_rbc_wheel_first;
	rigid_body_constraint_angular_actuator* m_rbc_angular_actuator_first;
	rigid_body_constraint_upright* m_rbc_upright_first;
	rigid_body_constraint_custom_orientation* m_rbc_custom_orientation_first;
	rigid_body_constraint_custom_path* m_rbc_custom_path_first;
	rigid_body_constraint_contact* m_rbc_contact_first;
	rigid_body* m_partition_head;
	rigid_body* m_partition_tail;
	rigid_body* m_next_node;
	int m_partition_size;
};

class rigid_body
{
	phys_vec3 m_last_position;
	phys_vec3 m_moved_vec;
	float m_smallest_lambda;
	phys_mat44 m_mat;
	phys_vec3 m_inv_inertia;
	phys_vec3 m_gravity_acc_vec;
	phys_vec3 m_t_vel;
	phys_vec3 m_a_vel;
	phys_vec3 m_last_t_vel;
	phys_vec3 m_last_a_vel;
	phys_vec3 m_force_sum;
	phys_vec3 m_torque_sum;
	float m_inv_mass;
	float m_max_avel;
	float m_max_delta_t;
	unsigned int m_flags;
	unsigned int m_tick;
	pulse_sum_node* m_node;
	int m_constraint_count;
	int m_contact_count;
	int m_stable_min_contact_count;
	float m_stable_energy_time;
	float m_largest_vel_sq;
	float m_t_drag_coef;
	float m_a_drag_coef;
	void* m_userdata;
	rb_inplace_partition_node m_partition_node;
};

class user_rigid_body : rigid_body
{
	const phys_mat44* m_dictator;
	phys_mat44 m_dictator_mat;
};

class rigid_body_pair_key
{
	rigid_body* m_b1;
	rigid_body* m_b2;
};