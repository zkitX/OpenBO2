#include "com_math_anglevectors.h"

#include <universal/assertive.h>

void AngleVectors(const vec3_t* angles, vec3_t* forward, vec3_t* right, vec3_t* up)
{
    const vec3_t* v4; // esi
    float v5; // xmm1_4
    float angle; // [esp+8h] [ebp-18h]
    float anglea; // [esp+8h] [ebp-18h]
    float sr; // [esp+Ch] [ebp-14h]
    float cr; // [esp+10h] [ebp-10h]
    float cp; // [esp+14h] [ebp-Ch]
    float sy; // [esp+18h] [ebp-8h]
    float cy; // [esp+1Ch] [ebp-4h]
    float anglesa; // [esp+28h] [ebp+8h]

    v4 = angles;
    v5 = angles->y * 0.017453292;
    cy = cos(v5);
    sy = sinf(v5);
    angle = angles->x * 0.017453292;
    cp = cosf(angle);
    anglesa = sinf(angle);
    if (forward)
    {
        forward->y = cp * sy;
        forward->x = cp * cy;
        forward->z = -anglesa;
    }
    if (right || up)
    {
        anglea = v4->z * 0.017453292;
        cr = cosf(anglea);
        sr = sinf(anglea);
        if (right)
        {
            right->x = (cr * sy) - ((sr * anglesa) * cy);
            right->y = ((cr * -1.0) * cy) - ((sr * anglesa) * sy);
            right->z = (sr * -1.0) * cp;
        }
        if (up)
        {
            up->x = ((cr * anglesa) * cy) + (sr * sy);
            up->y = ((cr * anglesa) * sy) - (sr * cy);
            up->z = cr * cp;
        }
    }
}

void AnglesToAxis(const vec3_t* angles, vec3_t* axis)
{
    const vec3_t* v2; // esi
    float angle; // [esp+8h] [ebp-18h]
    float anglea; // [esp+8h] [ebp-18h]
    float angleb; // [esp+8h] [ebp-18h]
    float cr; // [esp+Ch] [ebp-14h]
    float sr; // [esp+10h] [ebp-10h]
    float cp; // [esp+14h] [ebp-Ch]
    float sy; // [esp+18h] [ebp-8h]
    float cy; // [esp+1Ch] [ebp-4h]
    float anglesa; // [esp+28h] [ebp+8h]

    v2 = angles;
    angle = angles->y * 0.017453292;
    cy = cosf(angle);
    sy = sinf(angle);
    anglea = angles->x * 0.017453292;
    cp = cosf(anglea);
    anglesa = sin(anglea);
    axis->y = cp * sy;
    axis->x = cp * cy;
    axis->z = -anglesa;
    angleb = v2->z * 0.017453292;
    cr = cosf(angleb);
    sr = sinf(angleb);
    axis[1].x = ((sr * anglesa) * cy) - (cr * sy);
    axis[1].y = ((sr * anglesa) * sy) + (cr * cy);
    axis[1].z = sr * cp;
    axis[2].x = ((cr * anglesa) * cy) + (sr * sy);
    axis[2].y = ((cr * anglesa) * sy) - (sr * cy);
    axis[2].z = cr * cp;
}

void AnglesToQuat(const vec3_t* angles, vec4_t* quat)
{
    vec3_t axis[3]; // [esp+4h] [ebp-28h]

    AnglesToAxis(angles, axis);
    AxisToQuat(axis, quat);
}

void QuatToAxis(const vec4_t* quat, vec3_t* axis)
{
    float v2; // xmm2_4
    float v3; // xmm0_4
    float v4; // xmm5_4
    float v5; // xmm1_4
    float v6; // xmm6_4
    float v7; // xmm3_4
    float v8; // xmm4_4
    float v9; // xmm7_4
    float v10; // xmm2_4
    float v11; // xmm5_4
    float v12; // xmm6_4
    float v13; // xmm7_4
    float xz; // [esp+0h] [ebp-10h]
    float zw; // [esp+4h] [ebp-Ch]
    float xx; // [esp+8h] [ebp-8h]
    float xw; // [esp+Ch] [ebp-4h]

    v2 = quat->v[1];
    v3 = quat->v[2];
    v4 = quat->v[0];
    v5 = 2.0 / ((((v2 * v2) + (v4 * v4)) + (v3 * v3)) + (quat->v[3] * quat->v[3]));
    xx = v5 * (v4 * v4);
    v6 = quat->v[0] * v5;
    v7 = v5 * (v2 * v2);
    v8 = v5 * (v3 * v3);
    xz = v3 * v6;
    v9 = quat->v[3] * v6;
    v10 = v2 * v5;
    v11 = quat->v[1] * v6;
    v12 = quat->v[2] * v10;
    xw = v9;
    v13 = quat->v[3] * v10;
    zw = (v3 * v5) * quat->v[3];
    axis->x = 1.0 - (v8 + v7);
    axis->y = zw + v11;
    axis->z = xz - v13;
    axis[1].x = v11 - zw;
    axis[1].y = 1.0 - (v8 + xx);
    axis[1].z = v12 + xw;
    axis[2].x = v13 + xz;
    axis[2].y = v12 - xw;
    axis[2].z = 1.0 - (v7 + xx);
}

