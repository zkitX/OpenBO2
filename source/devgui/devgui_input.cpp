#include "devgui_input.h"
#include "devgui.h"

#include "../qcommon/cmd.h"

float i_fsign(float val)
{
    float result; // st7

    if (val < 0.0)
        result = -1.0;
    else
        result = 1.0;
    return result;
}

bool DevGui_IsButtonDown(DevGuiInputButton button)
{
    return s_input.buttonDown[button];
}

void DevGui_MouseEvent(int dx, int dy)
{
    s_input.mousePos[0] = (float)dx;
    s_input.mousePos[1] = (float)dy;
}

void DevGui_InputInit()
{
    Cmd_AddCommandInternal("devgui", DevGui_Toggle, &DevGui_Toggle_VAR);
}

void DevGui_InputShutdown()
{
    //Cmd_RemoveCommand("devgui");
}

void DevGui_SelectGamepad(ControllerIndex_t gamePadIndex)
{
    s_input.selectedGamePadIndex = gamePadIndex;
}
