
#ifndef FRAMELIB_RESONANT_H
#define FRAMELIB_RESONANT_H

#include "FrameLib_DSP.h"
#include "FrameLib_Filter_Constants.h"

// FIX - All filters to templates
// FIX - time varying params

class FrameLib_Resonant : public FrameLib_Processor
{
    class Resonant
    {
        
    public:
        
        Resonant() : scl(0.0), r2(0.0), y1(0.0), y2(0.0)
        {
        }
        
        void reset()
        {
            y1 = y2 = 0.0;
        }
        
        double HPF(double x)
        {
            return x - calculateFilter(x);
        }
        
        double LPF(double x)
        {
            return calculateFilter(x);
        }
        
        void setParams(double freq, double reson, double samplingRate)
        {
            double frad = cos(freq * M_PI * 2.0 / samplingRate);
            double res = 0.882497 * exp(reson * 0.125);
            
            scl = (frad * res) * -2.0;
            r2 = res * res;
        }
        
    private:
        
        inline double calculateFilter(double x)
        {
            x = x * ((scl + r2) + 1.0);
            double y = x - ((scl * y1) + (r2 * y2));
            
            y2 = y1;
            y1 = y;
            
            return y;
        }
        
        double scl, r2, y1, y2;
    };
    
    enum AttributeList {kFreq, kReson, kMode};
    
    enum Modes {kLPF, kHPF};

public:
	
    FrameLib_Resonant(DSPQueue *queue, FrameLib_Attributes::Serial *serialisedAttributes) : FrameLib_Processor(queue, 1, 1, 0, 0)
    {
        mAttributes.addDouble(kFreq, "freq", 0.0, 0);
        mAttributes.setMin(0.0);
        
        mAttributes.addDouble(kReson, "reson", 0.0, 1);
        mAttributes.setClip(0.0, 1.0);
        
        mAttributes.addEnum(kMode, "mode", 2);
        mAttributes.addEnumItem(kLPF, "lpf");
        mAttributes.addEnumItem(kHPF, "hpf");
        
        mAttributes.set(serialisedAttributes);
    }
    
protected:
    
    void process ()
	{
        Resonant filter;
        Modes mode = (Modes) mAttributes.getValue(kMode);
        
        bool staticParams = TRUE;
        
        double freq = mAttributes.getValue(kFreq);
        double reson = mAttributes.getValue(kReson);
        
        // Get Input
        
        unsigned long sizeIn, sizeOut;
        double *input = getInput(0, &sizeIn);

        requestOutputSize(0, sizeIn);
        allocateOutputs();
        
        double *output = getOutput(0, &sizeOut);
        
        filter.setParams(freq, reson, mSamplingRate);
        
        if (staticParams)
        {
            switch (mode)
            {
                case kLPF:
                    for (unsigned long i = 0; i < sizeOut; i++)
                        output[i] = filter.LPF(input[i]);
                    break;
                    
                case kHPF:
                    for (unsigned long i = 0; i < sizeOut; i++)
                        output[i] = filter.HPF(input[i]);
                    break;
            }
        }
    }
};

#endif