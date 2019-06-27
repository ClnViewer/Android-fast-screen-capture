

namespace ACapture
{

class AScreenConf
{
    private:
        std::atomic<int32_t> _err;

    public:
        bool IsCapStream;
        bool IsCapFile;
        bool IsCapStdOut;
        bool IsCapRatio;
        bool IsCapRotate;
        bool IsCapPack;
        bool IsSDL2Compatible;
        bool IsHelp;
        uint32_t Ratio;
        uint32_t Rotate;
        uint32_t FastPack;
        std::string FileName;

        AScreenConf(int32_t argc, char **argv);
        ~AScreenConf();

        void printHelp();

};

}
