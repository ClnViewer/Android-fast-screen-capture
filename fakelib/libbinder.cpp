

#include "libfake.h"

namespace android {

class IPCThreadState;

class ProcessState
{
public:
    static  sp<ProcessState> self();
    void    startThreadPool();

private:
    ProcessState();
   ~ProcessState();
};

sp<ProcessState> ProcessState::self() {
    LOG("ProcessState::self()\n");
    sp<ProcessState> p;
    p.m_ptr = NULL;
    return p;
}
void ProcessState::startThreadPool() {
    LOG("ProcessState::startThreadPool()\n");
}
ProcessState::ProcessState() {
    LOG("ProcessState::ProcessState()\n");
}
ProcessState::~ProcessState() {
    LOG("ProcessState::~ProcessState()\n");
}
    
}

