
#include "FrameLib_Interval.h"

FrameLib_Interval::FrameLib_Interval(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, FrameLib_Proxy *proxy) : FrameLib_Scheduler(context, proxy, &sParamInfo, 1, 1)
{
    mParameters.addDouble(kInterval, "interval", 64, 0);
    mParameters.setMin(0);
    
    mParameters.addEnum(kUnits, "units", 1);
    mParameters.addEnumItem(kSamples, "samples");
    mParameters.addEnumItem(kMS, "ms");
    mParameters.addEnumItem(kSeconds, "seconds");
    mParameters.addEnumItem(kHz, "hz");
    
    mParameters.set(serialisedParameters);
    
    setParameterInput(0);
    
    calculateInterval();
}

// Info

std::string FrameLib_Interval::objectInfo(bool verbose)
{
    return formatInfo("Schedules frames at regular intervals, which can be adjusted using the interval parameter: The output is an empty frame.",
                   "Schedules frames at regular intervals, which can be adjusted using the interval parameter.", verbose);
}

std::string FrameLib_Interval::inputInfo(unsigned long idx, bool verbose)
{
    return parameterInputInfo(verbose);
}

std::string FrameLib_Interval::outputInfo(unsigned long idx, bool verbose)
{
    return "Empty Trigger Frames";
}

// Parameter Info

FrameLib_Interval::ParameterInfo FrameLib_Interval::sParamInfo;

FrameLib_Interval::ParameterInfo::ParameterInfo()
{
    add("Sets the interval between frames in the units of the units parameter.");
    add("Sets the time units used to set the interval between frames.");
}

// Calculate Interval

void FrameLib_Interval::calculateInterval()
{
    FrameLib_TimeFormat interval = mParameters.getValue(kInterval);
    
    switch (static_cast<Units>(mParameters.getValue(kUnits)))
    {
        case kHz:           interval = hzToSamples(interval);           break;
        case kMS:           interval = msToSamples(interval);           break;
        case kSeconds:      interval = secondsToSamples(interval);      break;
        case kSamples:      break;
    }
    
    if (!interval)
        interval = FrameLib_TimeFormat::smallest();
    
    mInterval = interval;
}

// Update and Schedule

void FrameLib_Interval::update()
{
    if (mParameters.changed(kInterval) || mParameters.changed(kUnits))
        calculateInterval();
}

FrameLib_Interval::SchedulerInfo FrameLib_Interval::schedule(bool newFrame, bool noAdvance)
{
    return SchedulerInfo(mInterval, true, true);
}
