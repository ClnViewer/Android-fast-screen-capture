
#include "ascreencap.h"
#include "ascreencap-ABitmapLite.h"
#include "extern/lz4/lz4.h"

namespace ACapture
{

/*
struct _RGB {
    uint8_t r_, g_, b_;
};
*/

ABitmapLite::ABitmapLite()
        : sz(0U), w(0U), h(0U), s(0U), b(0U), f(0U), rat(0U), rot(0U),
          src(nullptr), dst(nullptr), ishead(false), nohead(false) {}

ABitmapLite::ABitmapLite(
    uint32_t _w, uint32_t _h, uint32_t _s, uint32_t _f, const void *_src, size_t _sz
    ) : sz(0U), w(0U), h(0U), s(0U), b(0U), f(0U), rat(0U), rot(0U),
        src(nullptr), dst(nullptr), ishead(false), nohead(false)
    {
        SetData(_w, _h, _s, _f, _src, _sz);
    }

ABitmapLite::~ABitmapLite() {}

bool ABitmapLite::TestData(bool issrc)
    {
        return !(
                 (!w)  ||
                 (!h)  ||
                 (!s)  ||
                 (!sz) ||
                 ((issrc) && (!src))
                );
    }

void ABitmapLite::Reset()
    {
        sz = 0U;
        w = h = s = b = f = bpp = 0U;
        src = dst = nullptr;
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
        w = _w;
        h = _h;
        s = _s;
        f = _f;
        sz = _sz;

        if (TestData(false))
        {
            b = (sz / w / h);
            SetData(_src, _sz);
        }
    }

void ABitmapLite::SetData(const void *_src, size_t _sz)
    {
        uint8_t *l_src = static_cast<uint8_t*>(
            const_cast<void*>(_src)
            );
        vsrc.assign(&l_src[0], &l_src[0] + _sz);
        //std::reverse(vsrc.begin(), vsrc.end());
        sz = _sz;
        src = &vsrc[0];
        dst = nullptr;
    }

uint8_t * ABitmapLite::GetData(size_t *psz)
    {
        do
        {
            *psz = 0U;

            if (!TestData(true))
                break;

            if (!convertBmp())
                break;

            if (!nohead)
                if (!headerBmp())
                    break;

            *psz = ((ishead) ?
                    (sz + sizeof(ABitmapLite::BMPHEADER)) : sz
                );
            return ((ishead) ?
                    &dst[0] : &dst[sizeof(ABitmapLite::BMPHEADER)]
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
                w,
                h
            };
            memcpy(&vdstz[0], &sh, sizeof(sh));

            *psz =  vdstz.size();
            dst  = &vdstz[0];
            return &dst[0];
        }
        while (0);

        *psz = 0U;
        return nullptr;
    }

uint32_t ABitmapLite::getBpp() const
    {
        /// TODO: format normalize
        switch(f)
        {
            /// 32 BPP
            case android::PIXEL_FORMAT_RGBA_8888:
            case android::PIXEL_FORMAT_RGBX_8888:
            case android::PIXEL_FORMAT_BGRA_8888:
            case android::PIXEL_FORMAT_sRGB_A_8888:
            case android::PIXEL_FORMAT_sRGB_X_8888:
                return 32;
            /// 24 BPP
            case android::PIXEL_FORMAT_RGB_888:
                return 24;
            /// 16 BPP
            case android::PIXEL_FORMAT_RGB_565:
            case android::PIXEL_FORMAT_RGBA_5551:
            case android::PIXEL_FORMAT_RGBA_4444:
                return 16;
            default: return 0;
        }
    }

uint32_t ABitmapLite::getPad(uint32_t _w) const
    {
        uint32_t pad = 0U;
        while (((_w + pad) % 4) != 0)
            pad++;
        return pad;
    }

bool ABitmapLite::convertBmp()
    {
        if (!src)
            return false;

	    const uint32_t
                 hd   = h,
                 wr   = ((rat) ? __extension__ (
                                { uint32_t a = (w / rat); if (w % rat) a++; a; }) : w),
                 hr   = ((rat) ? __extension__ (
                                { uint32_t a = (h / rat); if (h % rat) a++; a; }) : h),
                 ar   = ((rat) ? rat : 1U),
                 wsz  = (w * b),
                 wdz  = (wr * 3U),
                 pfmt = getBpp(),
                 dpad = getPad(wdz);

        if (!pfmt)
            return false;

        sz = ((wdz + dpad) * hr);
        w  = ((rat) ? wr : w);
        h  = ((rat) ? hr : h);

        vdst.resize(sz + sizeof(ABitmapLite::BMPHEADER));
        dst = &vdst[0];
        uint8_t *_dst = &vdst[sizeof(ABitmapLite::BMPHEADER)];
	    uint32_t y;

#       pragma omp parallel for private(y) schedule(dynamic)
        for(y = 0U; y < hd; y += ar)
        {
            uint32_t sx = 0U, ssz = (y * wsz),
                     dx = 0U, dsz = (((y / ar) * wdz) + (dpad * y)),
                     ax = (ar * b);

            std::vector<uint8_t> v(wsz);
            v.assign(
                &src[ssz],
                &src[ssz + wsz]
            );
            std::reverse(v.begin(), v.end());

            if ((!rat) && (pfmt == 24))
            {
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
                        _dst[(dsz + dx + 2)] = (255 * (*pixel16 & 0x001F))/32;
                        _dst[(dsz + dx + 1)] = (255 * ((*pixel16 & 0x07E0) >> 5))/64;
                        _dst[(dsz + dx + 0)] = (255 * ((*pixel16 & 0xF800) >> 11))/32;
                        break;
                    }
                    case 24:
                    {
                        memcpy(&_dst[(dsz + dx)], &v[sx], b);
                        break;
                    }
                    case 32:
                    {
                        uint32_t *pixel32 = (uint32_t*)(&v[sx]);
//                        _dst[(dsz + dx + 2)] =  *pixel32 & 0x000000FF;
//                        _dst[(dsz + dx + 1)] = (*pixel32 & 0x0000FF00) >> 8;
//                        _dst[(dsz + dx + 0)] = (*pixel32 & 0x00FF0000) >> 16;
//
                        _dst[(dsz + dx + 2)] =  *pixel32 & 0xFF000000;
                        _dst[(dsz + dx + 1)] = (*pixel32 & 0x00FF0000) >> 8;
                        _dst[(dsz + dx + 0)] = (*pixel32 & 0x0000FF00) >> 16;

                        break;
                    }
                }
            }
        }
        if (rot)
            rotateBmp(rot);

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
                    if (!dst)
                        return;
                    break;
                }
            default:
                return;
        }

        uint32_t y, spad, dpad, ssz;

        switch (angle)
        {
            case 90:
            case 270:
            case 360:
                {
                    spad = getPad(w);
                    dpad = getPad(h);
                    ssz  = (((w * 3U) + dpad) * h);
                    break;
                }
            default:
                {
                    spad = dpad = getPad(w);
                    ssz  = sz;
                    break;
                }
                break;
        }

        std::vector<uint8_t> v(ssz + sizeof(ABitmapLite::BMPHEADER));
        const uint8_t *p0 = &dst[sizeof(ABitmapLite::BMPHEADER)];
        uint8_t       *p1 = &v[sizeof(ABitmapLite::BMPHEADER)];

