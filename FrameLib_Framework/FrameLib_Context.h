
#ifndef FRAMELIB_CONTEXT_H
#define FRAMELIB_CONTEXT_H

// Forward Definitions

class FrameLib_Global;
class FrameLib_GlobalAllocator;
class FrameLib_LocalAllocator;

struct FrameLib_Context
{
    FrameLib_Context(FrameLib_Global *global, void *ref) : mGlobal(global), mReference(ref) {}
    
    FrameLib_LocalAllocator *getAllocator();
    
    // FIX - hack for now...
    
    void *getConnectionQueue();
    void *getDSPQueue();
    
    void releaseAllocator();
    void releaseConnectionQueue();
    void releaseDSPQueue();

    
private:
    
    FrameLib_Global *mGlobal;
    void *mReference;
};

#endif