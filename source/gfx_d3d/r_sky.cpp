#include "r_sky.h"

#include <universal/com_math_anglevectors.h>

void R_RegisterSunDvars()
{
    r_sunsprite_shader = _Dvar_RegisterString(
        "r_sunsprite_shader",
        "sun",
        0,
        "name for static sprite; can be any material");
    r_sunsprite_size = _Dvar_RegisterFloat(
        "r_sunsprite_size",
        16.0,
        1.0,
        1000.0,
        0,
        "diameter in pixels at 640x480 and 80 fov");
    r_sunflare_shader = _Dvar_RegisterString(
        "r_sunflare_shader",
        "sun_flare",
        0,
        "name for flare effect; can be any material");
    r_sunflare_min_size = _Dvar_RegisterFloat(
        "r_sunflare_min_size",
        0.0,
        0.0,
        10000.0,
        0,
        "smallest size of flare effect in pixels at 640x480");
    r_sunflare_min_angle = _Dvar_RegisterFloat(
        "r_sunflare_min_angle",
        45.0,
        0.0,
        90.0,
        0,
        "angle from sun in degrees outside which effect is 0");
    r_sunflare_max_size = _Dvar_RegisterFloat(
        "r_sunflare_max_size",
        2500.0,
        0.0,
        10000.0,
        0,
        "largest size of flare effect in pixels at 640x480");
    r_sunflare_max_angle = _Dvar_RegisterFloat(
        "r_sunflare_max_angle",
        2.0,
        0.0,
        90.0,
        0,
        "angle from sun in degrees inside which effect is max");
    r_sunflare_max_alpha = _Dvar_RegisterFloat(
        "r_sunflare_max_alpha",
        1.0,
        0.0,
        1.0,
        0,
        "0-1 vertex color and alpha of sun at max effect");
    r_sunflare_fadein = _Dvar_RegisterFloat(
        "r_sunflare_fadein",
        1.0,
        0.0,
        60.0,
        0,
        "time in seconds to fade alpha from 0% to 100%");
    r_sunflare_fadeout = _Dvar_RegisterFloat(
        "r_sunflare_fadeout",
        1.0,
        0.0,
        60.0,
        0,
        "time in seconds to fade alpha from 100% to 0%");
    r_sunblind_min_angle = _Dvar_RegisterFloat(
        "r_sunblind_min_angle",
        30.0,
        0.0,
        90.0,
        0,
        "angle from sun in degrees outside which blinding is 0");
    r_sunblind_max_angle = _Dvar_RegisterFloat(
        "r_sunblind_max_angle",
        5.0,
        0.0,
        90.0,
        0,
        "angle from sun in degrees inside which blinding is max");
    r_sunblind_max_darken = _Dvar_RegisterFloat(
        "r_sunblind_max_darken",
        0.75,
        0.0,
        1.0,
        0,
        "0-1 fraction for how black the world is at max blind");
    r_sunblind_fadein = _Dvar_RegisterFloat(
        "r_sunblind_fadein",
        0.5,
        0.0,
        60.0,
        0,
        "time in seconds to fade blind from 0% to 100%");
    r_sunblind_fadeout = _Dvar_RegisterFloat(
        "r_sunblind_fadeout",
        3.0,
        0.0,
        60.0,
        0,
        "time in seconds to fade blind from 100% to 0%");
    r_sunglare_min_angle = _Dvar_RegisterFloat(
        "r_sunglare_min_angle",
        30.0,
        0.0,
        90.0,
        0,
        "angle from sun in degrees inside which glare is maximum");
    r_sunglare_max_angle = _Dvar_RegisterFloat(
        "r_sunglare_max_angle",
        5.0,
        0.0,
        90.0,
        0,
        "angle from sun in degrees inside which glare is minimum");
    r_sunglare_max_lighten = _Dvar_RegisterFloat(
        "r_sunglare_max_lighten",
        0.75,
        0.0,
        1.0,
        0,
        "0-1 fraction for how white the world is at max glare");
    r_sunglare_fadein = _Dvar_RegisterFloat(
        "r_sunglare_fadein",
        0.5,
        0.0,
        60.0,
        0,
        "time in seconds to fade glare from 0% to 100%");
    r_sunglare_fadeout = _Dvar_RegisterFloat(
        "r_sunglare_fadeout",
        3.0,
        0.0,
        60.0,
        0,
        "time in seconds to fade glare from 100% to 0%");
    r_sun_fx_position = _Dvar_RegisterVec3(
        "r_sun_fx_position",
        0.0,
        0.0,
        0.0,
        -360.0,
        360.0,
        0,
        "Position in degrees of the sun effect");
}

