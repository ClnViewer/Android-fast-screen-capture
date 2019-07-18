
#include "ascreencap.h"
#include "ascreencap-AScreenConf.h"
#include "ascreencap-ABitmapLite.h"
#include "ascreencap-AScreenCap.h"
#include "extern/argh.h"
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>


/*
#include <signal.h>

static struct sigaction sigThis{};

static void exitSignal(int sig)
{
    __LOG_PRINT("exitHandler signal: [%d]", sig);
    exit(1);
}

static bool setSignal()
{
    sigThis.sa_handler = exitSignal;
    sigThis.sa_flags = 0;
    ::sigemptyset(&sigThis.sa_mask);
    ::sigaddset(&sigThis.sa_mask, SIGPIPE);

    return (
        (::sigaction(SIGINT, &sigThis, NULL) == 0) &&
        (::sigprocmask(SIG_BLOCK, &sigThis.sa_mask, NULL) == 0)
        );
}
*/

int main(int argc, char **argv)
{
    int32_t ret = 0;
#   if defined(_DEBUG)
    auto _t1 = HClockNow();
#   endif

#   if (__ANDROID_VER__ >= 8)
    android::ProcessState::self()->setThreadPoolMaxThreadCount(0);
    android::ProcessState::self()->startThreadPool();
#   elif ((__ANDROID_VER__ <= 7) && (__ANDROID_VER__ >= 5))
    android::sp<android::ProcessState> proc(android::ProcessState::self());
    android::ProcessState::self()->startThreadPool();
#   else
#   endif

    /*
    if (!setSignal())
    {
        __LOG_PRINT("Signal not set: [%s]", strerror(errno));
        return 120;
    }
    */

    ACapture::AScreenConf cnf(argc, argv);
    ACapture::AScreenCap sc;

    if (cnf.IsHelp)
    {
        cnf.printHelp();
        return 0;
    }

    sc.setRatio(cnf.Ratio);
    sc.setRotate(cnf.Rotate);
    sc.setNohead(cnf.IsSDL2Compatible);

#   if defined(_DEBUG)
    auto _t2 = HClockNow();
#   endif

    do
    {
        if (cnf.IsCapFile)
        {
            if (sc.getScreen())
            {
#               if defined(_DEBUG)
                auto _t3 = HClockNow();
#               endif
                if (!sc.getError())
                    if (sc.saveFile(cnf.FileName, cnf.IsCapPack, cnf.FastPack))
                    {
#                       if defined(_DEBUG)
                        auto _t4 = HClockNow();
                        auto _d2 = HClockDiff(_t2, _t1);
                        auto _d3 = HClockDiff(_t3, _t2);
                        auto _d4 = HClockDiff(_t4, _t3);
                        auto _d0 = HClockDiff(_t4, _t1);
                        __LOG_PRINT(
                            "End, load (ms.) -> base: [%lld], capture: [%lld], image: [%lld], total: [%lld] | internal-err: [%d]",
                            _d2, _d3, _d4, _d0, sc.getError()
                            );
#                       endif
                        break;
                    }
            }
            __LOG_PRINT("Capture to file error [%d]", sc.getError());
            ret = 127; break;
        }
        else if (cnf.IsCapStdOut)
        {
            if (sc.getScreen())
            {
                if (!sc.getError())
                    if (sc.printStdout(cnf.IsCapPack, cnf.FastPack))
                        break;
            }
            __LOG_PRINT("Capture to stdout error [%d]", sc.getError());
            ret = 127; break;
        }
        else if (cnf.IsCapStream)
        {
            sc.getStream(cnf.FastPack);
        }
    }
    while (0);

    /// ? fuck! long wait..
    //android::IPCThreadState::self()->joinThreadPool();
    return ret;
}
