#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.hpp"

struct FftAdpcm: Module {};

extern PyType_Spec FftAdpcmSpec;
