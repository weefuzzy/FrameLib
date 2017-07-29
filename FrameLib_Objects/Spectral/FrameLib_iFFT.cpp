
#include "FrameLib_iFFT.h"
#include "FrameLib_Spectral_Functions.h"

// Constructor / Destructor

FrameLib_iFFT::FrameLib_iFFT(FrameLib_Context context, FrameLib_Parameters::Serial *serialisedParameters, void *owner) : FrameLib_Processor(context, 2, 1)
{
    mParameters.addInt(kMaxLength, "maxlength", 16384, 0);
    mParameters.setMin(0);
    mParameters.setInstantiation();
    
    mParameters.set(serialisedParameters);
    
    unsigned long maxFFTSizeLog2 = ilog2(mParameters.getInt(kMaxLength));
    
    mFFTSetup = hisstools_create_setup_d(maxFFTSizeLog2);
    mMaxFFTSize = 1 << maxFFTSizeLog2;
}

FrameLib_iFFT::~FrameLib_iFFT()
{
    hisstools_destroy_setup_d(mFFTSetup);
}

// Process

void FrameLib_iFFT::process()
{
    FFT_SPLIT_COMPLEX_D spectrum;
    
    unsigned long sizeIn1, sizeIn2, sizeIn, sizeOut;
    unsigned long FFTSizelog2 = 0;
    
    // Get Inputs
    
    double *input1 = getInput(0, &sizeIn1);
    double *input2 = getInput(1, &sizeIn2);
    
    sizeIn = sizeIn1 < sizeIn2 ? sizeIn1 : sizeIn2;
    
    if (sizeIn)
    {
        FFTSizelog2 = log2((sizeIn - 1) << 1);
        sizeOut = 1 << FFTSizelog2;
    }
    else
        sizeOut = 0;
    
    // Sanity Check
    
    if (sizeOut > mMaxFFTSize)
        sizeOut = 0;
    
    // Calculate output size
    
    requestOutputSize(0, sizeOut);
    allocateOutputs();
    double *output = getOutput(0, &sizeOut);
    
    spectrum.realp = (double *) (sizeOut ? mAllocator->alloc(sizeOut * sizeof(double)) : NULL);
    spectrum.imagp = spectrum.realp + (sizeOut >> 1);
    
    if (spectrum.realp)
    {
        double scale = 1.0 / (double) sizeOut;
        
        // Copy Spectrum
        
        for (unsigned long i = 0; (i < sizeIn) && (i < (sizeOut >> 1)); i++)
        {
            spectrum.realp[i] = input1[i];
            spectrum.imagp[i] = input2[i];
        }
        
        if (sizeIn == (sizeOut >> 1) + 1)
            spectrum.imagp[0] = input1[sizeOut >> 1];
        
        // Convert to time domain
        
        hisstools_rifft_d(mFFTSetup, &spectrum, FFTSizelog2);
        hisstools_zip_d(&spectrum, output, FFTSizelog2);
        
        // Scale
        
        for (unsigned long i = 0; i < sizeOut; i++)
            output[i] *= scale;
        
        mAllocator->dealloc(spectrum.realp);
    }
}