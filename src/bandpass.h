#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/complex.hpp>

#include "module.h"

struct Bandpass: Module {
    float low_cut = 0.0f;
    float high_cut = 0.0f;
    float transition = 0.0f;
    bool use_fft = true;
};

extern PyType_Spec BandpassSpec;
