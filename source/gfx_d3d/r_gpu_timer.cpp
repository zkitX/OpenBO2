#include "r_gpu_timer.h"

#include <qcommon/com_clients.h>
#include <universal/assertive.h>
#include <universal/win_common.h>

DxGlobals dx;

void R_GPU_CalcDerivedTimers(GPUTimerFrame* timerFrame)
{
    unsigned int v1; // edx
    unsigned int v2; // edi
    GPUTimer* v3; // ecx
    signed int v4; // ebx
    unsigned int v5; // ecx
    unsigned int v6; // edi
    unsigned int v7; // esi
    unsigned int v8; // edx
    unsigned int v9; // edx
    unsigned int v10; // ecx
    unsigned int v11; // edx
    unsigned int v12; // esi
    unsigned int v13; // edx
    unsigned int v14; // edx
    unsigned int v15; // edx
    unsigned int v16; // edx
    bool v17; // zf
    unsigned int v18; // ecx

    v1 = -1;
    v2 = 0;
    v3 = timerFrame->timers;
    v4 = 8;
    do
    {
        if (v3->isActive)
        {
            if (v3->beginTime < v1)
                v1 = v3->beginTime;
            if (v2 < v3->totalTime + v3->beginTime)
                v2 = v3->totalTime + v3->beginTime;
        }
        if (v3[1].isActive)
        {
            if (v3[1].beginTime < v1)
                v1 = v3[1].beginTime;
            if (v2 < v3[1].beginTime + v3[1].totalTime)
                v2 = v3[1].beginTime + v3[1].totalTime;
        }
        if (v3[2].isActive)
        {
            if (v3[2].beginTime < v1)
                v1 = v3[2].beginTime;
            if (v2 < v3[2].beginTime + v3[2].totalTime)
                v2 = v3[2].beginTime + v3[2].totalTime;
        }
        if (v3[3].isActive)
        {
            if (v3[3].beginTime < v1)
                v1 = v3[3].beginTime;
            if (v2 < v3[3].beginTime + v3[3].totalTime)
                v2 = v3[3].beginTime + v3[3].totalTime;
        }
        if (v3[4].isActive)
        {
            if (v3[4].beginTime < v1)
                v1 = v3[4].beginTime;
            if (v2 < v3[4].beginTime + v3[4].totalTime)
                v2 = v3[4].beginTime + v3[4].totalTime;
        }
        v3 += 5;
        --v4;
    } while (v4);
    v5 = timerFrame->timers[5].beginTime;
    v6 = v2 - v1;
    v7 = timerFrame->timers[5].totalTime;
    timerFrame->beginTime = v1;
    timerFrame->timers[11].beginTime = timerFrame->timers[16].beginTime;
    v8 = timerFrame->timers[9].totalTime + timerFrame->timers[10].totalTime;
    timerFrame->timers[2].beginTime = v5;
    v9 = timerFrame->timers[8].totalTime + v8;
    timerFrame->timers[27].beginTime = timerFrame->timers[28].beginTime;
    v10 = v9 + timerFrame->timers[7].totalTime;
    v11 = timerFrame->timers[6].totalTime;
    timerFrame->totalTime = v6;
    s_totalTime = v6;
    v12 = v11 + v7;
    v13 = timerFrame->timers[23].totalTime + timerFrame->timers[25].totalTime + timerFrame->timers[31].totalTime;
    timerFrame->timers[2].totalTime = v10 + v12;
    v14 = timerFrame->timers[21].totalTime + timerFrame->timers[22].totalTime + v13;
    timerFrame->timers[3].totalTime = v12;
    v15 = timerFrame->timers[19].totalTime + v14;
    timerFrame->timers[4].totalTime = v10;
    v16 = timerFrame->timers[16].totalTime + timerFrame->timers[17].totalTime + timerFrame->timers[18].totalTime + v15;
    timerFrame->timers[27].totalTime = timerFrame->timers[28].totalTime;
    v17 = s_displayMode == 1;
    timerFrame->timers[11].totalTime = timerFrame->timers[12].totalTime + timerFrame->timers[13].totalTime + v16;
    if (v17)
    {
        v18 = timerFrame->timers[35].totalTime + timerFrame->timers[38].totalTime;
        timerFrame->timers[39].beginTime = timerFrame->timers[35].beginTime;
        timerFrame->timers[39].totalTime = v18;
        timerFrame->timers[35].isActive = 0;
        timerFrame->timers[38].isActive = 0;
        timerFrame->timers[39].isActive = 1;
    }
}

