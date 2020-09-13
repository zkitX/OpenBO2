#include "win_gamepad.h"

#include <cmath>
#include <defs.h>

void GPad_UpdateDigitals(ControllerIndex_t portIndex, const _XINPUT_GAMEPAD* xpad)
{
    GamePad* v2; // esi
    long double v3; // st7
    long double v4; // st7
    float leftDeflect; // [esp+10h] [ebp+8h]
    float leftDeflecta; // [esp+10h] [ebp+8h]

#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((portIndex >= 0) && (portIndex < 1))",
            &pBlock))
    {
        __debugbreak();
    }
    if (!xpad && !Assert_MyHandler(__FILE__, __LINE__, 0, "(xpad)", &pBlock))
        __debugbreak();
#endif
    v2 = &s_gamePads[portIndex];
    v2->lastDigitals = v2->digitals;
    v2->digitals = xpad->wButtons;
    v3 = GamerProfile_GetFloat(PROFILE_GPAD_BUTTON_R_STICK_DEFLECT, portIndex);
    if (fabs(v2->sticks[2]) > v3)
        v2->digitals &= 0xFF7Fu;
    leftDeflect = v3;
    if (*reinterpret_cast< float* >(LODWORD(v2->sticks[3]) & _mask__AbsFloat_) > leftDeflect)
        v2->digitals &= 0xFF7Fu;
    v4 = GamerProfile_GetFloat(PROFILE_GPAD_BUTTON_L_STICK_DEFLECT, portIndex);
    if (fabs(v2->sticks[0]) > v4)
        v2->digitals &= 0xFFBFu;
    leftDeflecta = v4;
    if (*reinterpret_cast<float*>(LODWORD(v2->sticks[1]) & _mask__AbsFloat_) > leftDeflecta)
        v2->digitals &= 0xFFBFu;
}

void GPad_UpdateAnalogs(ControllerIndex_t portIndex, const _XINPUT_GAMEPAD* xpad)
{
    ControllerIndex_t v2; // esi
    GamePad* v3; // eax
    float v4; // xmm0_4
    float v5; // xmm0_4
    float button_deadzone; // [esp+4h] [ebp-4h]

    v2 = portIndex;
    button_deadzone = Dvar_GetFloat(gpad_button_deadzone);
#ifdef _DEBUG
    if (((v2 & 0x80000000) != 0 || (signed int)v2 >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((portIndex >= 0) && (portIndex < 1))",
            &pBlock))
    {
        __debugbreak();
    }
    if (!xpad && !Assert_MyHandler(__FILE__, __LINE__, 0, "(xpad)", &pBlock))
        __debugbreak();
#endif
    v3 = &s_gamePads[v2];
    v3->lastAnalogs[0] = v3->analogs[0];
    v4 = (float)((float)((float)(unsigned __int8)xpad->bLeftTrigger * 0.0039215689) - button_deadzone)
        / (float)(1.0 - button_deadzone);
    if (*reinterpret_cast<float*>(LODWORD(v4) ^ _mask__NegFloat_) >= 0.0)
        v4 = 0.0;
    v3->analogs[0] = v4;
    v3->lastAnalogs[1] = v3->analogs[1];
    v5 = (float)((float)((float)(unsigned __int8)xpad->bRightTrigger * 0.0039215689) - button_deadzone)
        / (float)(1.0 - button_deadzone);
    if (*reinterpret_cast<float*>(LODWORD(v5) ^ _mask__NegFloat_) < 0.0)
        v3->analogs[1] = v5;
    else
        v3->analogs[1] = 0.0;
}

void GPad_ConvertStickToFloat(float* outX, float* outY, __int16 inX, __int16 inY)
{
    double v4; // st7
    float stick_deadzone_max; // ST0C_4
    float deadZoneTotal; // ST08_4
    float v7; // xmm0_4
    float v8; // xmm1_4
    float v9; // xmm3_4
    float v10; // xmm5_4
    float v11; // xmm0_4
    float v12; // xmm1_4
    float v13; // xmm3_4
    float stick_deadzone_min; // [esp+8h] [ebp-8h]

    stick_deadzone_min = Dvar_GetFloat(gpad_stick_deadzone_min);
    v4 = Dvar_GetFloat(gpad_stick_deadzone_max);
    v7 = (float)inX * 0.000030518509;
    v8 = (float)inY * 0.000030518509;
    v9 = sqrt((float)(v8 * v8) + (float)(v7 * v7));
    if (*reinterpret_cast<float*>(LODWORD(v9) ^ _mask__NegFloat_) < 0.0)
        v10 = v9;
    else
        v10 = 1.0;
    v11 = v7 * (float)(1.0 / v10);
    v12 = v8 * (float)(1.0 / v10);
    if (stick_deadzone_min <= v9)
    {
        stick_deadzone_max = v4;
        if (v9 <= (float)(1.0 - stick_deadzone_max))
        {
            deadZoneTotal = v4 + stick_deadzone_min;
            v13 = (float)(v9 - stick_deadzone_min) / (float)(1.0 - deadZoneTotal);
        }
        else
        {
            v13 = 1.0;
        }
    }
    else
    {
        v13 = 0.0;
    }
    *outX = v11 * v13;
    *outY = v12 * v13;
}

