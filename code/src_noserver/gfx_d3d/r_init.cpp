#include "r_init.h"
#include "r_legacy.h"
#include "r_dvars.h"

#include <qcommon/threads.h>
#include <universal/assertive.h>
#include <universal/dvar.h>
#include <win32/win_main.h>

bool R_Is3DOn()
{
    if (Dvar_GetBool(r_loadForRenderer))
        return true;
    else
        return false;
}

const char* R_ErrorDescription(int hr)
{
    return DXGetErrorDescriptionA(hr);
}

bool R_IsNullRenderer()
{
    return false;
}

char* R_GetFontPathFromName(const char* fontName)
{
    int fontRes;

    if (!fontName
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(fontName)", nullptr))
    {
        __debugbreak();
    }
    if (!*fontName
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(fontName[0])",
            nullptr))
    {
        __debugbreak();
    }
    fontRes = Dvar_GetInt(r_fontResolution);
    return va("fonts/%d/%s", fontRes, fontName);
}

void R_SetBrightness(float brightness)
{
    if (rg.gamma != brightness) {
        Dvar_SetFloat(r_brightness, brightness);
        rg.gamma = brightness;
        rg.gammaModified = 1;
    }
}

void R_SetColorMappings()
{
    GfxGammaRamp gammaRamp;

    if (vidConfig.deviceSupportsGamma)
        RB_SetGammaRamp(&gammaRamp);
}

void R_GammaCorrect(char* buffer, int bufSize)
{
    int i;
    int bufferCheck;
    GfxGammaRamp gammaRamp;

    if (!buffer
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(buffer)", nullptr))
    {
        __debugbreak();
    }
    if (bufSize <= 0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((bufSize > 0))",
            "(bufSize) = %i",
            bufSize))
    {
        __debugbreak();
    }
    for (i = 0; i < bufSize; ++i)
    {
        bufferCheck = (unsigned __int8)buffer[i];
        if (bufferCheck > 255
            && !Assert_MyHandler(
                __FILE__,
                __LINE__,
                1,
                "((inValue <= 255))",
                "(inValue) = %i",
                bufferCheck))
        {
            __debugbreak();
        }
        buffer[i] = 255 * gammaRamp.entries[bufferCheck] / 0xFFFF;
    }
}

void SetGfxConfig(const GfxConfiguration* config)
{
    unsigned int m_maxClientViews;
    int m_critSectCount;
    const char* vaError;

    if (!config
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(config)", nullptr))
    {
        __debugbreak();
    }
    m_maxClientViews = config->maxClientViews;
    if ((config->maxClientViews < 1 || m_maxClientViews > 4)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(1) <= (config->maxClientViews) && (config->maxClientViews) <= (4)",
            "config->maxClientViews not in [1, GFX_MAX_CLIENT_VIEWS]\n\t%i not in [%i, %i]",
            m_maxClientViews,
            1,
            4))
    {
        __debugbreak();
    }
    m_critSectCount = config->critSectCount;
    if (m_critSectCount != 77)
    {
        vaError = va("%d != %d", m_critSectCount, 77);
        if (!Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(config->critSectCount == CRITSECT_COUNT)",
            "%s",
            vaError))
            __debugbreak();
    }
    gfxCfg = *config;
}

void R_MakeDedicated(const GfxConfiguration* config)
{
    SetGfxConfig(config);
    if (!r_loadForRenderer)
        R_RegisterDvars();
    Dvar_SetBool(r_loadForRenderer, 0);
    Dvar_MakeLatchedValueCurrent(r_loadForRenderer);
    DB_LoadGraphicsAssetsForPC();
}

void R_SetVidRestart()
{
    if ((Dvar_HasLatchedValue(r_mode)
        || Dvar_HasLatchedValue(r_fullscreen)
        || Dvar_HasLatchedValue(r_aaSamples)
        || Dvar_HasLatchedValue(r_monitor))
        && !R_Is3DOn())
    {
        dx.vidRestart = 1;
    }
}

void R_AllocateMinimalResources()
{
    g_allocateMinimalResources = true;
}

void R_SetIsMultiplayer(bool isMp)
{
    r_glob.isMultiplayer = isMp;
}

void R_SetIsZombie(bool isZM)
{
    r_glob.isZombie = isZM;
}

void R_ShutdownStreams()
{
    if (dx.device)
        R_ClearAllStreamSources(&gfxCmdBufState.prim);
}

void R_Shutdown(int destroyWindow)
{
    if (Sys_QueryD3DShutdownEvent()
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(Sys_QueryD3DShutdownEvent() == qfalse)",
            nullptr))
    {
        __debugbreak();
    }
    g_destroy_window = destroyWindow;
    Sys_SetD3DShutdownEvent();
    while (Sys_QueryD3DShutdownEvent())
        NET_Sleep(0);
}

void R_UnloadWorld()
{
    if (!Dvar_GetBool(useFastFile)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(IsFastFileLoad())",
            nullptr))
    {
        __debugbreak();
    }
    if (rgp.world)
        Sys_Error("Cannot unload bsp while it is in use");
    rgp.postMapFastfileLoadFinished = 0;
}

void R_BeginRegistration(vidConfig_t* vidConfigOut, GfxViewParms* a2)
{
    Sys_SetRGRegisteredEvent();
    while (Sys_QueryRGRegisteredEvent())
    {
        Sys_CheckQuitRequest();
        NET_Sleep(0);
    }
    if (!vidConfigOut
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(vidConfigOut)",
            nullptr))
    {
        __debugbreak();
    }
    qmemcpy(vidConfigOut, &vidConfig, sizeof(vidConfig_t));
    if (r_glob.startedRenderThread
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(!r_glob.startedRenderThread)",
            nullptr))
    {
        __debugbreak();
    }
    r_glob.startedRenderThread = 1;
}

void R_EndRegistration(void)
{
    if (!rg.registered
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(rg.registered)",
            nullptr))
    {
        __debugbreak();
    }
    if (!Dvar_GetBool(useFastFile))
    {
        R_SyncRenderThread();
        RB_TouchAllImages();
    }
}

void R_TrackStatistics(trStatistics_t* stats)
{
    rg.globalstats = stats;
}

void R_ConfigureRenderer(const GfxConfiguration* config)
{
    SetGfxConfig(config);
    R_InitRenderCommands();
}

bool R_StereoActivated()
{
    return dx.nvStereoActivated;
}
