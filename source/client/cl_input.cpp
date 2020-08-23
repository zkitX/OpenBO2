#include "cl_input.h"

#include "../devgui/devgui.h"
#include "../universal/assertive.h"
#include "../win32/win_input.h"

int CL_MouseEvent(LocalClientNum_t localClientNum, int x, int y, int dx, int dy)
{
    int result; // eax

    if (DevGui_IsActive())
    {
        DevGui_MouseEvent(dx, dy);
        result = 1;
    }
    else
    {
        if (localClientNum
            && !assertive::Assert_MyHandler(
                "c:\\t6\\code\\src_noserver\\client_mp\\../client/client.h",
                644,
                0,
                "((localClientNum == 0))",
                "(localClientNum) = %i",
                localClientNum))
        {
            __debugbreak();
        }
        UI_UpdateMouseScreenPosition(localClientNum, x, y);
        if ((dx || dy) && keyCatchers & 0x48 && !UI_KeysBypassMenu(localClientNum) && !R_Is3DOn())
        {
            UI_MouseEvent(localClientNum, x, y);
            result = 0;
        }
        else
        {
            IN_ShowSystemCursor(0);
            mouseIn.mouseDx[mouseIn.mouseIndex] += dx;
            mouseIn.mouseDy[mouseIn.mouseIndex] += dy;
            result = 1;
        }
    }
    return result;
}
