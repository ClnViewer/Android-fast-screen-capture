

namespace ACapture
{

class AScreenCap
{
    private:
        ACapture::ABitmapLite _adata;
        android::ScreenshotClient _sc;
        android::sp<android::IBinder> _dsp;
        std::atomic<int32_t> _err;

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
