#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/complex.hpp>

#include "module.h"

struct LogAveragePower: module<Csdr::complex<float>, float> {};

extern PyType_Spec LogAveragePowerSpec;
