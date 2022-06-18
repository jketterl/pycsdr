#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.hpp"

struct OpusEncoder: Module {};

extern PyType_Spec OpusEncoderSpec;