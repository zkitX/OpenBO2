#pragma once
#include <universal/assertive.h>
#include <universal/com_math.h>

inline void Vec3Cross(const vec3_t* v0, const vec3_t* v1, vec3_t* cross) {
    if (v0 == cross
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(&v0 != &cross)",
            "%s",
            "&v0 != &cross"))
    {
        __debugbreak();
    }
    if (v1 == cross
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(&v1 != &cross)",
            "%s",
            "&v1 != &cross"))
    {
        __debugbreak();
    }
    cross->x = (v1->z * v0->y) - (v0->z * v1->y);
    cross->y = (v0->z * v1->x) - (v0->x * v1->z);
    cross->z = (v0->x * v1->y) - (v1->x * v0->y);
}