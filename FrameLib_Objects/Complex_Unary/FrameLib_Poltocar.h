
#ifndef FRAMELIB_POLTOCAR_H
#define FRAMELIB_POLTOCAR_H

#include "FrameLib_Complex_Unary_Template.h"

struct PolToCar
{
    std::complex<double> operator()(const std::complex<double> &x) { return std::polar<double>(x.real(), x.imag()); }
};

// Info Specialisations

template<>
std::string FrameLib_Complex_UnaryOp<PolToCar>::objectInfo(bool verbose)
{
    return formatInfo("Converts from polar values at the input to cartesian values at output: The outputs are frames of the same length as the longer of the two inputs. If one input is shorter than the other it is padded with zeros to the length of the other before conversion.",
                      "Converts from polar values at the input to cartesian values at output.", verbose);
}

template<>
std::string FrameLib_Complex_UnaryOp<PolToCar>::inputInfo(unsigned long idx, bool verbose)
{
    if (idx == 0)
        return formatInfo("Amplitudes (or abs values)", "Amplitudes", verbose);
    else
        return formatInfo("Phases (or args)", "Phases", verbose);
}

template<>
std::string FrameLib_Complex_UnaryOp<PolToCar>::outputInfo(unsigned long idx, bool verbose)
{
    if (idx == 0)
        return formatInfo("Real or X Output", "Real or X Output", verbose);
    else
        return formatInfo("Imaginary or Y Output", "Imag or Y Output", verbose);
}

using FrameLib_Poltocar = FrameLib_Complex_UnaryOp<PolToCar>;

#endif