void R_GPU_CalcTimers(GPUTimerFrame* timerFrame)
{
    GPUTimerFrame* v1; // edi
    GPUTimer* v2; // edi
    unsigned int v3; // esi
    GPUTimeStamp* v4; // esi
    unsigned __int64 v5; // kr08_8
    double v6; // st7
    GPUTimeStamp* v7; // edx
    GPUTimeStamp* v8; // eax
    unsigned int v9; // eax
    int v10; // ecx
    unsigned int v11; // eax
    unsigned int v12; // eax
    GPUTimer* v13; // eax
    int v14; // ecx
    GPUTimer* v15; // [esp+3Ch] [ebp-40h]
    unsigned __int64 beginTime; // [esp+40h] [ebp-3Ch]
    unsigned __int64 endTime; // [esp+48h] [ebp-34h]
    unsigned int v18; // [esp+50h] [ebp-2Ch]
    unsigned int frameEndTime; // [esp+54h] [ebp-28h]
    unsigned int frameBeginTime; // [esp+58h] [ebp-24h]
    __int64 v21; // [esp+5Ch] [ebp-20h]
    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint; // [esp+68h] [ebp-14h]

    v1 = timerFrame;
    frameBeginTime = -1;
    frameEndTime = 0;
    Sys_EnterCriticalSection(CRITSECT_DXCONTEXT);
    v2 = v1->timers;
    v3 = 0;
    v15 = v2;
    v18 = 0;
    do
    {
        if (v2->currentTimeStamp
            && !Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(!timer->currentTimeStamp)",
                nullptr))
        {
            __debugbreak();
        }
        if (v2->firstTimeStamp)
        {
            v4 = v2->firstTimeStamp;
            do
            {
                while ((dx.context->GetData)(v4->beginQuery, &beginTime, 8, 0))
                    Sleep(0);
                while ((dx.context->GetData)(v4->endQuery, &endTime, 8, 0))
                    Sleep(0);
                while ((dx.context->GetData)(v4->disjoint, &disjoint, 16, 0))
                    Sleep(0);
                v5 = beginTime;
                v6 = (disjoint.Frequency / 0xF4240);
                v2->beginTime = (beginTime / v6);
                v7 = s_firstFreeTimeStamp;
                s_firstFreeTimeStamp = v4;
                v21 = ((endTime - v5) / v6);
                v2->totalTime += v21;
                v8 = v4->next;
                v4->next = v7;
                v4 = v8;
            } while (v8);
            v9 = v2->beginTime;
            v3 = v18;
            *(LastGoodElapsed + v18) = v2->totalTime;
            LastGoodTime = v9;
            v2->firstTimeStamp = 0;
        }
        else
        {
            v2->beginTime = LastGoodTime;
        }
        v10 = (v2->totalTime + v2->adjustTime) < 0 ? 0 : v2->totalTime + v2->adjustTime;
        v11 = v2->beginTime;
        v2->totalTime = v10;
        if (v11 < frameBeginTime)
            frameBeginTime = v11;
        v12 = v10 + v11;
        if (frameEndTime < v12)
            frameEndTime = v12;
        v3 += 4;
        ++v2;
        v18 = v3;
    } while (v3 < 0xA0);
    if (frameBeginTime > frameEndTime)
    {
        v13 = v15;
        v14 = 40;
        do
        {
            v13->beginTime -= 0x7FFFFFFF;
            ++v13;
            --v14;
        } while (v14);
    }
    Sys_LeaveCriticalSection(CRITSECT_DXCONTEXT);
}

void R_GPU_FreeInterveningTimeStamps(int lastReadIndex, int readIndex)
{
    int v2; // ecx
    int v3; // ebx
    GPUTimeStamp* v4; // edx
    GPUTimeStamp** v5; // esi
    int v6; // edi
    GPUTimeStamp* v7; // eax
    GPUTimeStamp* v8; // ecx

    v2 = readIndex;
    if (readIndex < lastReadIndex)
    {
        v2 = readIndex + 64;
        readIndex += 64;
    }
    v3 = lastReadIndex + 1;
    if (lastReadIndex + 1 < v2)
    {
        v4 = s_firstFreeTimeStamp;
        do
        {
            v5 = &s_timerFrames[v3 % 64].timers[0].firstTimeStamp;
            v6 = 40;
            do
            {
                if (v5[1])
                {
                    if (!Assert_MyHandler(
                        __FILE__,
                        __LINE__,
                        0,
                        "(!timer->currentTimeStamp)",
                        nullptr))
                        __debugbreak();
                    v4 = s_firstFreeTimeStamp;
                }
                v7 = *v5;
                if (*v5)
                {
                    do
                    {
                        v8 = v7->next;
                        v7->next = v4;
                        v4 = v7;
                        v7 = v8;
                    } while (v8);
                    s_firstFreeTimeStamp = v4;
                    *v5 = 0;
                }
                v5 += 8;
                --v6;
            } while (v6);
            ++v3;
        } while (v3 < readIndex);
    }
}

