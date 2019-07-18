
#include "libfake.h"

/// typedef void native_handle_t;
/// typedef uint8_t HandleWrapMethod;

namespace android {

class IBinder;
class IGraphicBufferProducer;
class DisplayInfo;

/// TODO: add
//class String8;
//class Vector;

/// declaration -> libinder ?
class Rect
{
public:
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
    Rect();
    Rect(Rect const &);
    Rect(int32_t, int32_t);
    Rect(int32_t, int32_t, int32_t, int32_t);
    ~Rect();
};

/// declaration -> libui
class GraphicBuffer
{
public:
    GraphicBuffer(uint32_t, uint32_t, int32_t, uint32_t, uint64_t, std::string);
    GraphicBuffer(uint32_t, uint32_t, int32_t, uint32_t, uint32_t, uint32_t, void*, bool);
    GraphicBuffer(const void*, uint8_t, uint32_t, uint32_t, int32_t, uint32_t, uint64_t, uint32_t);

    uint32_t unlock();
    uint32_t lock(uint32_t, void**);
    uint32_t lock(uint32_t, const Rect&, void**);

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint32_t getStride() const;
    uint64_t getUsage() const;
    int32_t  getPixelFormat() const;
    uint32_t getLayerCount() const;
    Rect     getBounds() const;
    uint64_t getId() const;

private:
    ~GraphicBuffer();
};


/// realisation
class SurfaceComposerClient
{
public:

    SurfaceComposerClient() {}
    virtual ~SurfaceComposerClient() {}

    /*
    /// TODO: add
    static sp<IBinder> createDisplay(const String8& displayName, bool secure);
    static int32_t getDisplayConfigs(const sp<IBinder>& display, Vector<DisplayInfo>* v) { return 0; }
    static int32_t getAnimationFrameStats(FrameStats* outStats) { return 0; }
    int32_t        setTransparentRegionHint(const sp<IBinder>& id, const Region& transparent) { return 0; }
    */
    static sp<IBinder> getBuiltInDisplay(int32_t);
    static void        destroyDisplay(const sp<IBinder>&);
    static int32_t     getDisplayInfo(const sp<IBinder>&, DisplayInfo*);
    static int         getActiveConfig(const sp<IBinder>&);
    static int32_t     setActiveConfig(const sp<IBinder>&, int);
    static void        setDisplayPowerMode(const sp<IBinder>&, int);
    static void        openGlobalTransaction();
    static void        closeGlobalTransaction(bool);
    static void        setAnimationTransaction();
    static int32_t     clearAnimationFrameStats();

    static void setDisplaySurface(const sp<IBinder>&, const sp<IGraphicBufferProducer>&);
    static void setDisplayLayerStack(const sp<IBinder>&, uint32_t);
    static void setDisplaySize(const sp<IBinder>&, uint32_t, uint32_t);
    static void setDisplayProjection(const sp<IBinder>&, uint32_t, const Rect&, const Rect&);

