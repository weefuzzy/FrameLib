
#include "FrameLib_Tag.h"

// Constructor

FrameLib_Tag::FrameLib_Tag(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, FrameLib_Proxy *proxy) : FrameLib_Processor(context, proxy, &sParamInfo)
{
    char argStr[10];
    char nameStr[10];
    
    mParameters.addInt(kNumIns, "num_ins", 1);
    mParameters.setClip(1, maxNumIns);
    mParameters.setInstantiation();
    
    // Read in once to get number of strings needed
    
    mParameters.setErrorReportingEnabled(false);
    mParameters.set(serialisedParameters);
    mParameters.setErrorReportingEnabled(true);

    // If no number of inputs is specified explicityly then examine the serialised parameters to determine the number needed
    
    if (!mParameters.changed(kNumIns))
    {
        for (int i = 0; i < maxNumIns; i++)
        {
            sprintf(argStr, "%d", i);
            sprintf(nameStr, "tag_%02d", i + 1);
            if (serialisedParameters->find(argStr) != serialisedParameters->end() || serialisedParameters->find(nameStr) != serialisedParameters->end())
                mParameters.set(kNumIns, (long) (i + 1));
        }
    }
    
    // Read number of ins and setup parameters
    
    mNumIns = mParameters.getInt(kNumIns);
    
    for (int i = 0; i < mNumIns; i++)
    {
        sprintf(nameStr, "tag_%02d", i + 1);
        mParameters.addString(kNames + i, nameStr, i);
        mParameters.setInstantiation();
    }
    
    // Read in again to get parameter names
    
    mParameters.set(serialisedParameters);
    
    // Setup IO

    setIO(mNumIns + 1, 1);
    
    setInputMode(mNumIns, false, true, false, kFrameTagged);
    setOutputType(0, kFrameTagged);
}

// Info

std::string FrameLib_Tag::objectInfo(bool verbose)
{
    return formatInfo("Tags vectors with names ready to send to the parameter input of an object, or for routing purposes: "
                      "A variable number of inputs is available, each of which deal will a specific tag. "
                      "The number of inputs is specified either explicitly with a parameter or implicitly by which arguments or tag parameters are present."
                      "The final input takes tagged input which is concatanated with other inputs after tagging. All inputs trigger output.",
                      "Tags vectors with names ready to send to the parameter input of an object or for routing purposes.", verbose);
}

std::string FrameLib_Tag::inputInfo(unsigned long idx, bool verbose)
{
    if (idx == mNumIns)
        return formatInfo("Parameter Input - takes tagged input for concatenation with other inputs", "Parameter Input", verbose);
    else
        return formatInfo("Input for Tag #", "Input  for Tag #", idx, verbose);

}

std::string FrameLib_Tag::outputInfo(unsigned long idx, bool verbose)
{
    return "Tagged Output Frames";
}

// Parameter Info

FrameLib_Tag::ParameterInfo FrameLib_Tag::sParamInfo;

FrameLib_Tag::ParameterInfo::ParameterInfo()
{
    char str[256];
    
    add("Sets the number of inputs (and hence the number of tags).");
    
    for (int i = 0; i < maxNumIns; i++)
    {
        sprintf(str, "Sets the tag for input %d.", i + 1);
        add(str);
    }
}

// Process

void FrameLib_Tag::process()
{    
    const Serial *preTagged = getInput(mNumIns);
    unsigned long sizeIn;
    
    requestOutputSize(0, 0);
    
    for (unsigned long i = 0; i < mNumIns; i++)
    {
        getInput(i, &sizeIn);
        requestAddedOutputSize(0, Serial::calcSize(mParameters.getString(kNames + i), sizeIn));
    }
    
    requestAddedOutputSize(0, Serial::calcSize(preTagged));
    
    if (allocateOutputs())
    {
        FrameLib_Parameters::Serial *output = getOutput(0);
        
        for (unsigned long i = 0; i < mNumIns; i++)
        {
            const double *input = getInput(i, &sizeIn);
            output->write(mParameters.getString(kNames + i), input, sizeIn);
        }
        
        output->write(preTagged);
    }
}
