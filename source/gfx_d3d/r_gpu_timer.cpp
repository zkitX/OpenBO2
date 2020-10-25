#include "pch.h"
#include "r_gpu_timer.h"

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
    unsigned int v5; // ecx
    unsigned __int64 v6; // kr00_8
    unsigned int v7; // edx
    unsigned __int64 v8; // kr08_8
    GPUTimeStamp* v9; // edx
    GPUTimeStamp* v10; // eax
    unsigned int v11; // eax
    signed int v12; // ecx
    unsigned int v13; // eax
    unsigned int v14; // eax
    GPUTimer* v15; // eax
    signed int v16; // ecx
    GPUTimer* v17; // [esp+3Ch] [ebp-40h]
    unsigned __int64 beginTime; // [esp+40h] [ebp-3Ch]
    unsigned __int64 endTime; // [esp+48h] [ebp-34h]
    unsigned int v20; // [esp+50h] [ebp-2Ch]
    unsigned int frameEndTime; // [esp+54h] [ebp-28h]
    unsigned int frameBeginTime; // [esp+58h] [ebp-24h]
    __int64 v23; // [esp+5Ch] [ebp-20h]
    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint; // [esp+68h] [ebp-14h]

    v1 = timerFrame;
    frameBeginTime = -1;
    frameEndTime = 0;
    Sys_EnterCriticalSection(CRITSECT_DXCONTEXT);
    v2 = v1->timers;
    v3 = 0;
    v17 = v2;
    v20 = 0;
    do
    {
        if (v2->currentTimeStamp
            && !(unsigned __int8)Assert_MyHandler(
                "c:\\t6\\code\\src_noserver\\gfx_d3d\\r_gpu_timer.cpp",
                386,
                0,
                "(!timer->currentTimeStamp)",
                (const char*)&clientinfo))
        {
            __debugbreak();
        }
        if (v2->firstTimeStamp)
        {
            v4 = v2->firstTimeStamp;
            do
            {
                while (((int(*)(ID3D11DeviceContext*, ID3D11Query*, unsigned __int64*, signed int, int))dx.context->vfptr[9].Release)(
                    dx.context,
                    v4->beginQuery,
                    &beginTime,
                    8,
                    0))
                    Sleep(0);
                while (((int(*)(ID3D11DeviceContext*, ID3D11Query*, unsigned __int64*, signed int, int))dx.context->vfptr[9].Release)(
                    dx.context,
                    v4->endQuery,
                    &endTime,
                    8,
                    0))
                    Sleep(0);
                while (((int(*)(ID3D11DeviceContext*, ID3D11Query*, D3D11_QUERY_DATA_TIMESTAMP_DISJOINT*, signed int, int))dx.context->vfptr[9].Release)(
                    dx.context,
                    v4->disjoint,
                    &disjoint,
                    16,
                    0))
                    Sleep(0);
                v5 = beginTime;
                v6 = disjoint.Frequency / 0xF4240;
                v7 = HIDWORD(beginTime);
                v2->beginTime = (signed __int64)((double)beginTime / (double)(disjoint.Frequency / 0xF4240));
                v8 = endTime - __PAIR__(v7, v5);
                v9 = s_firstFreeTimeStamp;
                s_firstFreeTimeStamp = v4;
                v23 = (signed __int64)((double)v8 / (double)v6);
                v2->totalTime += v23;
                v10 = v4->next;
                v4->next = v9;
                v4 = v10;
            } while (v10);
            v11 = v2->beginTime;
            v3 = v20;
            *(unsigned int*)((char*)LastGoodElapsed + v20) = v2->totalTime;
            LastGoodTime = v11;
            v2->firstTimeStamp = 0;
        }
        else
        {
            v2->beginTime = LastGoodTime;
        }
        v12 = (signed int)(v2->totalTime + v2->adjustTime) < 0 ? 0 : v2->totalTime + v2->adjustTime;
        v13 = v2->beginTime;
        v2->totalTime = v12;
        if (v13 < frameBeginTime)
            frameBeginTime = v13;
        v14 = v12 + v13;
        if (frameEndTime < v14)
            frameEndTime = v14;
        v3 += 4;
        ++v2;
        v20 = v3;
    } while (v3 < 0xA0);
    if (frameBeginTime > frameEndTime)
    {
        v15 = v17;
        v16 = 40;
        do
        {
            v15->beginTime -= 0x7FFFFFFF;
            ++v15;
            --v16;
        } while (v16);
    }
    Sys_LeaveCriticalSection(CRITSECT_DXCONTEXT);
}

void R_GPU_FreeInterveningTimeStamps(int, int)
{
}

void R_GPU_EndFrameCallback(void)
{
}

void R_GPU_BeginFrame(void)
{
}

void R_GPU_EndFrame(void)
{
}

void R_GPU_BeginTimer(GPUTimerId)
{
}

void R_GPU_EndTimer(void)
{
}

void R_GPU_DrawTimers(void)
{
}