    int32_t    hide(const sp<IBinder>&);
    int32_t    show(const sp<IBinder>&);
    int32_t    setFlags(const sp<IBinder>&, uint32_t, uint32_t);
    int32_t    setLayer(const sp<IBinder>&, uint32_t);
    int32_t    setAlpha(const sp<IBinder>&, float);
    int32_t    setMatrix(const sp<IBinder>&, float, float, float, float);
    int32_t    setPosition(const sp<IBinder>&, float, float);
    int32_t    setSize(const sp<IBinder>&, uint32_t, uint32_t);
    int32_t    setCrop(const sp<IBinder>&, const Rect&);
    int32_t    setLayerStack(const sp<IBinder>&, uint32_t);
    int32_t    destroySurface(const sp<IBinder>&);
    int32_t    clearLayerFrameStats(const sp<IBinder>&) const;
    int32_t    getLayerFrameStats(const sp<IBinder>&, void*) const;

};

sp<IBinder> SurfaceComposerClient::getBuiltInDisplay(int id) {
          LOG("SurfaceComposerClient::getBuiltInDisplay(%d)\n", id);
          sp<IBinder> p;
          p.m_ptr = NULL;
          return p;
    }
void    SurfaceComposerClient::destroyDisplay(const sp<IBinder>& b) {
          LOG("SurfaceComposerClient::destroyDisplay()\n");
    }
int32_t SurfaceComposerClient::getDisplayInfo(const sp<IBinder>& display, DisplayInfo* info) {
          LOG("SurfaceComposerClient::getDisplayInfo()\n");
          return 0;
    }
int32_t SurfaceComposerClient::getActiveConfig(const sp<IBinder>& display) {
          LOG("SurfaceComposerClient::getActiveConfig()\n");
          return 0;
    }
int32_t SurfaceComposerClient::setActiveConfig(const sp<IBinder>& display, int id) {
          LOG("SurfaceComposerClient::setActiveConfig()\n");
          return 0;
    }
void    SurfaceComposerClient::setDisplayPowerMode(const sp<IBinder>& display, int mode) {
          LOG("SurfaceComposerClient::setDisplayPowerMode()\n");
    }
void    SurfaceComposerClient::openGlobalTransaction() {
          LOG("SurfaceComposerClient::openGlobalTransaction()\n");
    }
void    SurfaceComposerClient::closeGlobalTransaction(bool synchronous) {
          LOG("SurfaceComposerClient::closeGlobalTransaction()\n");
    }
void    SurfaceComposerClient::setAnimationTransaction() {
          LOG("SurfaceComposerClient::setAnimationTransaction()\n");
    }
int32_t SurfaceComposerClient::clearAnimationFrameStats() {
          LOG("SurfaceComposerClient::clearAnimationFrameStats()\n");
          return 0;
    }
void    SurfaceComposerClient::setDisplaySurface(const sp<IBinder>& token, const sp<IGraphicBufferProducer>& bufferProducer) {
          LOG("SurfaceComposerClient::setDisplaySurface()\n");
    }
void    SurfaceComposerClient::setDisplayLayerStack(const sp<IBinder>& token, uint32_t layerStack) {
          LOG("SurfaceComposerClient::setDisplayLayerStack()\n");
    }
void    SurfaceComposerClient::setDisplaySize(const sp<IBinder>& token, uint32_t width, uint32_t height) {
          LOG("SurfaceComposerClient::setDisplaySize()\n");
    }
void    SurfaceComposerClient::setDisplayProjection(const sp<IBinder>& token, uint32_t orientation, const Rect& layerStackRect, const Rect& displayRect) {
          LOG("SurfaceComposerClient::setDisplayProjection()\n");
    }
int32_t    SurfaceComposerClient::hide(const sp<IBinder>& id) { return 0; }
int32_t    SurfaceComposerClient::show(const sp<IBinder>& id) { return 0; }
int32_t    SurfaceComposerClient::setFlags(const sp<IBinder>& id, uint32_t flags, uint32_t mask) { return 0; }
int32_t    SurfaceComposerClient::setLayer(const sp<IBinder>& id, uint32_t layer) { return 0; }
int32_t    SurfaceComposerClient::setAlpha(const sp<IBinder>& id, float alpha) { return 0; }
int32_t    SurfaceComposerClient::setMatrix(const sp<IBinder>& id, float dsdx, float dtdx, float dsdy, float dtdy) { return 0; }
int32_t    SurfaceComposerClient::setPosition(const sp<IBinder>& id, float x, float y) { return 0; }
int32_t    SurfaceComposerClient::setSize(const sp<IBinder>& id, uint32_t w, uint32_t h) { return 0; }
int32_t    SurfaceComposerClient::setCrop(const sp<IBinder>& id, const Rect& crop) { return 0; }
int32_t    SurfaceComposerClient::setLayerStack(const sp<IBinder>& id, uint32_t layerStack) { return 0; }
int32_t    SurfaceComposerClient::destroySurface(const sp<IBinder>& id) { return 0; }
int32_t    SurfaceComposerClient::clearLayerFrameStats(const sp<IBinder>& token) const  { return 0; }
int32_t    SurfaceComposerClient::getLayerFrameStats(const sp<IBinder>& token, void* outStats) const { return 0; }


/// realisation
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

    /// < 4.0
    int32_t update ();
    /// 4.0 - 6.0 work
    int32_t update (const sp<IBinder>&, Rect, bool);
    /// 
    int32_t update (const sp<IBinder>&, Rect, uint32_t, uint32_t, bool);
    int32_t update (const sp<IBinder>&, Rect, uint32_t, uint32_t, uint32_t, uint32_t, bool);
    /// 6.0 - 8.1.0 work
    int32_t update (const sp<IBinder>&, Rect, uint32_t, uint32_t, uint32_t, uint32_t, bool, uint32_t);
    /// 6.0
    static int32_t capture(const sp<IBinder>&, const sp<IGraphicBufferProducer>&, Rect, uint32_t, uint32_t, uint32_t, uint32_t, bool);
    /// 9.0
    static int32_t capture(const sp<IBinder>&, Rect, uint32_t, uint32_t, uint32_t, uint32_t, bool, uint32_t, sp<GraphicBuffer>*);

    /*  Call param: 6.0
	update (const sp<IBinder>&, Rect, 0, 0, 0, -1U, false, 0|1|2|3);
    */
    /*  Call param: 8.0
	update (const sp<IBinder>&, Rect, 0, 0, INT32_MIN, INT32_MAX, false, 0|1|2|3);
    */
    /*  Call param: 9.0
	capture(const sp<IBinder>&, Rect, 0, 0, INT32_MIN, INT32_MAX, false, uint32_t, sp<GraphicBuffer>*); 
	  void* base = NULL;
	  result = outBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN, &base);
	    w = outBuffer->getWidth();
	    h = outBuffer->getHeight();
	    s = outBuffer->getStride();
	    f = outBuffer->getPixelFormat();
	    d = HAL_DATASPACE_UNKNOWN;
	    size = s * h * bytesPerPixel(f);
    */


    void const* getPixels() const;
    uint32_t    getWidth() const;
    uint32_t    getHeight() const;
    uint32_t    getStride() const;
    int32_t     getFormat() const;
    size_t      getSize() const;
    void        release();
    /// 6.0 - 8.0
    int32_t     getDataSpace();
};

int32_t ScreenshotClient::update() { LOG("ScreenshotClient::update(%d)\n", __LINE__); return 1; }
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
int32_t ScreenshotClient::capture(const sp<IBinder>& display,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            uint32_t minLayerZ, uint32_t maxLayerZ,
            bool useIdentityTransform, uint32_t rotation,
            sp<GraphicBuffer>* gb) { LOG("ScreenshotClient::capture(%d)\n", __LINE__); return 1; }

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
int32_t ScreenshotClient::getDataSpace() {  /// >= 6.0
    LOG("ScreenshotClient::getSize()\n");
    return 0;
}
void ScreenshotClient::release() {
    LOG("ScreenshotClient::release()\n");
}


};

