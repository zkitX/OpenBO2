#include "xmodel.h"

#include <universal/assertive.h>

void XModelPartsFree(XModelPartsLoad* modelParts)
{
    int size; // [esp+0h] [ebp-Ch]
    int i; // [esp+4h] [ebp-8h]
    unsigned __int16* boneNames; // [esp+8h] [ebp-4h]

    if (!modelParts
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "modelParts"))
    {
        __debugbreak();
    }
    boneNames = modelParts->boneNames;
    size = (unsigned __int8)modelParts->numBones;
    for (i = 0; i < size; ++i)
        SL_RemoveRefToString(0, boneNames[i]);
}