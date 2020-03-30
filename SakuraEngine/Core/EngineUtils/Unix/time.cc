/*
 * @Author: your name
 * @Date: 2020-02-09 16:02:00
 * @LastEditTime : 2020-02-09 17:00:40
 * @LastEditors  : SaeruHikari
 * @Description: In User Settings Edit
 * @FilePath: /SakuraEngine/Core/EngineUtils/Unix/time.cc
 */
#ifndef SAKURA_TARGET_PLATFORM_UNIX
static_assert(0, "Target platform is not unix but unix implementation linked, check for the makefiles!");
#endif
#include "../time.h"
#include <time.h>
#include <sys/time.h>

using namespace Sakura;

struct _Clk
{
  #ifdef CLOCK_MONOTONIC
    sinline static int64 ms(void)
    {
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        return static_cast<int64>(t.tv_sec) * 1000 + t.tv_nsec / 1000000;
    }
    sinline static int64 us(void)
    {
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        return static_cast<int64>(t.tv_sec) * 1000000 + t.tv_nsec / 1000;
    }
  #else
        static inline int64 ms() {
        struct timeval t;
        gettimeofday(&t, 0);
        return static_cast<int64>(t.tv_sec) * 1000 + t.tv_usec / 1000;
    }

    static inline int64 us() {
        struct timeval t;
        gettimeofday(&t, 0);
        return static_cast<int64>(t.tv_sec) * 1000000 + t.tv_usec;
    }
  #endif  
};

int64 Sakura::now::ms()
{
    return _Clk::ms();
}

int64 Sakura::now::us()
{
    return _Clk::us();
}

sstring Sakura::now::str(const char* fm /*="%Y-%m-%d %H:%M:%S"*/)
{
    time_t x = time(0);
    struct tm t;
    localtime_r(&x, &t);

    char buf[32]; // 32 is big enough in most cases
    size_t r = strftime(buf, 32, fm, &t);
    return sstring(buf, r);
}