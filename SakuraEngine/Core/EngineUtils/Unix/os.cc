/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 19:30:30
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 20:02:18
 */
#include "Core/EngineUtils/os.h"
#include <unistd.h>
#if defined(__APPLE__) || defined(SAKURA_TARGET_PLATFORM_OSX)
#include <mach-o/dyld.h>
#endif

namespace Sakura::os
{
    spmr_string env(const char* name)
    {
        char* x = getenv(name);
        return x ? spmr_string(x) : spmr_string();
    }

    spmr_string homedir()
    {
        return os::env("HOME");
    }

    spmr_string cwd()
    {
        char buf[512];
        char* s = getcwd(buf, 4096);
        return s ? spmr_string(s) : spmr_string();
    }

    spmr_string exename()
    {
        spmr_string s = os::exepath();
        return s.substr(s.rfind('/') + 1);
    }

    int pid(void)
    {
        return (int)getpid();
    }

    int cpunum(void)
    {
        static int ncpu = (int)sysconf(_SC_NPROCESSORS_ONLN);
        return ncpu;
    }

#if defined(__linux__) || defined(SAKURA_TARGET_PLATFORM_LINUX)
    spmr_string exepath() {
        char buf[4096] = { 0 };
        int r = (int) readlink("/proc/self/exe", buf, 4096);
        return r > 0 ? spmr_string(buf, r) : spmr_string();
    }
#else
    spmr_string exepath() {
        char buf[4096] = { 0 };
        uint32_t size = sizeof(buf);
        int r = _NSGetExecutablePath(buf, &size);
        return r == 0 ? spmr_string(buf) : spmr_string();
    }
#endif
}
