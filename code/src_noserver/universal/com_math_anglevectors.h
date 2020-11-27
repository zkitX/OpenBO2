#pragma once

#include <universal/com_math.h>

void AngleVectors(const vec3_t* angles, vec3_t* forward, vec3_t* right, vec3_t* up);
void AnglesToAxis(const vec3_t* angles, vec3_t* axis);
void AnglesToQuat(const vec3_t* angles, vec4_t* quat);
void QuatToAxis(const vec4_t* quat, vec3_t* axis);
void vectosignedangles(const vec3_t* vec, vec3_t* angles);
void AxisToSignedAngles(const vec3_t* axis, vec3_t* angles);