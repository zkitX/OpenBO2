#include "r_wind.h"

#include "universal/com_math.h"

void R_SetWindDirection(vec3_t* dir, float speed)
{
    float speeda; // [esp+10h] [ebp+Ch]

    frontendWind.speed = speed;
    frontendWind.windDirection.x = dir->x;
    frontendWind.windDirection.v[1] = frontendWind.windDirection.y;
    speeda = I_rsqrt(
        (frontendWind.windDirection.y * frontendWind.windDirection.y)
        + (frontendWind.windDirection.x * frontendWind.windDirection.x));
    frontendWind.windDirection.x = frontendWind.windDirection.x * speeda;
    frontendWind.windDirection.y = frontendWind.windDirection.y * speeda;
    frontendWind.windDirection.z = frontendWind.windDirection.z * speeda;
}

void R_SetVariantWindSpringAngle(unsigned int index, float angle)
{
    if (index >= 16
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(index) < (unsigned)((sizeof( frontendWind.windSpringAngle ) / (sizeof( frontendWind.windSpringAngle"
            "[0] ) * (sizeof( frontendWind.windSpringAngle ) != 4 || sizeof( frontendWind.windSpringAngle[0] ) <= 4))))",
            "index doesn't index ARRAY_COUNT(frontendWind.windSpringAngle)\n\t%i not in [0, %i)",
            index,
            16))
    {
        __debugbreak();
    }
    frontendWind.windSpringAngle[index] = angle;
}

void R_SetVariantWindRustleAngle(unsigned int windIndex, vec3_t* angles)
{
    if (windIndex >= 16
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(windIndex) < (unsigned)((sizeof( frontendWind.windRustleAngle ) / (sizeof( frontendWind.windRustleA"
            "ngle[0] ) * (sizeof( frontendWind.windRustleAngle ) != 4 || sizeof( frontendWind.windRustleAngle[0] ) <= 4))))",
            "windIndex doesn't index ARRAY_COUNT(frontendWind.windRustleAngle)\n\t%i not in [0, %i)",
            windIndex,
            16))
    {
        __debugbreak();
    }
    frontendWind.windRustleAngle[windIndex] = *angles;
}

void R_SetWindShaderConstants(GfxCmdBufInput* input)
{
}

void R_CopyWindStateToBackEnd()
{
    qmemcpy(&frontEndDataOut->gfxWindState, &frontendWind, sizeof(frontEndDataOut->gfxWindState));
}
