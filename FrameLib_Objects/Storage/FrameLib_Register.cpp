
#include "FrameLib_Register.h"

// Constructor

FrameLib_Register::FrameLib_Register(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, void *owner) : FrameLib_Processor(context, 2, 1)
{
    mParameters.addEnum(kMode, "mode", 0);
    mParameters.addEnumItem(kStore, "store");
    mParameters.addEnumItem(kPass, "pass");
    
    mParameters.set(serialisedParameters);
    
    Modes mode = (Modes) mParameters.getInt(kMode);
    
    if (mode == kStore)
        inputMode(1, false, false, false);
    else
        inputMode(1, false, true, false);
}

// Process

void FrameLib_Register::process()
{
    // Get Register Input (we can ignore the first input as it is just a trigger)
    
    unsigned long sizeIn, sizeOut;
    double *input = getInput(1, &sizeIn);
    
    requestOutputSize(0, sizeIn);
    allocateOutputs();
    
    double *output = getOutput(0, &sizeOut);
    
    // Copy to output
    
    memcpy(output, input, sizeOut * sizeof(double));
}