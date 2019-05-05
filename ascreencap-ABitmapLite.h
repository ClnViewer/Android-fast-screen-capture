
#define BMZ_MAGIC 0x315a4d42

namespace ACapture
{

class ABitmapLite
{
    public:
        typedef struct __attribute__((__packed__))
        {
            uint32_t id;
            uint32_t osz;
            uint32_t dsz;
        } STREAMHEADER;

        typedef struct __attribute__((__packed__))
        {
            uint16_t  bfType;
            uint32_t  bfSize;
            uint32_t  bfReserved; // 1,2
            uint32_t bfOffBits;
        } BITMAPFILEHEADER;

        typedef struct __attribute__((__packed__))
        {
            uint32_t biSize;
            long     biWidth;
            long     biHeight;
            uint16_t biPlanes;
            uint16_t biBitCount;
            uint32_t biCompression;
            uint32_t biSizeImage;
            long     biXPelsPerMeter;
            long     biYPelsPerMeter;
            uint32_t biClrUsed;
            uint32_t biClrImportant;
        } BITMAPINFOHEADER;

        typedef struct
        {
            ABitmapLite::BITMAPFILEHEADER fh;
            ABitmapLite::BITMAPINFOHEADER ih;
        } BMPHEADER;

        size_t  sz;
        uint32_t w, h, s, b, f, rat, rot, bpp;
        uint8_t *src;
        uint8_t *dst;
        bool ishead;
        std::vector<uint8_t> vsrc;
        std::vector<uint8_t> vdst;
        std::vector<uint8_t> vdstz;

    ABitmapLite();
    ABitmapLite(uint32_t, uint32_t, uint32_t, uint32_t, const void*, size_t);
    ~ABitmapLite();

    void Reset();
    bool TestData(bool);
    void SetData(const void*, size_t);
    void SetData(uint32_t, uint32_t, uint32_t, uint32_t, const void*, size_t);
    uint8_t * GetData(size_t*);
    uint8_t * GetDataPack(size_t*, int32_t);

    private:

    void rotateBmp(uint32_t);
    bool convertBmp();
    bool headerBmp();
    uint32_t getBpp() const;

};

}