void GPad_UpdateSticksDown(GamePad* gPad)
{
    GamePad* v1; // esi
    float v2; // xmm1_4
    float v3; // xmm2_4
    char* v4; // edi
    float* v5; // ebx
    int v6; // esi
    char v7; // al
    float v8; // xmm0_4
    signed int v9; // [esp+Ch] [ebp-10h]
    float stick_pressed_hysteresis; // [esp+10h] [ebp-Ch]
    float stick_pressed; // [esp+14h] [ebp-8h]

    v1 = gPad;
    stick_pressed = Dvar_GetFloat(gpad_stick_pressed);
    stick_pressed_hysteresis = Dvar_GetFloat(gpad_stick_pressed_hysteresis);
    v2 = stick_pressed;
    v3 = stick_pressed_hysteresis;
    v4 = (char*)v1->stickDown;
    v5 = v1->sticks;
    v9 = 4;
    do
    {
        v6 = 0;
        do
        {
            v7 = v4[v6];
            v4[v6 + 8] = v7;
            if (v7)
                v8 = v2 - v3;
            else
                v8 = v3 + v2;
            if (v6)
            {
#ifdef _DEBUG
                if (v6 != 1
                    && !Assert_MyHandler(
                        __FILE__,
                        __LINE__,
                        0,
                        "(dir == GPAD_STICK_NEG)",
                        &pBlock))
                {
                    __debugbreak();
                }
#endif
                v2 = stick_pressed;
                v3 = stick_pressed_hysteresis;
                v4[v6] = *reinterpret_cast<float*>(LODWORD(v8) ^ _mask__NegFloat_) > * v5;
            }
            else
            {
                *v4 = *v5 > v8;
            }
            ++v6;
        } while (v6 != 2);
        ++v5;
        v4 += 2;
        --v9;
    } while (v9);
}

void GPad_UpdateSticks(ControllerIndex_t portIndex, const _XINPUT_GAMEPAD* xpad)
{
    GamePad* v2; // ebx
    double v3; // st7
    float v4; // xmm0_4
    double v5; // st7
    float v6; // xmm0_4
    double v7; // st7
    float v8; // xmm0_4
    vec2_t rVec; // [esp+Ch] [ebp-14h]
    vec2_t lVec; // [esp+14h] [ebp-Ch]
#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((portIndex >= 0) && (portIndex < 1))",
            &pBlock))
    {
        __debugbreak();
    }
    if (!xpad && !Assert_MyHandler(__FILE__, __LINE__, 0, "(xpad)", &pBlock))
        __debugbreak();
#endif
    v2 = &s_gamePads[portIndex];
    GPad_ConvertStickToFloat(lVec.v, &lVec.v[1], xpad->sThumbLX, xpad->sThumbLY);
    GPad_ConvertStickToFloat(rVec.v, &rVec.v[1], xpad->sThumbRX, xpad->sThumbRY);
    v3 = v2->sticks[0];
    v2->sticks[0] = lVec.v[0];
    v4 = lVec.v[1];
    v2->lastSticks[0] = v3;
    v5 = v2->sticks[1];
    v2->sticks[1] = v4;
    v6 = rVec.v[0];
    v2->lastSticks[1] = v5;
    v7 = v2->sticks[2];
    v2->sticks[2] = v6;
    v8 = rVec.v[1];
    v2->lastSticks[2] = v7;
    v2->lastSticks[3] = v2->sticks[3];
    v2->sticks[3] = v8;
    GPad_UpdateSticksDown(v2);
}

void GPad_StopRumbles(ControllerIndex_t portIndex)
{
    GamePad* v1; // eax

#ifdef _DEBUG
    if ((unsigned int)portIndex >= 1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(portIndex) < (unsigned)(1)",
            "portIndex doesn't index MAX_GPAD_COUNT\n\t%i not in [0, %i)",
            portIndex,
            1))
    {
        __debugbreak();
    }
#endif
    v1 = &s_gamePads[portIndex];
    v1->feedback.Rumble.wLeftMotorSpeed = 0;
    v1->feedback.Rumble.wRightMotorSpeed = 0;
    if (v1->enabled)
        XInputSetState(portIndex, &v1->feedback.Rumble);
}

bool GPad_InUse(LocalClientNum_t localClientNum)
{
    return Com_LocalClient_LastInput_Get(localClientNum) == 0;
}

