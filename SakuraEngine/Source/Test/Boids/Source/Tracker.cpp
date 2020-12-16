#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
//#define TRACY_ENABLE

#include "Tracker/Tracker.h"
#include "tracy/Tracy.hpp"

#include "tracy/common/TracySystem.cpp"
#ifdef TRACY_ENABLE
#ifdef _MSC_VER
#  pragma warning(push, 0)
#endif

#include "tracy/common/tracy_lz4.cpp"
#include "tracy/client/TracyProfiler.cpp"
#include "tracy/client/TracyCallstack.cpp"
#include "tracy/client/TracySysTime.cpp"
#include "tracy/client/TracySysTrace.cpp"
#include "tracy/common/TracySocket.cpp"
#include "tracy/client/tracy_rpmalloc.cpp"
#include "tracy/client/TracyDxt1.cpp"

#ifdef _MSC_VER
#  pragma comment(lib, "ws2_32.lib")
#  pragma comment(lib, "dbghelp.lib")
#  pragma warning(pop)
#endif
#endif

