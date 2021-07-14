#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

extern PyObject* FormatType;

extern PyObject* FORMAT_CHAR;
extern PyObject* FORMAT_SHORT;
extern PyObject* FORMAT_FLOAT;
extern PyObject* FORMAT_COMPLEX_FLOAT;