bool GPad_IsActive(ControllerIndex_t portIndex)
{
#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((portIndex >= 0) && (portIndex < 1))",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    return s_gamePads[portIndex].enabled;
}

double GPad_GetButton(ControllerIndex_t portIndex, GamePadButton button)
{
    GamePad* v2; // esi
    double result; // st7
    float value; // [esp+0h] [ebp-4h]

    value = 0.0;
#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(( portIndex >= 0 ) && ( portIndex < 1 ))",
            &pBlock))
    {
        __debugbreak();
    }
    if (!(button & 0x30000000)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(button & ( 0x10000000 | 0x20000000 ))",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    v2 = &s_gamePads[portIndex];
    if (button & 0x10000000)
    {
        if (button & v2->digitals & 0xEFFFFFFF)
            result = 1.0;
        else
            result = 0.0;
    }
    else
    {
        if (button & 0x20000000)
            value = v2->analogs[button & 0xDFFFFFFF];
        result = value;
    }
    return result;
}

bool GPad_ButtonRequiresUpdates(ControllerIndex_t portIndex, GamePadButton button)
{
    return button & 0x20000000 && GPad_GetButton(portIndex, button) > 0.0;
}

signed int GPad_IsButtonPressed(ControllerIndex_t portIndex, GamePadButton button)
{
    GamePad* v2; // edi
    unsigned __int32 v3; // esi
    __int32 v4; // eax
    int v5; // ecx
    bool v6; // dl
    int v7; // ecx
    bool v8; // al

#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(( portIndex >= 0 ) && ( portIndex < 1 ))",
            &pBlock))
    {
        __debugbreak();
    }
    if (!(button & 0x30000000)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(button & ( 0x10000000 | 0x20000000 ))",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    v2 = &s_gamePads[portIndex];
    if (!(button & 0x10000000))
    {
        if (!(button & 0x20000000))
            return 0;
        v6 = v2->analogs[button & 0xDFFFFFFF] > 0.0;
        if (v2->lastAnalogs[button & 0xDFFFFFFF] > 0.0)
        {
            v8 = 1;
            goto LABEL_20;
        }
    LABEL_19:
        v8 = 0;
        goto LABEL_20;
    }
    v3 = button & 0xEFFFFFFF;
    v4 = button & 0xF;
    if (button & 0xF)
    {
        v5 = v2->digitals;
        if (v5 & v3)
        {
            if ((v5 & 0xF) != v4)
                return 0;
        }
    }
    v6 = (v2->digitals & v3) != 0;
    if (button & 0xF)
    {
        v7 = v2->lastDigitals;
        if (v7 & v3)
        {
            if ((v7 & 0xF) != v4)
                goto LABEL_19;
        }
    }
    v8 = (v2->lastDigitals & v3) != 0;
LABEL_20:
    if (v6 && !v8)
        return 1;
    return 0;
}

signed int GPad_IsButtonReleased(ControllerIndex_t portIndex, GamePadButton button)
{
    GamePad* v2; // edi
    bool v3; // cl
    bool v4; // al

#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(( portIndex >= 0 ) && ( portIndex < 1 ))",
            &pBlock))
    {
        __debugbreak();
    }
    if (!(button & 0x30000000)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(button & ( 0x10000000 | 0x20000000 ))",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    v2 = &s_gamePads[portIndex];
    if (button & 0x10000000)
    {
        v3 = (v2->digitals & button & 0xEFFFFFFF) != 0;
        v4 = (v2->lastDigitals & button & 0xEFFFFFFF) != 0;
    }
    else
    {
        if (!(button & 0x20000000))
            return 0;
        v3 = v2->analogs[button & 0xDFFFFFFF] > 0.0;
        v4 = v2->lastAnalogs[button & 0xDFFFFFFF] > 0.0;
    }
    if (!v3 && v4)
        return 1;
    return 0;
}

double GPad_GetStick(ControllerIndex_t portIndex, GamePadStick stick)
{
#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(( portIndex >= 0 ) && ( portIndex < 1 ))",
            &pBlock))
    {
        __debugbreak();
    }
    if (!(stick & 0x40000000)
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(stick & 0x40000000)", &pBlock))
    {
        __debugbreak();
    }
#endif
    return flt_7AB2058[31 * portIndex + (stick & 0xBFFFFFFF)];
}

void GPad_SetLowRumble(ControllerIndex_t portIndex, float rumble)
{
#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(( portIndex >= 0 ) && ( portIndex < 1 ))",
            &pBlock))
    {
        __debugbreak();
    }
    if (rumble < 0.0
        && rumble > 1.0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((rumble >= 0.0f) || (rumble <= 1.0f))",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    flt_lowrumble[31 * portIndex] = rumble;
}

