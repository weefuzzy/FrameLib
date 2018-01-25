
#ifndef FRAMELIB_GAUSSIAN_H
#define FRAMELIB_GAUSSIAN_H

#include "FrameLib_DSP.h"
#include "FrameLib_RandGen.h"

class FrameLib_Gaussian : public FrameLib_Processor
{
    // Parameter Enums and Info
    
    enum ParameterList { kMode, kLength, kUnits };
    enum Modes { kRequestedLength, kInLength };
    enum Units { kSamples, kMS, kSeconds };

    struct ParameterInfo : public FrameLib_Parameters::Info { ParameterInfo(); };
    
public:
    
    // Constructor
    
    FrameLib_Gaussian(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, void *owner);
    
    // Info
    
    std::string objectInfo(bool verbose);
    std::string inputInfo(unsigned long idx, bool verbose);
    std::string outputInfo(unsigned long idx, bool verbose);
    
private:
    
    // Helpers
    
    unsigned long getLength();

    // Process
    
    void process();

    // Data
    
    FrameLib_RandGen mRandom;
    
    static ParameterInfo sParamInfo;
};

#endif