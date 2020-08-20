#include "blackbox.h"

void blackbox::BB_Alert(const char* filename, int line, const char* type, const char* msg)
{
    const char* v4; // esi
    unsigned int v5; // edi
    error::AddressInfo_s* v6; // eax
    int v7; // ebx
    int v8; // esi
    int v9; // eax
    int v10; // eax
    win_gamepad::ControllerIndex_t v11; // esi
    unsigned int v12; // eax
    BBMsgHeader* v13; // ecx
    int addressCount; // [esp+14h] [ebp-808h]
    char stacktrace[2048]; // [esp+18h] [ebp-804h]

    v4 = msg;
    stacktrace[0] = 0;
    v5 = 0;
    v6 = error::StackTrace_GetAddressInfo(&addressCount);
    if (addressCount <= 0)
        goto LABEL_14;
    v7 = 0;
    if (addressCount <= 0)
        goto LABEL_13;
    v8 = (int)v6->bestLineFilename;
    while (v5 < 0x800)
    {
        v9 = *(_DWORD*)(v8 - 200);
        if (v9)
        {
            if (*(_BYTE*)v8)
            {
                v10 = _snprintf(
                    &stacktrace[v5],
                    2048 - v5,
                    "0x%08x#%d#%s#%s#%s\n",
                    v9,
                    *(_DWORD*)(v8 + 68),
                    v8 - 132,
                    v8,
                    v8 - 196);
            LABEL_10:
                v5 += v10;
                goto LABEL_11;
            }
            if (*(_BYTE*)(v8 - 132))
            {
                v10 = _snprintf(&stacktrace[v5], 2048 - v5, "0x%08x##%s#%s#%s\n", v9, v8 - 132, v8 - 68, v8 - 196);
                goto LABEL_10;
            }
        }
    LABEL_11:
        ++v7;
        v8 += 272;
        if (v7 >= addressCount)
            break;
    }
    v4 = msg;
LABEL_13:
    stacktrace[2047] = 0;
LABEL_14:
    if (!I_strncmp(v4, &byte_C68B68, 1))
        ++v4;
    BB_Print(
        INVALID_CONTROLLER_PORT,
        "alerts",
        "line %d filename %s type %s stacktrace %s message %s",
        line,
        filename,
        type,
        stacktrace,
        v4);
    if (s_blackboxInitialized)
    {
        v11 = Com_LocalClient_GetControllerIndex(0);
        Sys_EnterCriticalSection(CRITSECT_BLACKBOX);
        if (!s_blackboxIsInteresting)
            goto LABEL_27;
        v12 = Sys_Milliseconds();
        v13 = bb_msg_header;
        bb_msg_header->sendTime = v12;
        if (bb_msg.overflow)
            v13->flags |= 1u;
        if (bb_msg.pppHashTable)
            bb_msg.data[bb_msg.pppBitOffset] = bb_msg.pppBits << (8 - LOBYTE(bb_msg.pppBitCount));
        if (dwRecordEvent(v11, DW_EVENT_BLACKBOX, bb_msg.data, bb_msg.cursize))
            LABEL_27:
        BB_SetupMsg();
        Sys_LeaveCriticalSection(CRITSECT_BLACKBOX);
    }
}

char blackbox::BB_CheckThrottle(const char* name)
{
    return 0;
}

unsigned int blackbox::BB_HashString(const char* str, int len)
{
    return 0;
}

void blackbox::BB_Init()
{
}

void blackbox::BB_InitDefinitions()
{
}

void blackbox::BB_MsgInit(bb_msg_t* msg, void* data, int maxsize)
{
}

void blackbox::BB_Notify(bool daily, const char* recipient, const char* msg, bool immediate)
{
}

char blackbox::BB_ParseParameter(const char* string, int len)
{
    return 0;
}

void blackbox::BB_Print(win_gamepad::ControllerIndex_t controllerIndex, const char* name, const char* fmt, ...)
{
}

int blackbox::BB_RegisterHighWaterMark(const char* name)
{
    return 0;
}

void blackbox::BB_ResetHighWaterMark(const char* name)
{
}

void blackbox::BB_ResetHighWaterMarks()
{
}

void blackbox::BB_Send(win_gamepad::ControllerIndex_t controllerIndex, bool force, bool bandwidthLimited)
{
}

void blackbox::BB_Send_f()
{
}

void blackbox::BB_SetHighWaterMark(int idx, unsigned int value)
{
}

void blackbox::BB_SetThrottle(const char* name, int throttle)
{
}

void blackbox::BB_SetupMsg()
{
}

void blackbox::BB_Start()
{
}

void blackbox::BB_Start_f()
{
}

void blackbox::BB_Throttle_f()
{
}

void blackbox::BB_WriteFormatParameters(bb_msg_t* msg, int parameterCount, const char* parameters, char* args)
{
}

void blackbox::BB_WriteInstanceData(win_gamepad::ControllerIndex_t controllerIndex)
{
}

void blackbox::BB_WriteInt8(bb_msg_t* msg, int c)
{
}

void blackbox::BB_WriteString(bb_msg_t* msg, const char* str, int len)
{
}

void blackbox::BB_WriteVarUInt32(bb_msg_t* msg, unsigned int c)
{
}

void blackbox::BB_WriteVarUInt64(unsigned __int64 c)
{
}
