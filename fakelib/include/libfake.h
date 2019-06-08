
#if !defined(__ANDROID__)
#error "__ANDROID__ build only !"
#endif

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define LOG(...) fprintf(stderr, __VA_ARGS__)
#define ATTR_UNUSED __attribute__ (( __unused__ ))

namespace android {

template <typename T> class sp {
public:
    T* m_ptr;
};
    
}