void R_GPU_EndFrameCallback()
{
    unsigned int* v0; // eax
    int v1; // ecx

    s_readIndex = s_callbackIndex;
    s_callbackIndex = (s_callbackIndex + 1) % 64;
    v0 = &s_timerFrames[0].timers[0].totalTime + 324 * s_callbackIndex;
    v1 = 40;
    do
    {
        *v0 = 0;
        v0 += 8;
        --v1;
    } while (v1);
}

void R_GPU_BeginFrame(void)
{
    GPUTimerFrame* v0; // esi
    int v1; // eax
    int v2; // ecx

    s_displayMode = Dvar_GetInt(r_gpuTimers);
    if (s_displayMode)
    {
        v0 = &s_timerFrames[s_writeIndex];
        v0->frameIndex = s_frameIndex;
        if (v0->isCurrent
            && !Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(!timerFrame->isCurrent)",
                nullptr))
        {
            __debugbreak();
        }
        ++s_frameIndex;
        v0->isCurrent = 1;
        v1 = &v0->timers[0].hide;
        v2 = 40;
        do
        {
            *(v1 - 1) = 0;
            *(v1 + 10) = 0;
            v1 += 32;
            --v2;
        } while (v2);
        if (Dvar_GetBool(r_vsync))
            Dvar_SetBool(r_vsync, 0);
    }
    else
    {
        s_frameIndex = 0;
    }
}

void R_GPU_EndFrame(void)
{
    GPUTimerFrame* v0; // esi
    bool v1; // sf
    signed int v2; // eax

    if (s_displayMode)
    {
        if (s_currentTimer
            && !Assert_MyHandler(__FILE__, __LINE__, 0, "(!s_currentTimer)", nullptr))
        {
            __debugbreak();
        }
        v0 = &s_timerFrames[s_writeIndex];
        if (!v0->isCurrent
            && !Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(timerFrame->isCurrent)",
                nullptr))
        {
            __debugbreak();
        }
        v2 = (s_writeIndex + 1) & 0x8000003F;
        v1 = v2 < 0;
        v0->isCurrent = 0;
        if (v1)
            v2 = ((v2 - 1) | 0xFFFFFFC0) + 1;
        s_writeIndex = v2;
        R_GPU_EndFrameCallback();
    }
}

void R_GPU_BeginTimer(GPUTimerId timerId)
{
    GPUTimerFrame* v3; // esi
    GPUTimer* v4; // eax
    GPUTimer* v5; // esi
    bool v6; // zf
    GPUTimeStamp* v7; // eax
    GPUTimeStamp* v8; // ecx
    GPUTimeStamp* v9; // edi

    if (s_displayMode)
    {
        v3 = &s_timerFrames[s_writeIndex];
        if (!v3->isCurrent
            && !Assert_MyHandler(
                "c:\\t6\\code\\src_noserver\\gfx_d3d\\r_gpu_timer.cpp",
                725,
                0,
                "(timerFrame->isCurrent)",
                nullptr))
        {
            __debugbreak();
        }
        if ((timerId < 0 || timerId >= 0x28)
            && !Assert_MyHandler(
                "c:\\t6\\code\\src_noserver\\gfx_d3d\\r_gpu_timer.cpp",
                726,
                0,
                "(timerId >= 0 && static_cast< uint >( timerId ) < GPU_TIMER_COUNT)",
                nullptr))
        {
            __debugbreak();
        }
        v4 = s_currentTimer;
        v5 = &v3->timers[timerId];
        if (s_currentTimer)
        {
            if (s_currentTimer == v5)
            {
                if (!Assert_MyHandler(
                    "c:\\t6\\code\\src_noserver\\gfx_d3d\\r_gpu_timer.cpp",
                    731,
                    0,
                    "(s_currentTimer != timer)",
                    nullptr))
                    __debugbreak();
                v4 = s_currentTimer;
            }
            v4->hasChild = 1;
        }
        v6 = s_firstFreeTimeStamp == 0;
        v5->parent = v4;
        s_currentTimer = v5;
        v5->hasChild = 256;
        v5->bucket = 0;
        if (v6
            && !Assert_MyHandler(
                "c:\\t6\\code\\src_noserver\\gfx_d3d\\r_gpu_timer.cpp",
                246,
                0,
                "(s_firstFreeTimeStamp)",
                nullptr))
        {
            __debugbreak();
        }
        v7 = s_firstFreeTimeStamp;
        v8 = s_firstFreeTimeStamp->next;
        s_firstFreeTimeStamp->next = 0;
        v9 = v7;
        v7->next = v5->firstTimeStamp;
        s_firstFreeTimeStamp = v8;
        v5->firstTimeStamp = v7;
        v5->currentTimeStamp = v7;
        Sys_EnterCriticalSection(CRITSECT_DXCONTEXT);
        (dx.context->Begin)(v9->disjoint);
        (dx.context->End)(v9->beginQuery);
        Sys_LeaveCriticalSection(CRITSECT_DXCONTEXT);
    }
}

