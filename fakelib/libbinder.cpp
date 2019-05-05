

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


class IPCThreadState
{
public:
    static  IPCThreadState* self();
    void    joinThreadPool(bool isMain = true);

private: 
    IPCThreadState();
    ~IPCThreadState();
};

IPCThreadState::IPCThreadState() {
    LOG("IPCThreadState::IPCThreadState()\n");
}
IPCThreadState::~IPCThreadState() {
    LOG("IPCThreadState::IPCThreadState()\n");
}
IPCThreadState * IPCThreadState::self() {
    LOG("IPCThreadState::self()\n");
    return new IPCThreadState();
}
void IPCThreadState::joinThreadPool(bool r) {
    LOG("IPCThreadState::joinThreadPool() %d\n", r);
}

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

