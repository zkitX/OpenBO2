#include "r_warn.h"

#include <win32/win_shared.h>

float R_UpdateFrameRate()
{
    float m_framerate;
    int ms;
    bool sameFrame;
    int msSince;

    if (frameCount != rg.frontEndFrameCount)
    {
        if (!frameCount)
        {
            previous = Sys_Milliseconds();
            frameCount = rg.frontEndFrameCount;
            return;
        }
        if (frameCount + 1 != rg.frontEndFrameCount) {
            m_framerate = 0.0;
            frameRate = m_framerate;
            frameCount = rg.frontEndFrameCount;
            return;
        }

        ms = Sys_Milliseconds();
        msSince = ms - previous;
        sameFrame = ms == previous;
        previous = ms;
        if (sameFrame)
        {
            msSince = 1;
        }
        else if (msSince < 0)
        {
            m_framerate = 0.0;
            frameRate = m_framerate;
            frameCount = rg.frontEndFrameCount;
            return;
        }
        m_framerate = 1000.0 / (float)msSince;
        frameRate = m_framerate;
        frameCount = rg.frontEndFrameCount;
        return;
    }
}

void R_WarnOncePerFrame(GfxWarningType warnType, ...)
{
    const char* warnMsg;
    char message[1024];
    va_list ap;

    va_start(ap, warnType);
    if (!r_warningRepeatDelay
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(r_warningRepeatDelay)",
            nullptr))
    {
        __debugbreak();
    }
    R_UpdateFrameRate();
    if (s_warnCount[warnType] < rg.frontEndFrameCount)
    {
        Dvar_GetFloat(r_warningRepeatDelay);
        warnMsg = s_warnFormat[warnType];
        s_warnCount[warnType] = rg.frontEndFrameCount + (frameCount * frameRate);
        _vsnprintf(message, 0x400u, warnMsg, ap);
        Com_PrintWarning(13, "%s", message);
    }
}

void R_WarnInitDvars()
{
    r_warningRepeatDelay = _Dvar_RegisterFloat(
        "r_warningRepeatDelay",
        5.0,
        0.0,
        30.0,
        0,
        "Number of seconds after displaying a \"per-frame\" warning before it will display again");
}
