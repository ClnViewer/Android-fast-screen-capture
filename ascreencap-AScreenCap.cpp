
#include "ascreencap.h"
#include "ascreencap-ABitmapLite.h"
#include "ascreencap-AScreenCap.h"
#include <ui/PixelFormat.h>

namespace ACapture
{

AScreenCap::AScreenCap()
    : _adata({}), _sc({}), _dsp({}), _err(0), _ready(false)
{
    _sc = android::ScreenshotClient();
    if ((_dsp = android::SurfaceComposerClient::getBuiltInDisplay(
                android::ISurfaceComposer::eDisplayIdMain
                )) == nullptr)
        __ERROR_THIS_SET;
}

AScreenCap::~AScreenCap()
{
    _sc.release();
}

int32_t AScreenCap::getError() const
{
    return _err;
}

void AScreenCap::setRatio(uint32_t r)
{
    _adata.rat = r;
}

void AScreenCap::setRotate(uint32_t r)
{
    _adata.rot = r;
}

void AScreenCap::setNohead(bool b)
{
    _adata.issdlcompat = b;
}

bool AScreenCap::saveFile(std::string const & fname, bool ispack, int32_t fast)
{
    int32_t fd = -1;
    errno = 0;

    do
    {
        int32_t _len;
        size_t  _psz = 0U;
        uint8_t *_dst = ((ispack) ? _adata.GetDataPack(&_psz, fast) : _adata.GetData(&_psz));

        if ((!_psz) || (!_dst))
            __ERROR_BOOL_SET;

        if ((fd = open(fname.c_str(), O_RDWR | O_CREAT, 0666)) < 0)
            break;
        if ((_len = write(fd, _dst, _psz)) != static_cast<int32_t>(_psz))
            break;

        close(fd);
#       if defined(_DEBUG)
        __LOG_PRINT("Wrote %d/%zu bytes to %s", _len, _psz, fname.c_str());
#       endif
        return true;
    }
    while(0);

    if (fd > -1)
        close(fd);

#   if defined(_DEBUG)
    __LOG_PRINT("-> error: %s", fname.c_str());
#   endif
    __ERROR_BOOL_SET;
}

bool AScreenCap::printStdout(bool ispack, int32_t fast)
{
    errno = 0;
    size_t  _psz = 0U;
    uint8_t *_dst = ((ispack) ? _adata.GetDataPack(&_psz, fast) : _adata.GetData(&_psz));

    if ((!_psz) || (!_dst))
        __ERROR_BOOL_SET;

    __LOG_PRINT("-> printStdout: [%d/%d] %zu", ispack, fast, _psz);
    write(dup(STDOUT_FILENO), _dst, _psz);
    return true;
}

bool AScreenCap::getScreen()
{
    _err = 0;
    errno = 0;

    do
    {
        _adata.Reset();

        if (!sysCap())
            __ERROR_BREAK_SET;

        errno = 0;

        _adata.SetData(
            _sc.getWidth(),
            _sc.getHeight(),
            _sc.getStride(),
            _sc.getFormat(),
            _sc.getPixels(),
            _sc.getSize()
            );

        if (!_adata.TestData(true))
            __ERROR_BREAK_SET;

#       if defined(_DEBUG)
        __LOG_PRINT("-> getScreen -> point:  %ux%u", _sc.getWidth(), _sc.getHeight());
        __LOG_PRINT("-> getScreen -> stride: %u", _sc.getStride());
        __LOG_PRINT("-> getScreen -> format: %u/%u", _sc.getFormat(), _adata.getBpp());
        __LOG_PRINT("-> getScreen -> size:   %u", _sc.getSize());
#       endif

#       if defined(_DEBUG_RAW_FILE)
        FILE *fp;
        static const char *fnameraw = "/data/local/tmp/OutBitmap.raw";
        __LOG_PRINT("-> getScreen -> write debug RAW file: %s", fnameraw);
        if ((fp = fopen(fnameraw, "w")))
        {
            int sraw = fwrite(_sc.getPixels(), 1, _sc.getSize(), fp);
            fclose(fp);
            __LOG_PRINT("-> getScreen -> wrote to file: %d bytes.", sraw);
        }
#       endif

    }
    while (0);

    if (_err)
        _adata.Reset();

    _sc.release();
    return (!_err);
}

void AScreenCap::getStream(int32_t fast)
{
    int32_t fd = dup(STDOUT_FILENO);
    _ready = false;
    std::thread thr
    {
        [=]()
        {
            while(true)
            {
                while(_ready.load())
                    std::this_thread::yield();

                if (sysCap())
                    _ready = true;
            }
        }
    };
    do
    {
        if ((!getLoop()) || (getError()))
            continue;

        size_t _psz = 0U;
        uint8_t *_dst = _adata.GetDataPack(&_psz, fast);

        if ((_dst) && (_psz))
            write(fd, _dst, _psz);
    }
    while (true);

    try
    {
        if (thr.joinable())
            thr.join();
    }
    catch (...) {}
}

bool AScreenCap::sysCap()
{
    _sc.release();
#   if (__ANDROID_API__ >= 21)
    if (_sc.update(_dsp, android::Rect(0, 0), false) != android::NO_ERROR)
#   elif (__ANDROID_API__ >= 17)
    if (_sc.update(_dsp) != android::NO_ERROR)
#   else
    if (_sc.update() != android::NO_ERROR)
#   endif
        return false;
    return true;
}

bool AScreenCap::getLoop()
{
    _err = 0;
    errno = 0;

    do
    {
        _adata.Reset();

        while(!_ready.load())
            std::this_thread::yield();

        _adata.SetData(
            _sc.getWidth(),
            _sc.getHeight(),
            _sc.getStride(),
            _sc.getFormat(),
            _sc.getPixels(),
            _sc.getSize()
        );

        _ready = false;

        if (!_adata.TestData(true))
            __ERROR_BREAK_SET;
    }
    while (0);

    if (_err)
        _adata.Reset();

    return (!_err);
}

}
