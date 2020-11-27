#include "xanim.h"

#include <universal/com_memory.h>

void XAnimFree(XAnimParts* parts)
{
    int size; // [esp+0h] [ebp-14h]
    XAnimNotifyInfo* notify; // [esp+4h] [ebp-10h]
    int i; // [esp+8h] [ebp-Ch]
    __int16 notifyInfoIndex; // [esp+Ch] [ebp-8h]
    unsigned __int16* boneNames; // [esp+10h] [ebp-4h]

    boneNames = parts->names;
    size = (unsigned __int8)parts->boneCount[9];
    for (i = 0; i < size; ++i)
        SL_RemoveRefToString(boneNames[i]);
    notifyInfoIndex = 0;
    notify = parts->notify;
    while (notifyInfoIndex < (signed int)(unsigned __int8)parts->notifyCount)
    {
        SL_RemoveRefToString(notify->name);
        ++notifyInfoIndex;
        ++notify;
    }
}

void XAnimFreeList(XAnim_s* anims)
{
    unsigned int i; // [esp+0h] [ebp-4h]

    if (anims->debugName)
    {
        Hunk_UserFree(g_DebugHunkUser, (void*)anims->debugName);
        anims->debugName = 0;
    }
    if (anims->debugAnimNames)
    {
        for (i = 0; i < anims->size; ++i)
        {
            if (anims->debugAnimNames[i])
            {
                Hunk_UserFree(g_DebugHunkUser, (void*)anims->debugAnimNames[i]);
                anims->debugAnimNames[i] = 0;
            }
        }
        Hunk_UserFree(g_DebugHunkUser, anims->debugAnimNames);
        anims->debugAnimNames = 0;
        Hunk_UserFree(g_DebugHunkUser, anims->wasLoggedIfMissing);
        anims->wasLoggedIfMissing = 0;
    }
}
