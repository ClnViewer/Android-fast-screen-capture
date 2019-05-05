
#include "ascreencap.h"
#include "ascreencap-AScreenConf.h"
#include "extern/argh.h"

#define __CONF_CAPFILE    "-f", "--file"
#define __CONF_CAPPACK    "-p", "--pack"
#define __CONF_CAPRATIO   "",   "--ratio"
#define __CONF_CAPORIENT  "",   "--rotate"
#define __CONF_CAPSTREAM  "-s", "--stream"

namespace ACapture
{

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

AScreenConf::AScreenConf(int32_t argc, char **argv)
    : _err(0),
      IsCapStream(false), IsCapFile(false), IsCapRatio(false), IsCapOrien(false), IsPackFile(false),
      Ratio(0U), Rotate(0U), FastPack(0U)
{
    argh::parser lcmd({
            __CONF_CAPSTREAM,
            __CONF_CAPFILE,
            __CONF_CAPPACK,
            __CONF_CAPRATIO,
            __CONF_CAPORIENT
            });

    lcmd.parse(argc, argv);

    size_t sp;
    std::string sratio;
    std::string sorient;
    std::string spack;

    IsCapStream = (lcmd[{ __CONF_CAPSTREAM }]);
    IsCapFile   = !(!(lcmd({ __CONF_CAPFILE })   >> FileName));
    IsCapRatio  = !(!(lcmd({ __CONF_CAPRATIO })  >> sratio));
    IsCapOrien  = !(!(lcmd({ __CONF_CAPORIENT }) >> sorient));
    bool isPack = !(!(lcmd({ __CONF_CAPPACK })   >> spack));

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

    if ((IsCapOrien) && (sorient.length()))
    {
        switch ((Rotate = strToUint(sorient)))
        {
            case 90:
            case 180:
            case 270:
            case 360: /// mirror mode
                break;
            default:
                {
                    IsCapOrien =  false;
                    Rotate = 0U;
                }
        }
    }

    if ((isPack) && (spack.length()))
    {
        FastPack = strToUint(spack);
        FastPack = ((FastPack > 9) ? 0 : (9 - FastPack));
    }

    IsCapFile = ((IsCapFile) ? IsCapFile : (!IsCapStream));
    IsCapStream = ((IsCapFile) ? false : IsCapStream);

#       if defined(_DEBUG)
        __LOG_PRINT("CONFIG SET: -> %u/%u/%u [%d/%d/%d/%d/%d] (%s)",
            Ratio, Rotate, FastPack,
            IsCapStream, IsCapFile, IsCapRatio, IsCapOrien, IsPackFile,
            FileName.c_str()
                    );
#       endif

}

AScreenConf::~AScreenConf()
{
}

}
