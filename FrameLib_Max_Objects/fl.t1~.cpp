
#include "FrameLib_Sink.h"
#include "FrameLib_Objects.h"
#include "FrameLib_DSP.h"

class FrameLib_T1 : public FrameLib_Processor
{
    enum AttributeList {kInputs};

public:
    
    FrameLib_T1(DSPQueue *queue, FrameLib_Attributes::Serial *serialisedAttributes) : FrameLib_Processor(queue)
    {
        mAttributes.addDouble(kInputs, "outputs", 1, 0);
        
        mAttributes.set(serialisedAttributes);
            
        setIO(mAttributes.getValue(kInputs), 2);
    }
        
protected:
    
    void process()
    {
        unsigned long numOuts = getNumOuts();
        
        unsigned long size_out = 3512 + (rand() % 1512);

        for (unsigned int i = 0; i < numOuts; i++)
        {
            requestOutputSize(i, size_out);
        }
         
        allocateOutputs();
        
        for (unsigned int i = 0; i < numOuts; i++)
        {
            unsigned long size;
            double *output = getOutput(i, &size);
            
            for (unsigned int j = 0; j < size; j++)
            {
                double mul = j * 2.0 / size;
                
                mul = mul > 1.0 ? 2.0 - mul : mul;
                
                output[j] = mul * sin((i + 1) * j * 250.0 * M_PI * 2.0 / mSamplingRate);
            }
        }
    }
};

//#define OBJECT_CLASS FrameLib_Expand<FrameLib_Exp>
#define OBJECT_CLASS FrameLib_Expand <FrameLib_T1>
#define OBJECT_NAME "fl.t1~"

#include "Framelib_Max.h"