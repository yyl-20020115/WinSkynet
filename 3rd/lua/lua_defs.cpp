#include "lua_defs.h"
#ifdef _WIN32
#include <Windows.h>
pid_t getpid()
{
    return ::GetCurrentProcessId();
}
#endif