void R_SetSunFromDvars(sunflare_t* sun)
{
    char* dvarString;
    float dvarFloat;
    vec3_t sun_fx_position;

    if (!sun
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(sun)", nullptr))
    {
        __debugbreak();
    }
    dvarString = Dvar_GetString(r_sunsprite_shader);
    sun->spriteMaterial = Material_RegisterHandle(dvarString, 6, 1, -1);
    dvarFloat = Dvar_GetFloat(r_sunsprite_size);
    sun->spriteSize = dvarFloat;
    dvarString = Dvar_GetString(r_sunflare_shader);
    sun->flareMaterial = Material_RegisterHandle(dvarString, 6, 1, -1);
    dvarFloat = Dvar_GetFloat(r_sunflare_min_size);
    dvarFloat = dvarFloat * 0.5;
    sun->flareMinSize = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunflare_min_angle);
    dvarFloat = cos(dvarFloat * 0.017453292);
    sun->flareMinDot = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunflare_max_size);
    dvarFloat = dvarFloat * 0.5;
    sun->flareMaxSize = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunflare_max_angle);
    dvarFloat = cos(dvarFloat * 0.017453292);
    sun->flareMaxDot = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunflare_max_alpha);
    sun->flareMaxAlpha = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunflare_fadein);
    dvarFloat = dvarFloat * 1000.0;
    dvarFloat = dvarFloat + 9.313225746154785e-10;
    sun->flareFadeInTime = (int)dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunflare_fadeout);
    dvarFloat = dvarFloat * 1000.0;
    dvarFloat = dvarFloat + 9.313225746154785e-10;
    sun->flareFadeOutTime = (int)dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunblind_min_angle);
    dvarFloat = cos(dvarFloat * 0.017453292);
    sun->blindMinDot = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunblind_max_angle);
    sun->blindMaxDot = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunblind_max_darken);
    sun->blindMaxDarken = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunblind_fadein);
    dvarFloat = dvarFloat * 1000.0;
    dvarFloat = dvarFloat + 9.313225746154785e-10;
    sun->blindFadeInTime = (int)dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunblind_fadeout);
    dvarFloat = dvarFloat * 1000.0;
    dvarFloat = dvarFloat + 9.313225746154785e-10;
    sun->blindFadeOutTime = (int)dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunglare_min_angle);
    dvarFloat = cos(dvarFloat * 0.017453292);
    sun->glareMinDot = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunglare_max_angle);
    sun->glareMaxDot = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunglare_max_lighten);
    sun->glareMaxLighten = dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunglare_fadein);
    dvarFloat = dvarFloat * 1000.0;
    dvarFloat = dvarFloat + 9.313225746154785e-10;
    sun->glareFadeInTime = (int)dvarFloat;
    dvarFloat = Dvar_GetFloat(r_sunglare_fadeout);
    dvarFloat = dvarFloat * 1000.0;
    sun->glareFadeOutTime = (int)(dvarFloat + 9.313225746154785e-10);
    Dvar_GetVec3(r_sun_fx_position, &sun_fx_position);
    AngleVectors(&sun_fx_position, &sun->sunFxPosition, 0, 0);
    sun->hasValidData = 1;
}

void R_LoadSunThroughDvars(const char* sunName, sunflare_t* sun)
{
    const char* m_sunName;
    sunflare_t* sunflare;
    const char* sunFileName;

    m_sunName = sunName;
    if (!sunName
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(sunName)", nullptr))
    {
        __debugbreak();
    }
    sunflare = sun;
    if (!sun
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(sun)", nullptr))
    {
        __debugbreak();
    }
    sunFileName = va("sun/%s.sun", m_sunName);
    if (FS_ReadFile(sunFileName, (void**)&sunName) >= 0)
    {
        if (Com_LoadDvarsFromBuffer(sunDvarNames, 0x15u, sunName, sunFileName))
            R_SetSunFromDvars(sunflare);
        FS_FreeFile((void*)sunName);
    }
    else
    {
        Com_Printf((int)sunFileName, 13, "WARNING: couldn't load sun file '%s'\n", sunFileName);
    }
}

void R_FlushSun()
{
    sunFlareArray[0].currentBlind = 0.0;
    sunFlareArray[0].currentGlare = 0.0;
    sunFlareArray[0].flareIntensity = 0.0;
    sunFlareArray[0].lastDot = 0.0;
    sunFlareArray[0].lastTime = 0;
    sunFlareArray[0].lastVisibility = 0.0;
    sunFlareArray[1].currentBlind = 0.0;
    sunFlareArray[1].currentGlare = 0.0;
    sunFlareArray[1].flareIntensity = 0.0;
    sunFlareArray[1].lastDot = 0.0;
    sunFlareArray[1].lastTime = 0;
    sunFlareArray[1].lastVisibility = 0.0;
    sunFlareArray[2].currentBlind = 0.0;
    sunFlareArray[2].currentGlare = 0.0;
    sunFlareArray[2].flareIntensity = 0.0;
    sunFlareArray[2].lastDot = 0.0;
    sunFlareArray[2].lastTime = 0;
    sunFlareArray[2].lastVisibility = 0.0;
    sunFlareArray[3].currentBlind = 0.0;
    sunFlareArray[3].currentGlare = 0.0;
    sunFlareArray[3].flareIntensity = 0.0;
    sunFlareArray[3].lastDot = 0.0;
    sunFlareArray[3].lastTime = 0;
    sunFlareArray[3].lastVisibility = 0.0;
}
