
#include "FrameLib_Select.h"

// Constructor

FrameLib_Select::FrameLib_Select(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, FrameLib_Proxy *proxy) : FrameLib_Processor(context, proxy, &sParamInfo)
{
    mParameters.addDouble(kNumIns, "num_ins", 2, 0);
    mParameters.setClip(2, 32);
    mParameters.setInstantiation();
    
    mParameters.addInt(kActiveIn, "input", 0, 1);
    
    mParameters.set(serialisedParameters);
    
    mNumIns = mParameters.getInt(kNumIns);
    mActiveIn = floor(mParameters.getValue(kActiveIn) - 1.0);
    
    setIO(mNumIns, 1);
    
    for (unsigned long i = 0; i < mNumIns; i++)
        setInputMode(i, false, i == mActiveIn, true, kFrameAny);
    
    setOutputType(0, kFrameAny);
    addParameterInput();
}

// Info

std::string FrameLib_Select::objectInfo(bool verbose)
{
    return formatInfo("Selects between a number of input frame streams: The number of inputs is variable. The selected input can be changed with a parameter.",
                   "Selects between a number of input frame streams.", verbose);
}

std::string FrameLib_Select::inputInfo(unsigned long idx, bool verbose)
{
    if (idx == mNumIns)
        return parameterInputInfo(verbose);
    else
        return formatInfo("Input #", "Input #", idx, verbose);
}

std::string FrameLib_Select::outputInfo(unsigned long idx, bool verbose)
{
    return "Output Frames";
}

// Parameter Info

FrameLib_Select::ParameterInfo FrameLib_Select::sParamInfo;

FrameLib_Select::ParameterInfo::ParameterInfo()
{
    add("Sets the number of inputs.");
    add("Sets the current input counting from 1 (or off if out of range).");
}

// Update and Process

void FrameLib_Select::update()
{
    if (mParameters.changed(kActiveIn))
    {        
        mActiveIn = floor(mParameters.getValue(kActiveIn) - 1.0);
        
        for (unsigned long i = 0; i < mNumIns; i++)
            updateTrigger(i, i == mActiveIn);
    }
}

void FrameLib_Select::process()
{
    // Copy active input to output
    
    prepareCopyInputToOutput(mActiveIn, 0);
    allocateOutputs();
    copyInputToOutput(mActiveIn, 0);
}
