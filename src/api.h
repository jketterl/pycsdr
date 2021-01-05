#define PY_SSIZE_T_CLEAN
#include <Python.h>

PyObject* getApiType(const char* name);
PyObject* getApiTypeTuple(const char* name);
