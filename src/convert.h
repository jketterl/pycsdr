#pragma once

#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module.h"

struct Convert: Module {
    PyObject* inFormat;
    PyObject* outFormat;
};

extern PyType_Spec ConvertSpec;