#include "devgui.h"

bool DevGui_IsActive()
{
    return DevGuiActive;
}

void DevGui_Toggle()
{
    if (d_menu.child.menu)
    {
        if (handle)
            goto LABEL_15;
#ifdef _DEBUG
        if (DevGuiActive
            && !Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(!devguiGlob.isActive)",
                (const char*)&pBlock))
        {
            __debugbreak();
        }
#endif
        handle = d_menu.child.menu;
        if (d_menu.child.menu)
        {
            DevGui_SelectTopLevelChild();
            if (handle)
            {
            LABEL_15:
                DevGuiActive = DevGuiActive == 0;
                if (DevGuiActive)
                    DevGui_SelectGamepad(0);
            }
        }
    }
#ifdef _DEBUG
    else if (DevGuiActive
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(!devguiGlob.isActive)",
            (const char*)&pBlock))
    {
        __debugbreak();
    }
#endif
}
