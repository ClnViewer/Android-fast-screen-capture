
#include "libfake.h"

namespace android {

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

// __attribute__ ((weak)) 
__attribute__ ((visibility ("hidden"))) Rect::Rect(int32_t x, int32_t y) : left(x), top(y) {}

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

GraphicBuffer::~GraphicBuffer() {}
GraphicBuffer::GraphicBuffer(uint32_t a1, uint32_t a2, int32_t a3, uint32_t a4, uint64_t a5, std::string a6) { LOG("GraphicBuffer::x()\n"); }
GraphicBuffer::GraphicBuffer(uint32_t a1, uint32_t a2, int32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, void*, bool a7) { LOG("GraphicBuffer::x()\n"); }
GraphicBuffer::GraphicBuffer(const void *a1, uint8_t a2, uint32_t a3, uint32_t a4, int32_t a5, uint32_t a6, uint64_t a7, uint32_t a8) { LOG("GraphicBuffer::x()\n"); }
uint32_t GraphicBuffer::unlock()                      { LOG("GraphicBuffer::x()\n"); return 0U; }
uint32_t GraphicBuffer::lock(uint32_t a1, void **a2)  { LOG("GraphicBuffer::x()\n"); return 0U; }
uint32_t GraphicBuffer::lock(uint32_t a1, const Rect& a2, void **a3) { LOG("GraphicBuffer::x()\n"); return 0U; }
uint32_t GraphicBuffer::getWidth() const           { LOG("GraphicBuffer::x()\n"); return 0U; }
uint32_t GraphicBuffer::getHeight() const          { LOG("GraphicBuffer::x()\n"); return 0U; }
uint32_t GraphicBuffer::getStride() const          { LOG("GraphicBuffer::x()\n"); return 0U; }
uint64_t GraphicBuffer::getUsage() const           { LOG("GraphicBuffer::x()\n"); return 0LLU; }
int32_t  GraphicBuffer::getPixelFormat() const     { LOG("GraphicBuffer::x()\n"); return 0; }
uint32_t GraphicBuffer::getLayerCount() const      { LOG("GraphicBuffer::x()\n"); return 0U; }
Rect     GraphicBuffer::getBounds() const          { LOG("GraphicBuffer::x()\n"); return Rect(0, 0); }
uint64_t GraphicBuffer::getId() const              { LOG("GraphicBuffer::x()\n"); return 0LLU; }

};