void R_GPU_EndTimer()
{
    GPUTimeStamp* v1; // esi

    if (s_displayMode)
    {
        if (!s_currentTimer
            && !Assert_MyHandler(__FILE__, __LINE__, 0, "(s_currentTimer)", nullptr))
        {
            __debugbreak();
        }
        if (s_currentTimer->currentTimeStamp != s_currentTimer->firstTimeStamp
            && !Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(s_currentTimer->currentTimeStamp == s_currentTimer->firstTimeStamp)",
                nullptr))
        {
            __debugbreak();
        }
        v1 = s_currentTimer->currentTimeStamp;
        s_currentTimer->currentTimeStamp = 0;
        Sys_EnterCriticalSection(CRITSECT_DXCONTEXT);
        (dx.context->End)(v1->endQuery);
        (dx.context->End)(v1->disjoint);
        Sys_LeaveCriticalSection(CRITSECT_DXCONTEXT);
        s_currentTimer = (GPUTimer*)s_currentTimer->parent;
    }
}

void R_GPU_DrawTimers(void)
{
    unsigned int v0; // edi
    GPUTimerFrame* v1; // esi
    unsigned int v2; // eax
    unsigned int v3; // edx
    GPUTimerDisplayMode v4; // eax
    PerfTimerDisplayType v5; // edx
    const char* v6; // eax
    float v7; // xmm0_4

    if (s_displayMode)
    {
        PIXBeginNamedEvent(-1, "R_GPU_DrawTimers");
        v0 = s_readIndex;
        R_GPU_FreeInterveningTimeStamps(s_lastReadIndex, s_readIndex);
        v1 = &s_timerFrames[v0];
        s_lastReadIndex = v0;
        R_GPU_CalcTimers(&s_timerFrames[v0]);
        R_GPU_CalcDerivedTimers(&s_timerFrames[v0]);
        v2 = v1->beginTime;
        v3 = v1->totalTime;
        s_gpuTimerFrameHistory.currentFrame = v0;
        s_gpuTimerFrameHistory.frames[v0].beginTime = v2;
        v4 = s_displayMode;
        s_gpuTimerFrameHistory.frames[s_gpuTimerFrameHistory.currentFrame].totalTime = v3;
        v5 = s_gpuPerfType[v4];
        s_gpuTimerStyle.displayMode = s_gpuPerfMode[v4];
        s_gpuTimerStyle.displayType = v5;
        if (v4 != 13)
        {
            if (!s_gpuTimerStyle.font)
            {
                v6 = R_GetFontPathFromName("consoleFont");
                s_gpuTimerStyle.font = R_RegisterFont(v6, 3);
                s_gpuTimerStyle.fontWidth = R_TextWidth(INVALID_LOCAL_CLIENT, "#", 1, s_gpuTimerStyle.font);
                v7 = R_TextHeight(s_gpuTimerStyle.font);
                v4 = s_displayMode;
                s_gpuTimerStyle.fontHeight = v7;
            }
            if (v4 == 1)
            {
                s_gpuTimerStyle.timerNameCharCount = g_gpuTimerSimpleNamesMaxLength;
                s_gpuTimerStyle.timerNames = g_gpuTimerSimpleNames;
            }
            else
            {
                s_gpuTimerStyle.timerNameCharCount = g_gpuTimerNamesMaxLength;
                s_gpuTimerStyle.timerNames = g_gpuTimerNames;
            }
            R_Perf_DrawTimers(&s_gpuTimerFrameHistory, &s_gpuTimerStyle);
        }
        if (Sys_IsRenderThread())
            D3DPERF_EndEvent();
    }
}