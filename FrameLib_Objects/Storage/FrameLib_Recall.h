
#ifndef FRAMELIB_RECALL_H
#define FRAMELIB_RECALL_H

#include "FrameLib_DSP.h"

// FIX - Threadsafety (is threadsafe for now because is scoped within same thread for now)

class FrameLib_Recall : public FrameLib_Processor
{
    enum AttributeList {kName};
    
public:
    
    FrameLib_Recall(DSPQueue *queue, FrameLib_Attributes::Serial *serialisedAttributes) : FrameLib_Processor(queue, 1, 1, 0, 0)
    {
        mAttributes.addString(kName, "name", "", 0);
        
        mAttributes.set(serialisedAttributes);
        
        mStorage = mAllocator->registerStorage(mAttributes.getString(kName));
    }
    
    ~FrameLib_Recall()
    {
        mAllocator->releaseStorage(mAttributes.getString(kName));
    }
    
    
protected:
    
    void process ()
    {
        unsigned long sizeOut;
        
        // Ignore input (it is for triggers only)

        requestOutputSize(0, mStorage->getSize());
        allocateOutputs();
            
        double *output = getOutput(0, &sizeOut);

        memcpy(output, mStorage->getData(), sizeOut * sizeof(double));
    }
    
private:
    
    FrameLib_Local_Allocator::Storage *mStorage;
};

#endif