#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.h"

struct FftSwap: module<float, float> {};

extern PyType_Spec FftSwapSpec;
