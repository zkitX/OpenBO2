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
        v10 = *(float*)&FLOAT_1_0;
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
            v13 = *(float*)&FLOAT_1_0;
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
}

void GPad_UpdateSticks(ControllerIndex_t portIndex, const _XINPUT_GAMEPAD* xpad)
{
}

void GPad_StopRumbles(ControllerIndex_t portIndex)
{
}

bool GPad_InUse(LocalClientNum_t localClientNum)
{
	return false;
}

bool GPad_IsActive(ControllerIndex_t portIndex)
{
	return false;
}

double GPad_GetButton(ControllerIndex_t portIndex, GamePadButton button)
{
	return 0.0;
}

bool GPad_ButtonRequiresUpdates(ControllerIndex_t portIndex, GamePadButton button)
{
	return false;
}

signed int GPad_IsButtonPressed(ControllerIndex_t portIndex, GamePadButton button)
{
	return 0;
}

signed int GPad_IsButtonReleased(ControllerIndex_t portIndex, GamePadButton button)
{
	return 0;
}

double GPad_GetStick(ControllerIndex_t portIndex, GamePadStick stick)
{
	return 0.0;
}

void GPad_SetLowRumble(ControllerIndex_t portIndex, float rumble)
{
}

void GPad_SetHighRumble(ControllerIndex_t portIndex, float rumble)
{
}

bool GPad_GetStickChangedToPressedState(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir, bool pressedState)
{
	return false;
}

bool GPad_IsStickPressed(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir)
{
	return false;
}

bool GPad_IsStickReleased(ControllerIndex_t portIndex, GamePadStick stick, GamePadStickDir stickDir)
{
	return false;
}

void GPad_RefreshAll()
{
}

void GPad_UpdateAll()
{
}
