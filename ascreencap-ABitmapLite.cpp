
#include "ascreencap.h"
#include "ascreencap-ABitmapLite.h"
#include "extern/lz4/lz4.h"

#define _PIX_FORMAT32_BMP(A,B) \
    {                       \
        uint32_t pos = A;   \
        _dst[(pos + 2)] =  *(B) & 0x000000FF; \
        _dst[(pos + 1)] = (*(B) & 0x0000FF00) >> 8; \
        _dst[(pos + 0)] = (*(B) & 0x00FF0000) >> 16; \
    }

#define _PIX_FORMAT32_SDL(A,B) \
    {                       \
        uint32_t pos = A;   \
        _dst[(pos + 0)] = (*(B) & 0xFF000000) >> 24; \
        _dst[(pos + 1)] = (*(B) & 0x00FF0000) >> 16; \
        _dst[(pos + 2)] = (*(B) & 0x0000FF00) >> 8;  \
    }

#define _PIX_FORMAT16_BMP(A,B) \
    {                       \
        uint32_t pos = A;   \
        _dst[(pos + 2)] = (255 * (*(B) & 0x001F))/32; \
        _dst[(pos + 1)] = (255 * ((*(B) & 0x07E0) >> 5))/64; \
        _dst[(pos + 0)] = (255 * ((*(B) & 0xF800) >> 11))/32; \
    }

#define _PIX_FORMAT16_SDL(A,B) \
    {                       \
        uint32_t pos = A;   \
        _dst[(pos + 2)] = (255 * ((*(B) & 0xF800) >> 11))/32; \
        _dst[(pos + 1)] = (255 * ((*(B) & 0x07E0) >> 5))/64; \
        _dst[(pos + 0)] = (255 * (*(B) & 0x001F))/32; \
    }


