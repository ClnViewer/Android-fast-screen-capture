
#include <atomic>
#include <memory>
#include <vector>
#include <string>
#include <thread>


#if defined(_DEBUG)
#   include <chrono>
    typedef std::chrono::high_resolution_clock::time_point TimeWatch;
#   define HClockNow() std::chrono::high_resolution_clock::now()
#   define HClockDiff(A,B) std::chrono::duration_cast<std::chrono::milliseconds>(A - B).count()
#endif

#include <binder/IBinder.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#if defined(_DEBUG)
#   define __LOG_PRINT(A, ...) \
         	__LOG_PRINT_(A, __func__, __LINE__, __VA_ARGS__)

#  if defined(_ANDROID_LOG)
#     define __LOG_PRINT_(A, B, C, ...) \
         __android_log_print(ANDROID_LOG_DEBUG, "ASCREENCAP", "+ [%s:%d] " A, B, C, __VA_ARGS__)
#  else
#     define __LOG_PRINT_(A, B, C,...) \
    	fprintf(stderr, "+ [%s:%d] " A " | global: [%d][%s]\n", B, C, __VA_ARGS__, errno, strerror(errno))
#  endif

#else
#  define __LOG_PRINT(A,...)
#endif

#define __ERROR_THIS_SET \
    _err = __LINE__;

#define __ERROR_BREAK_SET \
    { _err = __LINE__; break; }

#define __ERROR_BOOL_SET \
    { _err = __LINE__; return false; }

#define __NELE(a) (sizeof(a) / sizeof(a[0]))
