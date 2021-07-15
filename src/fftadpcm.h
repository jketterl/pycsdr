#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.h"

struct FftAdpcm: module<float, unsigned char> {};

extern PyType_Spec FftAdpcmSpec;
