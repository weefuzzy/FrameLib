
#include "FrameLib_Window.h"

#define W_PI			3.14159265358979323846
#define W_TWOPI			6.28318530717958647692
#define W_THREEPI		9.42477796076937971538
#define W_FOURPI		12.56637061435817295384
#define W_SIXPI			18.84955592153875943076

// Constructor / Destructor

FrameLib_Window::FrameLib_Window(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, void *owner) : FrameLib_Processor(context, 2, 1)
{
    mParameters.addEnum(kWindowType, "window", 0);
    mParameters.addEnumItem(kHann, "hann");
    mParameters.addEnumItem(kHamming, "hamming");
    mParameters.addEnumItem(kTriangle, "triangle");
    mParameters.addEnumItem(kCosine, "cosine");
    mParameters.addEnumItem(kBlackman, "blackman");
    mParameters.addEnumItem(kBlackman62, "blackman62");
    mParameters.addEnumItem(kBlackman70, "blackman70");
    mParameters.addEnumItem(kBlackman74, "blackman74");
    mParameters.addEnumItem(kBlackman92, "blackman92");
    mParameters.addEnumItem(kBlackmanHarris, "blackmanharris");
    mParameters.addEnumItem(kFlatTop, "flattop");
    mParameters.addEnumItem(kRectangle, "rectangle");
    
    mParameters.addDouble(kSize, "size", 0, 1);
    mParameters.setMin(0.0);
    
    mParameters.addBool(kSqrt, "sqrt", false, 2);
    
    mParameters.addEnum(kCompensation, "compensation");
    mParameters.addEnumItem(kOff, "off");
    mParameters.addEnumItem(kLinear, "linear");
    mParameters.addEnumItem(kPower, "power");
    mParameters.addEnumItem(kPowerOverLinear, "powoverlin");
    
    mParameters.addEnum(kEndPoints, "endpoints");
    mParameters.addEnumItem(kFirst, "first");
    mParameters.addEnumItem(kLast, "last");
    mParameters.addEnumItem(kBoth, "both");
    mParameters.addEnumItem(kNone, "none");
    
    mParameters.set(serialisedParameters);
    
    mWindow = NULL;
    mWindowType = kHann;
    mSize = 0;
    mSqrtWindow = false;
    mLinearGain = 0.0;
    mPowerGain = 0.0;
    
    inputMode(1, true, false, false, kFrameTagged);
}

FrameLib_Window::~FrameLib_Window()
{
    mAllocator->dealloc(mWindow);
}

// Helpers

