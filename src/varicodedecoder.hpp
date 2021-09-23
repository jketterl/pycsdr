#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.hpp"

struct VaricodeDecoder: Module {};

extern PyType_Spec VaricodeDecoderSpec;