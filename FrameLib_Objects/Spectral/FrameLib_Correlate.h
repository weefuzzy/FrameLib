
#ifndef FRAMELIB_CORRELATE_H
#define FRAMELIB_CORRELATE_H

#include "FrameLib_DSP.h"
#include "FrameLib_Convolution_Tools.h"

class FrameLib_Correlate : public FrameLib_Processor, private Spectral
{
    // Parameter Enums and Info

    enum ParameterList { kMaxLength, kMode, kEdgeMode };
    enum Mode { kReal, kComplex };
    
    struct ParameterInfo : public FrameLib_Parameters::Info { ParameterInfo(); };

public:
	
    // Constructor / Destructor
    
    FrameLib_Correlate(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, void *owner);
    
    // Info
    
    std::string objectInfo(bool verbose);
    std::string inputInfo(unsigned long idx, bool verbose);
    std::string outputInfo(unsigned long idx, bool verbose);

private:

    // Process
    
    void process();
	
private:
    
    // Data
    
    Mode mMode;

    static ParameterInfo sParamInfo;
};

#endif