#include "con_channels.h"

#include "../universal/assertive.h"

bool Con_IsChannelVisible(print_msg_dest_t dest, int channel, int errorFlags)
{
    bool result; // al
    print_msg_dest_t v4; // eax
    const unsigned int* v5; // edi
    int v6; // eax

    #ifdef _DEBUG
    if ((channel < 0 || channel >= 256)
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((channel >= 0 && channel < CON_MAX_CHANNELS))",
            "(channel) = %i",
            channel))
    {
        __debugbreak();
    }
    #endif // _DEBUG
    if (!pcGlob.openChannels[channel].name[0])
        return 0;
    v4 = dest;
    if (dest == 1)
    {
        if (channel == 2 || channel == 3 || channel == 5 || channel == 4)
            return 0;
        v4 = CON_DEST_CONSOLE;
    }
    else if (dest)
    {
        goto LABEL_16;
    }
    if (!channel)
        return 1;
LABEL_16:
    v5 = pcGlob.filters[v4];
    result = 1;
    if (!Com_BitCheckAssert(v5, channel, 32))
    {
        if ((v6 = (errorFlags >> 5) & 0x1F, v6 != 3) && v6 != 2 || !Com_BitCheckAssert(v5, 1, 32))
            result = 0;
    }
    return result;
}