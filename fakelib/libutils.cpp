
#include "libfake.h"

namespace android {

class RefBase
{
public:
            void            incStrong(const void* id) const;
            void            decStrong(const void* id) const;
            void            forceIncStrong(const void* id) const;
            int32_t         getStrongCount() const;


    typedef RefBase basetype;

protected:
                            RefBase();
    virtual                 ~RefBase();
    
    enum {
        OBJECT_LIFETIME_STRONG  = 0x0000,
        OBJECT_LIFETIME_WEAK    = 0x0001,
        OBJECT_LIFETIME_MASK    = 0x0001
    };
    
};

RefBase::RefBase() {
    LOG("RefBase::RefBase()\n");
}
RefBase::~RefBase() {
    LOG("RefBase::~RefBase()\n");
}
void RefBase::incStrong(const void* id) const {
    LOG("RefBase::incStrong()\n");
}
void RefBase::decStrong(const void* id) const {
    LOG("RefBase::decStrong()\n");
}
void RefBase::forceIncStrong(const void* id) const {
    LOG("RefBase::forceIncStrong()\n");
}
int32_t RefBase::getStrongCount() const {
    LOG("RefBase::getStrongCount()\n");
    return 0;
}


}
