#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.h"

struct Limit: Module {};

extern PyType_Spec LimitSpec;