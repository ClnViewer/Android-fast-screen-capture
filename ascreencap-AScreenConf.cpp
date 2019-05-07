
#include "ascreencap.h"
#include "ascreencap-AScreenConf.h"
#include "extern/argh.h"
#include "version.h"

#define __CONF_CAPFILE    "-f", "--file"
#define __CONF_CAPPACK    "-p", "--pack"
#define __CONF_CAPRATIO   "",   "--ratio"
#define __CONF_CAPROTATE  "",   "--rotate"
#define __CONF_CAPSTREAM  "-s", "--stream"
#define __CONF_CAPSTDOUT  "",   "--stdout"
#define __CONF_CAPSDL     "",   "--sdl"
#define __CONF_CAPHELP    "-h", "--help"

#define __HELP_PRINT(A) fprintf(stdout, "\t\t%s\t%s\t%s\n", (A).key1, (A).key2, (A).desc)
#define __HELP_SET_(A,B,C) .key1 = A, .key2 = B, .desc = C
#define __HELP_SET(...) __HELP_SET_(__VA_ARGS__)

namespace ACapture
{

struct help_s
{
    const char *key1;
    const char *key2;
    const char *desc;
};

static struct help_s helps[] =
{
    {
        __HELP_SET(__CONF_CAPFILE, "\t: output save to file, value is name")
    },
    {
        __HELP_SET(__CONF_CAPSTREAM, ": output to loop stream (STDOUT)")
    },
    {
        __HELP_SET(__CONF_CAPSTDOUT, ": output to STDOUT")
    },
    {
        __HELP_SET(__CONF_CAPPACK, "\t: output pack lz4 algorithm")
    },
    {
        __HELP_SET(__CONF_CAPSDL, "\t: output image SDL2 compatible mode: Landscape screen")
    },
    {
        __HELP_SET(__CONF_CAPRATIO, ": image resize ratio, valid scale 1-5")
    },
    {
        __HELP_SET(__CONF_CAPROTATE, ": image rotate: 90,180,270, value 360 = mirror")
    },
    {
        __HELP_SET(__CONF_CAPHELP, "\t: this help screen")
    }
};

static uint32_t strToUint(std::string const opt)
{
    try
    {
        return stoul(opt);
    }
    catch (std::invalid_argument & _ex)
    {
#       if defined(_DEBUG)
        __LOG_PRINT("CONFIG ERROR: -> (%s)", _ex.what());
#       endif
        return 0U;
    }
    catch (std::out_of_range & _ex)
    {
#       if defined(_DEBUG)
       __LOG_PRINT("CONFIG ERROR: -> (%s)", _ex.what());
#       endif
        return 0U;
    }
    catch (...)
    {
        return 0U;
    }
}

void AScreenConf::printHelp()
{
    fprintf(stdout, "\n\tAndroid Screen Capture - v.%s, rev:%s, build: %s.%s.%s\n",
            ACAP_FULLVERSION_STRING, ACAP_SVN_REVISION, ACAP_DATE, ACAP_MONTH, ACAP_YEAR
            );
    fprintf(stdout, "\tascreencap - replaced version of default screencap utility\n");
    fprintf(stdout, "\tgit: https://github.com/ClnViewer/Android-fast-screen-capture.git\n\n\n");
    for (uint32_t i = 0U; i < __NELE(helps); i++)
        __HELP_PRINT(helps[i]);
}

AScreenConf::AScreenConf(int32_t argc, char **argv)
    : _err(0),
      IsCapStream(false), IsCapFile(false), IsCapStdOut(false),
      IsCapRatio(false), IsCapRotate(false), IsPackFile(false), IsNoHeader(false), IsHelp(false),
      Ratio(0U), Rotate(0U), FastPack(0U)
{
    argh::parser lcmd({
            __CONF_CAPSTREAM,
            __CONF_CAPFILE,
            __CONF_CAPPACK,
            __CONF_CAPRATIO,
            __CONF_CAPROTATE,
            __CONF_CAPSTDOUT,
            __CONF_CAPSDL
            });

    lcmd.parse(argc, argv);

    size_t sp;
    std::string sratio;
    std::string srotate;
    std::string spack;

    IsNoHeader  = (lcmd[{ __CONF_CAPSDL }]);
    IsHelp      = (lcmd[{ __CONF_CAPHELP }]);
    IsCapStdOut = (lcmd[{ __CONF_CAPSTDOUT }]);
    IsCapStream = (lcmd[{ __CONF_CAPSTREAM }]);
    IsCapFile   = !(!(lcmd({ __CONF_CAPFILE })   >> FileName));
    IsCapRatio  = !(!(lcmd({ __CONF_CAPRATIO })  >> sratio));
    IsCapRotate  = !(!(lcmd({ __CONF_CAPROTATE }) >> srotate));
    bool isPack = !(!(lcmd({ __CONF_CAPPACK })   >> spack));

    if (!IsCapStdOut)
    {
        if ((!IsCapFile) || (!FileName.length()))
            FileName.assign("ACapture.bmp");
        else
        {
            if ((sp = FileName.find_last_of(".")) != std::wstring::npos)
            {
                std::string ext = FileName.substr(sp, FileName.length() - sp);
                if (!ext.compare(0U, ext.length(), ".bmz"))
                    IsPackFile = true;
            }
        }
        if (FileName.find("/") == std::wstring::npos)
            FileName.insert(0, "/data/local/tmp/");

        IsCapFile = ((IsCapFile) ? IsCapFile : (!IsCapStream));
        IsCapStream = ((IsCapFile) ? false : IsCapStream);
    }
    else
        IsCapStream = IsCapFile = false;

    if ((IsCapRatio) && (sratio.length()))
    {
        Ratio = strToUint(sratio);

        if (Ratio)
        {
            Ratio = ((Ratio > 5U) ? 5U : Ratio);
        }
        else
            IsCapRatio = false;
    }

    if (IsNoHeader)
        Rotate = 360U;
    else
        if ((IsCapRotate) && (srotate.length()))
        {
            switch ((Rotate = strToUint(srotate)))
            {
                case 90:
                case 180:
                case 270:
                case 360: /// mirror mode
                    break;
                default:
                {
                    IsCapRotate =  false;
                    Rotate = 0U;
                    break;
                }
            }
        }

    if ((isPack) && (spack.length()))
    {
        FastPack = strToUint(spack);
        FastPack = ((FastPack > 9) ? 0 : (9 - FastPack));
    }

#   if defined(_DEBUG)
    __LOG_PRINT("CONFIG SET: ->\n" \
                "\tFile path:\t[%s]\n" \
                "\tRatio:\t\t[%u]\n" \
                "\tRotate:\t[%u]\n" \
                "\tFastPack:\t[%u]\n" \
                "\tIsNoHeader:\t[%d]\n" \
                "\tIsCapStdOut:\t[%d]\n" \
                "\tIsCapStream:\t[%d]\n" \
                "\tIsCapFile:\t[%d]\n" \
                "\tIsCapRatio:\t[%d]\n" \
                "\tIsCapRotate:\t[%d]\n" \
                "\tIsPackFile:\t[%d]\n",
        FileName.c_str(),
        Ratio, Rotate, FastPack,
        IsNoHeader, IsCapStdOut, IsCapStream, IsCapFile, IsCapRatio, IsCapRotate, IsPackFile
    );
#   endif

}

AScreenConf::~AScreenConf()
{
}

}
