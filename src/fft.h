#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/complex.hpp>

#include "module.h"

struct Fft: Module {};

extern PyType_Spec FftSpec;