void vectosignedangles(const vec3_t* vec, vec3_t* angles)
{
    float _Y; // xmm1_4
    float v3; // st7
    float _X; // [esp+4h] [ebp-10h]
    float yaw; // [esp+Ch] [ebp-8h]
    float pitch; // [esp+10h] [ebp-4h]

    _Y = vec->y;
    if (_Y != 0.0 || vec->x != 0.0)
    {
        yaw = atan2f(_Y, vec->x) * 180.0 * 0.31830987;
        _X = sqrtf((vec->x * vec->x) + (vec->y * vec->y));
        v3 = atan2f(vec->z, _X);
        angles->v[1] = yaw;
        pitch = v3 * -180.0 * 0.31830987;
        angles->x = pitch;
    }
    else
    {
        if (-vec->z < 0.0)
            angles->x = -90.0;
        else
            angles->x = 90.0;
        angles->v[1] = 0;
    }
}

void AxisToSignedAngles(const vec3_t* axis, vec3_t* angles)
{
    vec3_t* v2; // edi
    const vec3_t* v3; // esi
    int v4; // xmm4_4
    float v5; // xmm1_4
    float v6; // xmm3_4
    float v7; // xmm2_4
    float v8; // xmm7_4
    float v9; // xmm0_4
    float v10; // xmm1_4
    float v11; // xmm3_4
    float v12; // xmm0_4
    float v13; // xmm3_4
    float v14; // xmm2_4
    float v15; // xmm3_4
    float v16; // xmm2_4
    double v17; // st7
    float _X; // [esp+4h] [ebp-30h]
    float fCos; // [esp+14h] [ebp-20h]
    float fSin; // [esp+18h] [ebp-1Ch]
    float rad; // [esp+1Ch] [ebp-18h]
    float v22; // [esp+20h] [ebp-14h]
    float v23; // [esp+24h] [ebp-10h]
    vec3_t right; // [esp+28h] [ebp-Ch]

    v2 = angles;
    v3 = axis;
    vectosignedangles(axis, angles);
    v5 = v3[1].x;
    v6 = v3[1].y;
    v7 = v3[1].z;
    rad = -v2->y * 0.017453292;
    v23 = fCos;
    v22 = fSin;
    fCos = cosf(rad);
    fSin = sinf(rad);
    v8 = v6;
    v9 = v5;
    v10 = (v5 * fSin) + (v6 * fCos);
    v11 = -v2->x * 0.017453292;
    v12 = (v9 * fCos) - (v8 * fSin);
    right.y = v10;
    rad = v11;
    v22 = fCos;
    v23 = fSin;
    fCos = cosf(v11);
    fSin = sinf(v11);
    v13 = v7;
    v14 = (v7 * fCos) - (v12 * fSin);
    v15 = (v13 * fSin) + (v12 * fCos);
    right.z = v14;
    if (v10 != 0.0 || v15 != 0.0)
    {
        _X = sqrtf((v15 * v15) + (v10 * v10));
        v17 = atan2f(right.z, _X);
        v10 = right.y;
        v23 = v17 * -180.0 * 0.31830987;
        v16 = v23;
    }
    else if ((-v14) < 0.0)
    {
        v16 = -90.0;
    }
    else
    {
        v16 = 90.0;
    }
    if (v10 >= 0.0)
    {
        v2->z = -v16;
    }
    else if (v16 >= 0.0)
    {
        v2->z = v16 + -180.0;
    }
    else
    {
        v2->z = v16 + 180.0;
    }
}