void FrameLib_Window::updateWindow (unsigned long inSize, EndPoints ends)
{
    WindowTypes windowType = (WindowTypes) mParameters.getInt(kWindowType);
    bool sqrtWindow = mParameters.getBool(kSqrt);
    unsigned long windowSize = mParameters.getInt(kSize);
    
    windowSize = !windowSize ? inSize : windowSize;
    
    windowSize = ends == kBoth ? windowSize - 1 : windowSize;
    windowSize = ends == kNone ? windowSize + 1 : windowSize;
    
    if (windowSize == mSize && windowType == mWindowType && sqrtWindow == mSqrtWindow)
        return;
    
    if (mSize != windowSize)
    {
        mAllocator->dealloc(mWindow);
        mWindow = (double *) mAllocator->alloc(sizeof(double) * (windowSize + 2));
    }
    
    switch (windowType)
    {
        case kHann:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = 0.5 - (0.5 * cos(W_TWOPI * ((double) i / (double) windowSize)));
            break;
            
        case kHamming:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = 0.54347826 - (0.45652174 * cos(W_TWOPI * ((double) i / (double) windowSize)));
            break;
            
        case kTriangle:
            for (unsigned long i = 0; i <= (windowSize >> 1); i++)
                mWindow[i] = (double) i / (double) (windowSize / 2.0);
            for (unsigned long i = (windowSize >> 1) + 1; i <= windowSize; i++)
                mWindow[i] = (double) (((double) windowSize - 1.0) - (double) i) / (double) (windowSize / 2.0);
            break;
            
        case kCosine:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = sin(W_PI * ((double) i / (double) windowSize));
            break;
            
        case kBlackman:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = 0.42659071 - (0.49656062 * cos(W_TWOPI * ((double) i / (double) windowSize))) + (0.07684867 * cos(W_FOURPI * ((double) i / (double) windowSize)));
            break;
            
        case kBlackman62:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = (0.44859f - 0.49364f * cos(W_TWOPI * ((double) i / (double) windowSize)) + 0.05677f * cos(W_FOURPI * ((double) i / (double) windowSize)));
            break;
            
        case kBlackman70:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = (0.42323f - 0.49755f * cos(W_TWOPI * ((double) i / (double) windowSize)) + 0.07922f * cos(W_FOURPI * ((double) i / (double) windowSize)));
            break;
            
        case kBlackman74:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = (0.402217f - 0.49703f * cos(W_TWOPI * ((double) i / (double) windowSize)) + 0.09892f * cos(W_FOURPI * ((double) i / (double) windowSize)) - 0.00188 * cos(W_THREEPI * ((double) i / (double) windowSize)));
            break;
            
        case kBlackman92:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = (0.35875f - 0.48829f * cos(W_TWOPI * ((double) i / (double) windowSize)) + 0.14128f * cos(W_FOURPI * ((double) i / (double) windowSize)) - 0.01168 * cos(W_THREEPI * ((double) i / (double) windowSize)));
            break;
            
        case kBlackmanHarris:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = 0.35875 - (0.48829 * cos(W_TWOPI * ((double) i / (double) windowSize))) + (0.14128 * cos(W_FOURPI * ((double) i / (double) windowSize))) - (0.01168 * cos(W_SIXPI * ((double) i / (double) windowSize)));
            break;
            
        case kFlatTop:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = 0.2810639 - (0.5208972 * cos(W_TWOPI * ((double) i / (double) windowSize))) + (0.1980399 * cos(W_FOURPI * ((double) i / (double) windowSize)));
            break;
            
        case kRectangle:
            for (unsigned long i = 0; i <= windowSize; i++)
                mWindow[i] = 1.0;
            break;
    }
    
    mWindow[windowSize + 1] = 0.0;
    
    if (sqrtWindow == true)
    {
        for (unsigned long i = 0; i <= windowSize; i++)
            mWindow[i] = sqrt(mWindow[i]);
    }
    
    // Store window parameters
    
    mWindowType = windowType;
    mSize = windowSize;
    mSqrtWindow = sqrtWindow;
    
    // Calculate the gain of the window
    
    double linearGain = 0.0;
    double powerGain = 0.0;
    
    for (unsigned long i = 0; i <= windowSize; i++)
        linearGain += mWindow[i];
    
    mLinearGain = linearGain / (double) (windowSize + 1);
    
    for (unsigned long i = 0; i <= windowSize; i++)
        powerGain += mWindow[i] * mWindow[i];
    
    mPowerGain = powerGain / (double) (windowSize + 1);
}

double FrameLib_Window::linearInterp(double pos)
{
    unsigned long idx = (unsigned long) pos;
    
    double fract = pos - idx;
    double lo = mWindow[idx];
    double hi = mWindow[idx + 1];
    
    return lo + fract * (hi - lo);
}

// Update and Process

void FrameLib_Window::update()
{
    FrameLib_Parameters::Serial *serialised = getInput(1);
    
    if (serialised)
        mParameters.set(serialised);
}

void FrameLib_Window::process()
{
    // Get Input
    
    unsigned long sizeIn, sizeOut, sizeFactor;
    double *input = getInput(0, &sizeIn);
    
    requestOutputSize(0, sizeIn);
    allocateOutputs();
    
    double *output = getOutput(0, &sizeOut);
    
    if (sizeOut)
    {
        Compensation compensate = (Compensation) mParameters.getInt(kCompensation);
        EndPoints ends = (EndPoints) mParameters.getInt(kEndPoints);
        
        double gain;
        
        sizeFactor = ends == kBoth ? sizeIn - 1 : sizeIn;
        sizeFactor = ends == kNone ? sizeIn + 1 : sizeFactor;
        
        updateWindow(sizeIn, ends);
        
        bool preIncrement = ends == kNone || ends == kLast;
        
        switch (compensate)
        {
            case kOff:
                gain = 1.0;
                break;
                
            case kLinear:
                gain = mLinearGain;
                break;
                
            case kPower:
                gain = mPowerGain;
                break;
                
            case kPowerOverLinear:
                gain = mPowerGain / mLinearGain;
                break;
        }
        
        if (mSize % sizeFactor)
        {
            double incr = (double) mSize / (double) sizeFactor;
            double pos = preIncrement ? incr : 0.0;
            
            for (unsigned long i = 0; i < sizeIn; i++, pos += incr)
                output[i] = input[i] * gain * linearInterp(pos);
        }
        else
        {
            unsigned long incr =  mSize / sizeFactor;
            unsigned long pos = preIncrement ? incr : 0;
            
            for (unsigned long i = 0; i < sizeIn; i++, pos += incr)
                output[i] = input[i] * gain * mWindow[pos];
        }
    }
}