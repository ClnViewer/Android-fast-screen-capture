

namespace ACapture
{

class AScreenCap
{
    private:
        ACapture::ABitmapLite _adata;
#       if (__ANDROID_VER__ >= 9)
        android::sp<android::GraphicBuffer> _sc;
#       else
        android::ScreenshotClient _sc;
#       endif
        android::sp<android::IBinder> _dsp;
        std::atomic<int32_t> _err;
        std::atomic<bool> _ready;

        bool    getLoop();
        bool    sysCap();

    public:
        AScreenCap();
        ~AScreenCap();

        void    setRatio(uint32_t);
        void    setRotate(uint32_t);
        void    setNohead(bool);
        int32_t getError() const;
        void    getStream(int32_t);
        bool    getScreen();
        bool    saveFile(std::string const &, bool, int32_t);
        bool    printStdout(bool, int32_t);

};

}