namespace ACapture
{

ABitmapLite::ABitmapLite()
        : ishead(false), issdlcompat(false), rat(0U), rot(0U) {}

ABitmapLite::ABitmapLite(
    uint32_t _w, uint32_t _h, uint32_t _s, uint32_t _f, const void *_src, size_t _sz
    ) : ishead(false), issdlcompat(false), rat(0U), rot(0U)
    {
        SetData(_w, _h, _s, _f, _src, _sz);
    }

ABitmapLite::~ABitmapLite() {}

bool ABitmapLite::TestData(bool issrc)
    {
        return !(
                 (!bmpdata.w)  ||
                 (!bmpdata.h)  ||
                 (!bmpdata.s)  ||
                 (!bmpdata.sz) ||
                 ((issrc) && (!bmpdata.src))
                );
    }

void ABitmapLite::Reset()
    {
        ::memset(&bmpdata, 0, sizeof(bmpdata));
        ishead = false;

        if (vsrc.size())
            vsrc.clear();
        if (vdst.size())
            vdst.clear();
        if (vdstz.size())
            vdstz.clear();
    }

void ABitmapLite::SetData(
    uint32_t _w, uint32_t _h, uint32_t _s, uint32_t _f, const void *_src, size_t _sz
    )
    {
        bmpdata.w = _w;
        bmpdata.h = _h;
        bmpdata.s = _s;
        bmpdata.f = _f;
#       if (__ANDROID_VER__ >= 9)
        _sz = (_s * _h * android::bytesPerPixel(_f));
#       endif
        bmpdata.sz = _sz;

        if (TestData(false))
        {
            bmpdata.b = (bmpdata.sz / bmpdata.w / bmpdata.h);
            SetData(_src, _sz);
        }
    }

void ABitmapLite::SetData(const void *_src, size_t _sz)
    {
        uint8_t *l_src = static_cast<uint8_t*>(
            const_cast<void*>(_src)
            );
        vsrc.assign(&l_src[0], &l_src[0] + _sz);
        bmpdata.sz = _sz;
        bmpdata.src = &vsrc[0];
        bmpdata.dst = nullptr;
    }

uint8_t * ABitmapLite::GetData(size_t *psz)
    {
        do
        {
            *psz = 0U;

            if (!TestData(true))
                break;

            if (!convertBmp(issdlcompat))
                break;

            if (!issdlcompat)
                if (!headerBmp())
                    break;

            *psz = ((ishead) ?
                    (bmpdata.sz + sizeof(ABitmapLite::BMPHEADER)) : bmpdata.sz
                );
            return ((ishead) ?
                    &bmpdata.dst[0] : &bmpdata.dst[sizeof(ABitmapLite::BMPHEADER)]
                );
        }
        while (0);

        return nullptr;
    }

uint8_t * ABitmapLite::GetDataPack(size_t *psz, int32_t fast)
    {
        do
        {
            uint8_t *pdst;

            if (
                (!(pdst = GetData(psz))) ||
                (!*psz)
               )
                break;

            int lzrsz,
                lzbsz = LZ4_compressBound(static_cast<int>(*psz));

            if (!lzbsz)
                break;

            vdstz.resize(lzbsz + sizeof(STREAMHEADER));

            lzrsz = LZ4_compress_fast(
                        (const char*)pdst,
                        (char*)&vdstz[sizeof(STREAMHEADER)],
                        static_cast<int>(*psz),
                        lzbsz,
                        fast
                        );

            if (lzrsz <= 0)
                break;

            vdstz.resize(lzrsz + sizeof(STREAMHEADER));

            STREAMHEADER sh = {
                BMZ_MAGIC,
                static_cast<uint32_t>(*psz),
                static_cast<uint32_t>(lzrsz),
                bmpdata.w,
                bmpdata.h
            };
            memcpy(&vdstz[0], &sh, sizeof(sh));

            *psz =  vdstz.size();
            bmpdata.dst  = &vdstz[0];
            return &bmpdata.dst[0];
        }
        while (0);

        *psz = 0U;
        return nullptr;
    }

uint32_t ABitmapLite::getBpp() const
    {
        /// TODO: format normalize
        switch(bmpdata.f)
        {
            /// * 32 BPP
            case android::PIXEL_FORMAT_RGBA_8888:
            case android::PIXEL_FORMAT_RGBX_8888:
            case android::PIXEL_FORMAT_BGRA_8888:
            /// > 5.1.1 duplicate PIXEL_FORMAT_RGBA_8888
            //case android::PIXEL_FORMAT_sRGB_A_8888:
            /// 5.1.1 AOSP
            //case android::PIXEL_FORMAT_sRGB_X_8888:
                return 32;
            /// * 24 BPP
            case android::PIXEL_FORMAT_RGB_888:
                return 24;
            /// * 16 BPP
            case android::PIXEL_FORMAT_RGB_565:
            case android::PIXEL_FORMAT_RGBA_5551:
            case android::PIXEL_FORMAT_RGBA_4444:
                return 16;
            default: return 0;
        }
    }

uint32_t ABitmapLite::getPadDst(uint32_t _w) const
    {
        uint32_t pad = 0U;
        while (((_w + pad) % 4) != 0)
            pad++;
        return pad;
    }

uint32_t ABitmapLite::getPadSrc() const
    {
        return ((bmpdata.s > bmpdata.w) ?
                static_cast<uint32_t>((bmpdata.s - bmpdata.w) * bmpdata.b) :
                0U
            );
    }

bool ABitmapLite::convertBmp(bool sdlcompat)
    {
        if (!bmpdata.src)
            return false;

	    const uint32_t
                 hd   = bmpdata.h,
                 wr   = ((rat) ? __extension__ (
                                { uint32_t a = (bmpdata.w / rat);
                                  if (bmpdata.w % rat) a++; a; })
                                : bmpdata.w),
                 hr   = ((rat) ? __extension__ (
                                { uint32_t a = (bmpdata.h / rat);
                                  if (bmpdata.h % rat) a++; a; })
                                : bmpdata.h),
                 ar   = ((rat) ? rat : 1U),
                 ax   = (ar * bmpdata.b),
                 br   =  bmpdata.b,
                 wsz  = (bmpdata.w * bmpdata.b),
                 wdz  = (wr * 3U),
                 pfmt = getBpp(),
                 spad = getPadSrc(),
                 dpad = getPadDst(wdz);


        if (!pfmt)
            return false;

        bmpdata.w  = ((rat) ? wr : bmpdata.w);
        bmpdata.h  = ((rat) ? hr : bmpdata.h);
        bmpdata.sz = ((wdz + dpad) * hr);

        if (!sdlcompat)
        {
            std::reverse(vsrc.begin(), vsrc.end());
            bmpdata.src = &vsrc[0];
        }

#       if defined(_DEBUG)
        __LOG_PRINT("-> convertBmp -> point:            %ux%u", bmpdata.w, bmpdata.h);
        __LOG_PRINT("-> convertBmp -> sdl/ratio/rotate: %d/%u/%u", sdlcompat, rat, rot);
        __LOG_PRINT("-> convertBmp -> format/bpp:       %u/%u", pfmt, bmpdata.b);
        __LOG_PRINT("-> convertBmp -> wsz/wdz/pads:     %u/%u/%u/%u", wsz, wdz, spad, dpad);
        __LOG_PRINT("-> convertBmp -> size:             %zu", bmpdata.sz);
#       endif

        vdst.resize(bmpdata.sz + sizeof(ABitmapLite::BMPHEADER));
        bmpdata.dst = &vdst[0];
        uint8_t *_dst = &vdst[sizeof(ABitmapLite::BMPHEADER)];
	    uint32_t y;

#       pragma omp parallel for private(y) schedule(static)
        for(y = 0U; y < hd; y += ar)
        {
            uint32_t sx = 0U, ssz = ((wsz + spad) * y),
                     dx = 0U, dsz = ((y / ar) * (wdz + dpad));

            std::vector<uint8_t> v(wsz + spad);
            v.assign(
                &bmpdata.src[ssz],
                &bmpdata.src[(ssz + wsz + spad)]
            );
            std::reverse(v.begin(), v.end());

            if ((!rat) && (pfmt == 24))
            {
                /// TODO: sdl2 format make
                memcpy(&_dst[dsz], &v[0], v.size());
                continue;
            }

            for (; sx < v.size(); sx += ax, dx += 3U)
            {
                switch (pfmt)
                {
                    case 16:
                    {
                        uint16_t *pixel16 = (uint16_t*)(&v[sx]);
                        if (sdlcompat)
                            _PIX_FORMAT16_SDL((dsz + dx), pixel16)
                        else
                            _PIX_FORMAT16_BMP((dsz + dx), pixel16)
                        break;
                    }
                    case 24:
                    {
                        /// TODO: sdl2 format make
                        memcpy(&_dst[(dsz + dx)], &v[sx], br);
                        break;
                    }
                    case 32:
                    {
                        uint32_t *pixel32 = (uint32_t*)(&v[sx]);
                        if (sdlcompat)
                            _PIX_FORMAT32_SDL((dsz + dx), pixel32)
                        else
                            _PIX_FORMAT32_BMP((dsz + dx), pixel32)
                        break;
                    }
                }
            }
        }
        if (rot)
            rotateBmp(rot);

#       if defined(_DEBUG_RAW_FILE)
        FILE *fp;
        static const char *fnameraw = "/data/local/tmp/OutBmp.raw";
        __LOG_PRINT("-> convertBmp -> write debug RAW file: %s", fnameraw);
        if ((fp = fopen(fnameraw, "w")))
        {
            int sraw = fwrite(_dst, 1, bmpdata.sz, fp);
            fclose(fp);
            __LOG_PRINT("-> convertBmp -> wrote to file: %d/%u bytes.", sraw, bmpdata.sz);
        }
#       endif

        return true;
    }

void ABitmapLite::rotateBmp(uint32_t angle)
    {
        switch (angle)
        {
            case 90:
            case 180:
            case 270:
            case 360: /// mirror mode
                {
                    if (!bmpdata.dst)
                        return;
                    break;
                }
            default:
                return;
        }

        uint32_t y, spad, dpad, ssz;
        const uint32_t wr = bmpdata.w, hr = bmpdata.h;

        switch (angle)
        {
            case 90:
            case 270:
            case 360:
                {
                    spad = getPadDst(wr);
                    dpad = getPadDst(hr);
                    ssz  = (((wr * 3U) + dpad) * hr);
                    break;
                }
            default:
                {
                    spad = dpad = getPadDst(wr);
                    ssz  = bmpdata.sz;
                    break;
                }
                break;
        }

        std::vector<uint8_t> v(ssz + sizeof(ABitmapLite::BMPHEADER));
        const uint8_t *p0 = &bmpdata.dst[sizeof(ABitmapLite::BMPHEADER)];
        uint8_t       *p1 = &v[sizeof(ABitmapLite::BMPHEADER)];

#       pragma omp parallel for private(y) schedule(dynamic)
        for (y = 0U; y < hr; y++)
        {
            uint32_t sx, dx;

            switch (angle)
            {
                case 90:
                    {
                        dx = (hr - y - 1) + (y * dpad);
                        sx = ((y) ? ((wr + spad) * y) : wr);
                        break;
                    }
                case 180:
                    {
                        dx = ((wr + dpad) * y);
                        sx = ((hr - y - 1) * (wr + spad));
                        break;
                    }
                case 270: // TODO:
                    {
                        dx = y + (y * dpad);
                        sx = ((wr + spad) * y);
                        break;
                    }
                case 360:
                    {
                        dx = y + (y * dpad);
                        sx = ((wr + spad) * y);
                        break;
                    }
                default:
                    {
                        continue;
                    }
            }

            for (uint32_t x = 0U; x < wr; x++)
            {
                uint32_t ssz, dsz;

                switch (angle)
                {
                    case 90:
                    case 360:
                        {
                            dsz = (((hr * x) + dx) * 3);
                            ssz = ((sx + x) * 3);
                            break;
                        }
                    case 180:
                        {
                            dsz = ((dx + x) * 3);
                            ssz = ((sx + (wr - x - 1)) * 3);
                            break;
                        }
                    case 270: // TODO:
                        {
                            dsz = (((hr * x) + dx) * 3);
                            ssz = ((sx + x) * 3);
                            break;
                        }
                    default:
                        {
                            continue;
                        }
                }

                p1[dsz + 0] = p0[ssz + 0];
                p1[dsz + 1] = p0[ssz + 1];
                p1[dsz + 2] = p0[ssz + 2];
            }
        }

        vdst.clear();
        vdst.assign(v.begin(), v.end());
        bmpdata.dst = &vdst[0];

        switch (angle)
        {
            case 90:
            case 270:
            case 360:
                {
                    if (bmpdata.w != bmpdata.h)
                        std::swap(bmpdata.w, bmpdata.h);
                    break;
                }
            default:
                break;
        }
    }

bool ABitmapLite::headerBmp()
    {
        if (!bmpdata.sz)
            return false;

        ABitmapLite::BMPHEADER bmph{};

        bmph.fh.bfType = 0x4D42; // "BM"
        bmph.fh.bfSize = bmpdata.sz + sizeof(bmph);
        bmph.fh.bfOffBits = sizeof(bmph);
        bmph.ih.biSize = sizeof(bmph.ih);
        bmph.ih.biWidth = static_cast<int32_t>(bmpdata.w);
        bmph.ih.biHeight = static_cast<int32_t>(bmpdata.h);
        bmph.ih.biPlanes = 1U;
        bmph.ih.biBitCount = 24; //(b * 8);
        bmph.ih.biCompression = 0x0000; // BI_RGB
        bmph.ih.biSizeImage = 0;

        if (!memcpy(&bmpdata.dst[0], &bmph, sizeof(bmph)))
            return false;

        ishead = true;

#       if defined(_DEBUG)
        __LOG_PRINT("-> bmph -> point: %ux%u", bmpdata.w, bmpdata.h);
        __LOG_PRINT("-> bmph -> size:  %u/%zu", bmph.fh.bfSize, bmpdata.sz);
#       endif
        return true;
    }

}
