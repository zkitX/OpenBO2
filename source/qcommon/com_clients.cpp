#include "com_clients.h"

#include <defs.h>
#include <universal/assertive.h>
#include <win32/win_main.h>

void Com_LocalClient_LastInput_Set(LocalClientNum_t localClientNum, LastInput_t currentInput)
{
    int v2; // eax
    int v3; // eax

    if (HIDWORD(qword_1DB4408[3 * localClientNum]) == currentInput)
    {
        resetTime = 0;
    }
    else
    {
        v2 = Sys_Milliseconds();
        if (resetTime)
        {
            if (resetTime <= v2)
            {
                v3 = HIDWORD(qword_1DB4408[3 * localClientNum]);
                resetTime = 0;
                HIDWORD(qword_1DB4408[3 * localClientNum]) = currentInput;
                if (!v3 || currentInput == LAST_INPUT_GAMEPAD)
                    CG_UpdateVehicleBindings(localClientNum);
            }
        }
        else
        {
            resetTime = v2 + 200;
        }
    }
}

LocalClientNum_t Com_LocalClients_GetPrimary()
{
    LocalClientNum_t result;

    result = primaryLocalClient;
    if (primaryLocalClient == -1)
    {
        if (!assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(primaryLocalClient != INVALID_LOCAL_CLIENT)",
            "%s",
            "Someone asked for the primary client and it was not set\n"))
            __debugbreak();
        result = primaryLocalClient;
    }
    return result;
}
