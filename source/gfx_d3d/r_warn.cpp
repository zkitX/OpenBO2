#include "r_warn.h"

void R_WarnInitDvars()
{
    r_warningRepeatDelay = _Dvar_RegisterFloat(
        "r_warningRepeatDelay",
        5.0,
        0.0,
        30.0,
        0,
        "Number of seconds after displaying a \"per-frame\" warning before it will display again");
}
