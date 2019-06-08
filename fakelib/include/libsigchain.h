
#if !defined(__LIBSIGNAL_H_)
#define __LIBSIGNAL_H_ 1

# if !defined(JNIEXPORT)
#  define JNIEXPORT  __attribute__ ((visibility ("default")))
# endif
# if !defined(FUNWEAK)
#  define FUNWEAK
# endif
# define ATTR_UNUSED __attribute__ (( __unused__ ))

namespace art {

static constexpr uint64_t SIGCHAIN_ALLOW_NORETURN = 0x1UL;

extern "C" FUNWEAK void ClaimSignalChain(int s ATTR_UNUSED, void* v ATTR_UNUSED) {}
extern "C" FUNWEAK void UnclaimSignalChain(int s ATTR_UNUSED) {}
extern "C" FUNWEAK void InvokeUserSignalHandler(int s ATTR_UNUSED, void* i ATTR_UNUSED, void* v ATTR_UNUSED) {}
extern "C" FUNWEAK void InitializeSignalChain() {}
extern "C" FUNWEAK void EnsureFrontOfChain(int s ATTR_UNUSED, void* v ATTR_UNUSED) {}
extern "C" FUNWEAK void SetSpecialSignalHandlerFn(int s ATTR_UNUSED, bool (*fn)(int, void*, void*) ATTR_UNUSED) {}
extern "C" FUNWEAK void AddSpecialSignalHandlerFn(int s ATTR_UNUSED, void* v ATTR_UNUSED) {}
extern "C" FUNWEAK void RemoveSpecialSignalHandlerFn(int s ATTR_UNUSED, bool (*fn)(int, void*, void*) ATTR_UNUSED) {}

}

#endif
