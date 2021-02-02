#pragma once
#include <assert.h>
#include <stdint.h>

#define SAKURA_ERROR(node,...) printf_s(node, __VA_ARGS__)
#define SAKURA_FATAL(node,...) printf_s(node, __VA_ARGS__)

#if defined(__MINGW32__)
# define SAKURA_ISSUE_BREAK() DebugBreak();
#elif defined(_MSC_VER)
# define SAKURA_ISSUE_BREAK() __debugbreak();
#elif defined(__powerpc64__)
# define SAKURA_ISSUE_BREAK() asm volatile ("tw 31,1,1");
#elif defined(__i386__) || defined(__ia64__) || defined(__x86_64__)
# define SAKURA_ISSUE_BREAK() asm("int $3");
#else
# define SAKURA_ISSUE_BREAK() abort();
#endif

#ifndef NDEBUG
#ifndef SAKURA_ASSERT_ENABLED
#define SAKURA_ASSERT_ENABLED
#endif
#endif

#ifdef SAKURA_ASSERT_ENABLED
#define SAKURA_BREAK() \
  do { \
    SAKURA_FATAL("BREAKPOINT HIT\n\tfile = %s\n\tline=%d\n", __FILE__, __LINE__); \
    SAKURA_ISSUE_BREAK() \
  } while (false)

#define SAKURA_ASSERT(cond) \
  do { \
    if (!(cond)) { \
      SAKURA_FATAL("ASSERTION FAILED\n\tfile = %s\n\tline = %d\n\tcond = %s\n", __FILE__, __LINE__, #cond); \
      SAKURA_ISSUE_BREAK() \
    } \
  } while (false)

#define SAKURA_ASSERT_MSG(cond, ...) \
  do { \
    if (!(cond)) { \
	  SAKURA_FATAL("ASSERTION FAILED\n\tfile = %s\n\tline = %d\n\tcond = %s\n\tmessage = ", __FILE__, __LINE__, #cond); \
      SAKURA_FATAL(__VA_ARGS__); \
      SAKURA_FATAL("\n"); \
      SAKURA_ISSUE_BREAK(); \
    } \
  } while (false)

#define SAKURA_ASSERT_CMD(cond, cmd) \
  do { \
    if (!(cond)) { \
      cmd; \
    } \
  } while (false)


#else
#define SAKURA_BREAK()
#define SAKURA_ASSERT(cond)
#define SAKURA_ASSERT_MSG(cond, ...)
#define SAKURA_ASSERT_CMD(cond, cmd)
#endif
#define SAKURA_CACHELINE_SIZE 64 // TODO: actually determine this.

extern "C"
{
    typedef unsigned int uint;
    typedef unsigned char uchar;
    typedef char char8_t;
    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;
    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    struct pos2d
    {
        uint32 x = 0;
        uint32 y = 0;
    };
    struct pos3d
    {
        uint32 x;
        uint32 y;
        uint32 z;
    };
    struct extent2d
    {
        uint32 width = 0;
        uint32 height = 0;
    };
    struct extent3d
    {
        uint32 width = 0;
        uint32 height = 0;
        uint32 depth = 0;
    };
    struct double4
    {
        union
        {
            struct { double r, g, b, a; }; //rgba view
            struct { double x, y, z, w; }; //xyzw view
        };
    };
}

#ifndef MAX_UINT8
#define MAX_UINT8  ((uint8)  ~((uint8) 0))
#endif
#ifndef MAX_UINT16
#define MAX_UINT16 ((uint16) ~((uint16)0))
#endif
#ifndef MAX_UINT32
#define MAX_UINT32 ((uint32) ~((uint32)0))
#endif
#ifndef MAX_UINT64
#define MAX_UINT64 ((uint64) ~((uint64)0))
#endif
#ifndef MAX_INT8
#define MAX_INT8   ((int8)  (MAX_UINT8  >> 1))
#endif
#ifndef MAX_INT16
#define MAX_INT16  ((int16) (MAX_UINT16 >> 1))
#endif
#ifndef MAX_INT32
#define MAX_INT32  ((int32) (MAX_UINT32 >> 1))
#endif
#ifndef MAX_INT64
#define MAX_INT64  ((int64) (MAX_UINT64 >> 1))
#endif
#ifndef MIN_INT8
#define MIN_INT8   ((int8)  ~MAX_INT8)
#endif
#ifndef MIN_INT16
#define MIN_INT16  ((int16) ~MAX_INT16)
#endif
#ifndef MIN_INT32
#define MIN_INT32  ((int32) ~MAX_INT32)
#endif
#ifndef MIN_INT64
#define MIN_INT64  ((int64) ~MAX_INT64)
#endif