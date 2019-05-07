
#include "ascreencap.h"
#include "ascreencap-ABitmapLite.h"
#include "ascreencap-AScreenCap.h"
#include <ui/PixelFormat.h>

namespace ACapture
{

AScreenCap::AScreenCap()
    : _adata({}), _sc(nullptr), _dsp({}), _err(0)
{
    if (!(_sc = new android::ScreenshotClient()))
    {
        __ERROR_THIS_SET;
    }
    else
        if ((_dsp = android::SurfaceComposerClient::getBuiltInDisplay(
                    android::ISurfaceComposer::eDisplayIdMain
                    )) == nullptr)
            __ERROR_THIS_SET;
}

AScreenCap::~AScreenCap()
{
    if (_sc)
    {
        _sc->release();
        delete _sc;
    }
    _sc = nullptr;

    _adata.~ABitmapLite();

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
    _adata.nohead = b;
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
#       if defined(_DEBUG)
        __LOG_PRINT("-> error: %s", fname.c_str());
#       endif
    __ERROR_BOOL_SET;
}

bool AScreenCap::printStdout(bool ispack, int32_t fast)
{
    errno = 0;
    size_t  _psz = 0U;
    uint8_t *_dst = ((ispack) ? _adata.GetDataPack(&_psz, fast) : _adata.GetData(&_psz));

    if ((!_psz) || (!_dst))
        __ERROR_BOOL_SET;

    fwrite(_dst, 1U, _psz, stdout);
    return true;
}

bool AScreenCap::getScreen()
{
    _err = 0;
    errno = 0;

    do
    {
        _adata.Reset();

        if (_sc->update(_dsp, android::Rect(0, 0), false) != android::NO_ERROR)
            __ERROR_BREAK_SET;

        errno = 0;

        _adata.SetData(
            _sc->getWidth(),
            _sc->getHeight(),
            _sc->getStride(),
            _sc->getFormat(),
            _sc->getPixels(),
            _sc->getSize()
            );

        if (!_adata.TestData(true))
            __ERROR_BREAK_SET;
    }
    while (0);

    if (_err)
        _adata.Reset();

    _sc->release();
    return (!_err);
}

void AScreenCap::getStream(int32_t fast)
{
    do
    {
        if ((!getScreen()) || (getError()))
            continue;

        size_t _psz = 0U;
        uint8_t *_dst = _adata.GetDataPack(&_psz, fast);

        if ((_dst) && (_psz))
            fwrite(_dst, 1, _psz, stdout);
    }
    while (true);
}

}
