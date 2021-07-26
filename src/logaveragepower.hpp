#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/complex.hpp>

#include "module.hpp"

struct LogAveragePower: Module {};

extern PyType_Spec LogAveragePowerSpec;
