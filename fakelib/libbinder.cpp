
#include "libfake.h"

namespace android {

class IPCThreadState;

class ProcessState
{
public:
    static   sp<ProcessState> self();
    void     startThreadPool();
    void     spawnPooledThread(bool);
    uint32_t setThreadPoolMaxThreadCount(size_t);

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
void IPCThreadState::joinThreadPool(bool b) {
    LOG("IPCThreadState::joinThreadPool() %d\n", b);
}

sp<ProcessState> ProcessState::self() {
    LOG("ProcessState::self()\n");
    sp<ProcessState> p;
    p.m_ptr = NULL;
    return p;
}

void ProcessState::spawnPooledThread(bool b) {
    LOG("ProcessState::spawnPooledThread() %d\n", b);
}

void ProcessState::startThreadPool() {
    LOG("ProcessState::startThreadPool()\n");
}

uint32_t ProcessState::setThreadPoolMaxThreadCount(size_t z) {
    LOG("ProcessState::setThreadPoolMaxThreadCount() %zu\n", z);
    return 0U;
}

ProcessState::ProcessState() {
    LOG("ProcessState::ProcessState()\n");
}

ProcessState::~ProcessState() {
    LOG("ProcessState::~ProcessState()\n");
}
    
}

