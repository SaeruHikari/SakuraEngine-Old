#ifdef _WIN32
#include "../os.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Sakura::os
{
    sstring env(const char* name)
    {
        sstring s(256, '\0');
        DWORD r = GetEnvironmentVariableA(name, &s[0], 256);
        s.resize(r);

        if (r > 256) {
            GetEnvironmentVariableA(name, &s[0], r);
            s.resize(r - 1);
        }

        return s;
    }

    sstring homedir() {
        return os::env("SYSTEMDRIVE") + os::env("HOMEPATH");
    }

    sstring cwd() {
        char buf[264];
        DWORD r = GetCurrentDirectoryA(sizeof(buf), buf);
        return sstring(buf, r);
    }

    sstring exepath() {
        char buf[264]; // MAX_PATH = 260
        DWORD r = GetModuleFileNameA(0, buf, sizeof(buf));
        return sstring(buf, r);
    }

    sstring exename() {
        sstring s = exepath();
        return s.substr(s.rfind('\\') + 1);
    }

    int pid() {
        return (int)GetCurrentProcessId();
    }

    static inline int _Cpunum() {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        return (int)info.dwNumberOfProcessors;
    }

    int cpunum() {
        static int ncpu = _Cpunum();
        return ncpu;
    }
}

#endif