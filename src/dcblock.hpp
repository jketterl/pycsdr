#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.hpp"

struct DcBlock: Module {};

extern PyType_Spec DcBlockSpec;