#       pragma omp parallel for private(y) schedule(dynamic)
        for (y = 0U; y < h; y++)
        {
            uint32_t sx, dx;

            switch (angle)
            {
                case 90:
                    {
                        dx = (h - y - 1) + (y * dpad);
                        sx = ((y) ? ((w + spad) * y) : w);
                        break;
                    }
                case 180:
                    {
                        dx = ((w + dpad) * y);
                        sx = ((h - y - 1) * (w + spad));
                        break;
                    }
                case 270: // TODO:
                    {
                        dx = y + (y * dpad);
                        sx = ((w + spad) * y);
                        break;
                    }
                case 360:
                    {
                        dx = y + (y * dpad);
                        sx = ((w + spad) * y);
                        break;
                    }
                default:
                    {
                        continue;
                    }
            }

            for (uint32_t x = 0U; x < w; x++)
            {
                uint32_t ssz, dsz;

                switch (angle)
                {
                    case 90:
                    case 360:
                        {
                            dsz = (((h * x) + dx) * 3);
                            ssz = ((sx + x) * 3);
                            break;
                        }
                    case 180:
                        {
                            dsz = ((dx + x) * 3);
                            ssz = ((sx + (w - x - 1)) * 3);
                            break;
                        }
                    case 270: // TODO:
                        {
                            dsz = (((h * x) + dx) * 3);
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
        dst = &vdst[0];

        switch (angle)
        {
            case 90:
            case 270:
            case 360:
                {
                    if (w != h)
                        std::swap(w, h);
                    break;
                }
            default:
                break;
        }
    }

bool ABitmapLite::headerBmp()
    {
        ABitmapLite::BMPHEADER bmph{};

        bmph.fh.bfType = 0x4D42; // "BM"
        bmph.fh.bfSize = sz + sizeof(bmph);
        bmph.fh.bfOffBits = sizeof(bmph);
        bmph.ih.biSize = sizeof(bmph.ih);
        bmph.ih.biWidth = static_cast<int32_t>(w);
        bmph.ih.biHeight = static_cast<int32_t>(h);
        bmph.ih.biPlanes = 1U;
        bmph.ih.biBitCount = 24; //(b * 8);
        bmph.ih.biCompression = 0x0000; // BI_RGB
        bmph.ih.biSizeImage = 0;

        /*
            int32_t bpl = (w * b);
            bpl = ((bpl & 0x0003) ? __extension__ ({ bpl |= 0x0003; ++bpl; }) : bpl);
            bmph.fh.bfSize = (bmph.fh.bfOffBits + (bpl * bmph.ih.biHeight));
        */

        if (!memcpy(&dst[0], &bmph, sizeof(bmph)))
            return false;

        ishead = true;

#       if defined(_DEBUG)
        __LOG_PRINT("-> bmph -> %u/%zu", bmph.fh.bfSize, sz);
#       endif
        return true;
    }

}