void GPad_SetHighRumble(ControllerIndex_t portIndex, float rumble)
{
#ifdef _DEBUG
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((portIndex >= 0) && (portIndex < 1))",
            &pBlock))
    {
        __debugbreak();
    }
    if (rumble < 0.0
        && rumble > 1.0
        && !Assert_MyHandler(
            "c:\\t6\\code\\src_noserver\\win32\\win_gamepad.cpp",
            658,
            0,
            "((rumble >= 0.0f) || (rumble <= 1.0f))",
            &pBlock))
    {
        __debugbreak();
    }
#endif
    flt_highrumble[31 * portIndex] = rumble;
}

bool GPad_GetStickChangedToPressedState(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir, bool pressedState)
{
    ControllerIndex_t v4; // esi
    GamePadStick v5; // edi
    unsigned int v6; // edi
    GamePad* v7; // esi
    bool v8; // al

    v4 = portIndex;
    v5 = stick;
    if (((portIndex & 0x80000000) != 0 || (signed int)portIndex >= 1)
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(( portIndex >= 0 ) && ( portIndex < 1 ))",
            &pBlock))
    {
        __debugbreak();
    }
    if (!(v5 & 0x40000000)
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(stick & 0x40000000)", &pBlock))
    {
        __debugbreak();
    }
    if (stickDir
        && stickDir != 1
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(( stickDir == GPAD_STICK_POS ) || ( stickDir == GPAD_STICK_NEG ))",
            &pBlock))
    {
        __debugbreak();
    }
    v6 = v5 & 0xBFFFFFFF;
    v7 = &s_gamePads[v4];
    if (v6 >= 4
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((unsigned)stickIndex < 4)",
            &pBlock))
    {
        __debugbreak();
    }
    v8 = v7->stickDown[v6][stickDir];
    return v8 != v7->stickDownLast[v6][stickDir] && v8 == pressedState;
}

bool GPad_IsStickPressed(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir)
{
    return GPad_GetStickChangedToPressedState(portIndex, stick, stickDir, 1);
}

bool GPad_IsStickReleased(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir)
{
    return GPad_GetStickChangedToPressedState(portIndex, stick, stickDir, 0);
}

void GPad_RefreshAll()
{
    bool v0; // bl
    bool v1; // al
    _XINPUT_GAMEPAD xpad; // [esp+4h] [ebp-Ch]

    v0 = s_gamePads[0].enabled;
    v1 = 0;
    if (!XInputGetCapabilities(0, 1u, &pCapabilities))
    {
        Com_ControllerIndex_GetLocalClientNum(0);
        if (Dvar_GetBool(gpad_enabled))
            v1 = 1;
    }
    s_gamePads[0].enabled = v1;
    byte_7AB2041 = 0;
    if (s_removedCB && v0 && !v1)
    {
        *(unsigned __int64*)&xpad.wButtons = 0i64;
        *(unsigned __int64*)&xpad.sThumbRX = 0;
        GPad_UpdateSticks(CONTROLLER_INDEX_FIRST, &xpad);
        GPad_UpdateDigitals(CONTROLLER_INDEX_FIRST, &xpad);
        GPad_UpdateAnalogs(CONTROLLER_INDEX_FIRST, &xpad);
        s_removedCB(CONTROLLER_INDEX_FIRST);
    }
    else if (s_insertedCB && !v0 && v1)
    {
        s_insertedCB(CONTROLLER_INDEX_FIRST);
    }
}

void GPad_UpdateAll()
{
    int gpadDebug; // esi
    _XINPUT_STATE inputState; // [esp+8h] [ebp-14h]

    gpadDebug = Dvar_GetInt(gpad_debug);
    if (gpadDebug >= 0)
    {
        GPad_RefreshAll();
        if (!XInputGetState(0, &inputState))
        {
            GPad_UpdateSticks(CONTROLLER_INDEX_FIRST, &inputState.Gamepad);
            GPad_UpdateDigitals(CONTROLLER_INDEX_FIRST, &inputState.Gamepad);
            GPad_UpdateAnalogs(CONTROLLER_INDEX_FIRST, &inputState.Gamepad);
        }
        goto LABEL_9;
    }
    if (!inputCounter)
    {
        if (hasInput[0])
        {
            GPad_UpdateSticks(CONTROLLER_INDEX_FIRST, &xpad);
            GPad_UpdateDigitals(CONTROLLER_INDEX_FIRST, &xpad);
            GPad_UpdateAnalogs(CONTROLLER_INDEX_FIRST, &xpad);
        }
    }
    if (--inputCounter <= gpadDebug)
    {
        GPad_RefreshAll();
        hasInput[0] = XInputGetState(0, delayedInputState) == 0;
    LABEL_9:
        inputCounter = 0;
        return;
    }
}
