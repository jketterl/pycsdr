#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/fft.hpp>
#include <csdr/complex.hpp>
#include "module.h"

struct Fft: module<Csdr::complex<float>, Csdr::complex<float>> {
};

extern PyType_Spec FftSpec;
