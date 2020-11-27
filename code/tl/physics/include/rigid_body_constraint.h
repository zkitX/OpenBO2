#pragma once
#include <rigid_body.h>
#include <phys_avl_tree.h>

#include <constraint_solver/pulse_sum_normal.h>

#include <collision/phys_collision.h>

/*
=============
rigid_body_constraint
=============
*/
struct rigid_body_constraint
{
	rigid_body* b1;
	rigid_body* b2;
	rigid_body_constraint* m_next;
};

/*
=============
rigid_body_constraint_point
=============
*/
class rigid_body_constraint_point : rigid_body_constraint
{
public:
	phys_vec3 m_b1_r_loc;
	phys_vec3 m_b2_r_loc;
	pulse_sum_cache m_ps_cache_list[3];
	float m_stress;
	float m_spring_k;
	float m_damp_k;
	bool m_spring_enabled;
};

/*
=============
rigid_body_constraint_hinge
=============
*/
class rigid_body_constraint_hinge : rigid_body_constraint
{
public:
	phys_vec3 m_b1_r_loc;
	phys_vec3 m_b2_r_loc;
	phys_vec3 m_b1_axis_loc;
	phys_vec3 m_b2_axis_loc;
	phys_vec3 m_b1_a1_loc;
	phys_vec3 m_b1_a2_loc;
	phys_vec3 m_b1_ref_loc;
	phys_vec3 m_b2_ref_min_loc;
	phys_vec3 m_b2_ref_max_loc;
	float m_damp_k;
	unsigned int m_flags;
	pulse_sum_cache m_ps_cache[8];
};
/*
=============
rigid_body_constraint_distance
=============
*/
class rigid_body_constraint_distance : rigid_body_constraint
{
	phys_vec3 m_b1_r_loc;
	phys_vec3 m_b2_r_loc;
	float m_min_distance;
	float m_max_distance;
	float m_next_max_distance;
	float m_max_distance_vel;
	float m_damp_coef;
	unsigned int m_flags;
	pulse_sum_cache m_ps_cache_list[3];
};

class rigid_body_constraint_ragdoll : rigid_body_constraint
{
	phys_vec3 m_b1_r_loc;
	phys_vec3 m_b2_r_loc;
	unsigned int m_flags;
	pulse_sum_cache m_ps_cache_list[10];
	phys_vec3 m_b1_axis_loc;
	phys_vec3 m_b2_axis_loc;
	phys_vec3 m_b1_a1_loc;
	phys_vec3 m_b1_a2_loc;
	phys_vec3 m_b1_ref_loc;
	phys_vec3 m_b2_ref_min_loc;
	phys_vec3 m_b2_ref_max_loc;
	ragdoll_joint_limit_info m_joint_limits[2];
	int m_joint_limits_count;
	float m_damp_k;
};

class rigid_body_constraint_wheel : rigid_body_constraint
{
	phys_vec3 m_b2_hitp_loc;
	phys_vec3 m_b2_hitn_loc;
	phys_vec3 m_b1_wheel_center_loc;
	phys_vec3 m_b1_suspension_dir_loc;
	phys_vec3 m_b1_wheel_axis_loc;
	float m_wheel_radius;
	float m_fwd_fric_k;
	float m_side_fric_k;
	float m_side_fric_max;
	float m_suspension_stiffness_k;
	float m_suspension_damp_k;
	float m_hard_limit_dist;
	float m_roll_stability_factor;
	float m_pitch_stability_factor;
	float m_turning_radius_ratio_max_speed;
	float m_turning_radius_ratio_accel;
	float m_desired_speed_k;
	float m_acceleration_factor_k;
	float m_braking_factor_k;
	float m_wheel_vel;
	float m_wheel_fwd;
	float m_wheel_pos;
	float m_wheel_displaced_center_dist;
	float m_wheel_normal_force;
	unsigned int m_wheel_state;
	unsigned int m_wheel_flags;
	pulse_sum_cache m_ps_cache_list[4];
	pulse_sum_normal* m_ps_suspension;
	pulse_sum_normal* m_ps_side_fric;
	pulse_sum_normal* m_ps_fwd_fric;
};

class rigid_body_constraint_angular_actuator : rigid_body_constraint
{
	phys_mat44 m_target_mat;
	phys_mat44 m_next_target_mat;
	phys_vec3 m_a_vel;
	float m_power;
	float m_power_scale;
	bool m_enabled;
	pulse_sum_cache m_ps_cache_list[3];
};

class rigid_body_constraint_upright : rigid_body_constraint
{
	phys_vec3 m_b1_forward_axis_loc;
	phys_vec3 m_b1_right_axis_loc;
	phys_vec3 m_b1_up_axis_loc;
	phys_vec3 m_b1_lean_axis_loc;
	phys_vec3 m_b2_up_axis_loc;
	phys_vec3 m_last_t_vel;
	phys_vec3 m_last_a_vel;
	float m_avg_side_force;
	float m_avg_normal_force;
	float m_lean_angle_calc_delta_t;
	float m_lean_angle;
	float m_lean_angle_multiplier;
	float m_max_lean_angle;
	float m_moving_average_total_time;
	bool m_enabled;
	pulse_sum_cache m_ps_cache_list[1];
};

class rigid_body_constraint_custom_orientation : rigid_body_constraint
{
	pulse_sum_cache m_ps_cache_list[5];
	bool m_active;
	bool m_no_orientation_correction;
	float m_torque_resistance_pitch_roll;
	float m_torque_resistance_yaw;
	float m_torque_resistance_yaw_max_collision_speed;
	float m_upright_strength;
};

class rigid_body_constraint_custom_path : rigid_body_constraint
{
	phys_mat44 m_path_mat;
	phys_vec3 b1_r_loc;
	user_rigid_body* m_urb;
	int m_timestamp;
	float m_spring_scale;
	pulse_sum_cache m_list_psc[4];
};

class rigid_body_constraint_contact : rigid_body_constraint
{
	phys_simple_link_list<contact_point_info> m_list_contact_point_info_buffer_1;
	phys_simple_link_list<contact_point_info> m_list_contact_point_info_buffer_2;
	unsigned int m_solver_priority;
	phys_inplace_avl_tree_node<rigid_body_constraint_contact> m_avl_tree_node;
	rigid_body_pair_key m_avl_key;
};
