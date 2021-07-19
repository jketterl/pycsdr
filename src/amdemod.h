#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.h"

struct AmDemod: Module {};

extern PyType_Spec AmDemodSpec;