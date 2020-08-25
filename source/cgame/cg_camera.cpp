#include "cg_camera.h"

#include "../universal/assertive.h"
#include "../win32/win_gamepad.h"

void CG_UpdateVehicleBindings(Bind_t a1, LocalClientNum_t localClientNum)
{
    LocalClientNum_t v2; // esi
    cg_t* v3; // eax
    __int16 v4; // ax
    const centity_t* v5; // eax
    const VehicleDef* v6; // edi
    int v7; // edx
    int v8; // eax
    int v9; // ebx
    int v10; // ecx
    int v11; // edx
    int v12; // eax
    int v13; // ecx
    int v14; // edx
    ControllerIndex_t v15; // eax
    Bind_t v16; // [esp-8h] [ebp-30h]
    int twokeys[2]; // [esp+4h] [ebp-24h]
    int boostButton; // [esp+Ch] [ebp-1Ch]
    int switchSeatButton; // [esp+10h] [ebp-18h]
    int attackSecondaryButton; // [esp+14h] [ebp-14h]
    int attackButton; // [esp+18h] [ebp-10h]
    int handBrakeButton; // [esp+1Ch] [ebp-Ch]
    int reverseBrakeButton; // [esp+20h] [ebp-8h]
    int moveDownButton; // [esp+24h] [ebp-4h]
    signed int moveUpButton; // [esp+30h] [ebp+8h]

    v2 = localClientNum;
    if (((localClientNum & 0x80000000) != 0 || (signed int)localClientNum >= 1)
        && !assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((localClientNum >= 0 && localClientNum < 1))",
            "(localClientNum) = %i",
            localClientNum))
    {
        __debugbreak();
    }
    if (((unsigned int)clientUIActives[localClientNum].flags >> 5) & 1)
    {
        if (CL_LocalClientIsInGame(localClientNum))
        {
            v3 = CG_GetLocalClientGlobals(localClientNum);
            if (v3)
            {
                if (v3->snap)
                {
                    if (v3->predictedPlayerState.eFlags & 0x4000)
                    {
                        if (!v3->predictedPlayerState.vehiclePos)
                        {
                            v4 = v3->predictedPlayerState.viewlocked_entNum;
                            if (v4 != 1023)
                            {
                                v16 = a1;
                                v5 = CG_GetEntity(localClientNum, v4);
                                v6 = CG_Vehicle_GetVehicleDef(v5);
                                v7 = v6->handBrakeButton;
                                v8 = v6->attackButton;
                                v9 = v6->gasButton;
                                reverseBrakeButton = v6->reverseBrakeButton;
                                v10 = v6->attackSecondaryButton;
                                handBrakeButton = v7;
                                v11 = v6->moveUpButton;
                                attackButton = v8;
                                v12 = v6->moveDownButton;
                                attackSecondaryButton = v10;
                                v13 = v6->switchSeatButton;
                                moveUpButton = v11;
                                v14 = v6->boostButton;
                                moveDownButton = v12;
                                switchSeatButton = v13;
                                boostButton = v14;
                                v15 = Com_LocalClient_GetControllerIndex(v2);
                                Cbuf_ExecuteBuffer(v2, v15, "unbindall2");
                                if (!GPad_InUse(v2) && !I_strcmp(v6->name, "qrdrone_mp"))
                                {
                                    moveUpButton = 101;
                                    moveDownButton = 113;
                                }
                                Key_SetBinding(v2, 18, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 19, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 6, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 5, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 1, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 2, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 3, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 4, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 16, 0, BIND_INDEX_VEHICLE, 0);
                                Key_SetBinding(v2, 17, 0, BIND_INDEX_VEHICLE, 0);
                                if (v9 > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, v9, v16);
                                    Key_SetBindings(v2, twokeys, BIND_GAS1, BIND_INDEX_VEHICLE);
                                }
                                if (reverseBrakeButton > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, reverseBrakeButton, v16);
                                    Key_SetBindings(v2, twokeys, BIND_REVERSE1, BIND_INDEX_VEHICLE);
                                }
                                if (handBrakeButton > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, handBrakeButton, v16);
                                    Key_SetBindings(v2, twokeys, BIND_HANDBRAKE1, BIND_INDEX_VEHICLE);
                                }
                                if (attackButton > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, attackButton, v16);
                                    Key_SetBindings(v2, twokeys, BIND_VEHICLEATTACK1, BIND_INDEX_VEHICLE);
                                }
                                if (attackSecondaryButton > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, attackSecondaryButton, v16);
                                    Key_SetBindings(v2, twokeys, BIND_VEHICLESECONDATTACK1, BIND_INDEX_VEHICLE);
                                }
                                if (moveUpButton > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, moveUpButton, v16);
                                    Key_SetBindings(v2, twokeys, BIND_VEHICLEMOVEUP1, BIND_INDEX_VEHICLE);
                                }
                                if (moveDownButton > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, moveDownButton, v16);
                                    Key_SetBindings(v2, twokeys, BIND_VEHICLEMOVEDOWN1, BIND_INDEX_VEHICLE);
                                }
                                if (switchSeatButton > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, switchSeatButton, v16);
                                    Key_SetBindings(v2, twokeys, BIND_VEHICLESWTICHSEAT1, BIND_INDEX_VEHICLE);
                                }
                                if (boostButton > 0)
                                {
                                    CG_RemapVehicleButton(v2, twokeys, boostButton, v16);
                                    Key_SetBindings(v2, twokeys, BIND_VEHICLEBOOST1, BIND_INDEX_VEHICLE);
                                }
                            }
                        }
                    }
                    CL_ClearKeys(v2);
                }
            }
        }
    }
}
