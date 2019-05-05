
#include "libfake.h"

namespace android {

class IBinder;

class Rect
{
public:
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
    Rect() {}
    Rect(Rect const & r) {}
    Rect(int32_t left, int32_t top) {}
    Rect(int32_t left, int32_t top, int32_t right, int32_t bottom) {}
    ~Rect() {}
};

class ScreenshotClient
{
    /*
    sp<IMemoryHeap> mHeap;
    uint32_t mWidth;
    uint32_t mHeight;
    PixelFormat mFormat;
    char data[64];
    */
public:
    ScreenshotClient();
    ~ScreenshotClient();

    int32_t update(const sp<IBinder>& display,
            Rect sourceCrop, bool useIdentityTransform);
    int32_t update(const sp<IBinder>& display,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            bool useIdentityTransform);
    int32_t update(const sp<IBinder>& display,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            uint32_t minLayerZ, uint32_t maxLayerZ,
            bool useIdentityTransform);
    int32_t update(const sp<IBinder>& display,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            uint32_t minLayerZ, uint32_t maxLayerZ,
            bool useIdentityTransform, uint32_t rotation);


    void const* getPixels() const;
    uint32_t    getWidth() const;
    uint32_t    getHeight() const;
    uint32_t    getStride() const;
    int32_t     getFormat() const;
    size_t      getSize() const;
    void        release();
};

int32_t ScreenshotClient::update(const sp<IBinder>& display,
            Rect sourceCrop, bool useIdentityTransform) { LOG("ScreenshotClient::update(%d)\n", __LINE__); return 1; }
int32_t ScreenshotClient::update(const sp<IBinder>& display,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            bool useIdentityTransform) { LOG("ScreenshotClient::update(%d)\n", __LINE__); return 1; }
int32_t ScreenshotClient::update(const sp<IBinder>& display,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            uint32_t minLayerZ, uint32_t maxLayerZ,
            bool useIdentityTransform) { LOG("ScreenshotClient::update(%d)\n", __LINE__); return 1; }
int32_t ScreenshotClient::update(const sp<IBinder>& display,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            uint32_t minLayerZ, uint32_t maxLayerZ,
            bool useIdentityTransform, uint32_t rotation) { LOG("ScreenshotClient::update(%d)\n", __LINE__); return 1; }

ScreenshotClient::ScreenshotClient() {
    LOG("ScreenshotClient::ScreenshotClient()\n");
}
ScreenshotClient::~ScreenshotClient() {
    LOG("ScreenshotClient::~ScreenshotClient()\n");
}
void const* ScreenshotClient::getPixels() const {
    LOG("ScreenshotClient::getPixels()\n");
    return NULL;
}
uint32_t ScreenshotClient::getWidth() const {
    LOG("ScreenshotClient::getWidth()\n");
    return 0;
}
uint32_t ScreenshotClient::getHeight() const {
    LOG("ScreenshotClient::getHeight()\n");
    return 0;
}
uint32_t ScreenshotClient::getStride() const {
    LOG("ScreenshotClient::getHeight()\n");
    return 0;
}
int32_t ScreenshotClient::getFormat() const {
    LOG("ScreenshotClient::getFormat()\n");
    return 0;
}
size_t ScreenshotClient::getSize() const {
    LOG("ScreenshotClient::getSize()\n");
    return 0;
}
void ScreenshotClient::release() {
    LOG("ScreenshotClient::release()\n");
}


class SurfaceComposerClient
{
public:
    static sp<IBinder> getBuiltInDisplay(int32_t id);
};

sp<IBinder> SurfaceComposerClient::getBuiltInDisplay(int32_t id) {
    LOG("SurfaceComposerClient::getBuiltInDisplay(int32_t id)\n");
    sp<IBinder> p;
    p.m_ptr = NULL;
    return p;
}

}

