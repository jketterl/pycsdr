#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/complex.hpp>

#include "module.hpp"

struct NoiseFilter: Module {
    int threshold = 10;
    int wndSize = 32;
    int fftSize = 4096;
};

extern PyType_Spec NoiseFilterSpec;
