#include "win_common.h"
#include "assertive.h"

void Sys_EnterCriticalSection(CriticalSection critSect) {
#ifdef _DEBUG
    if ((unsigned int)critSect >= 0x4D
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(critSect) < (unsigned)(CRITSECT_COUNT)",
            "critSect doesn't index CRITSECT_COUNT\n\t%i not in [0, %i)",
            critSect,
            77))
    {
        __debugbreak();
    }
#endif // _DEBUG
    EnterCriticalSection((LPCRITICAL_SECTION)(24 * critSect + 128634080));
}

void Sys_LeaveCriticalSection(CriticalSection critSect) {
#ifdef _DEBUG
    if ((unsigned int)critSect >= 0x4D
        && !(unsigned __int8)assertive::Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(unsigned)(critSect) < (unsigned)(CRITSECT_COUNT)",
            "critSect doesn't index CRITSECT_COUNT\n\t%i not in [0, %i)",
            critSect,
            77))
    {
        __debugbreak();
    }
#endif
}