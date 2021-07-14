#include "types.h"

static PyObject* types_module = PyImport_ImportModule("pycsdr.types");

PyObject* FormatType = PyObject_GetAttrString(types_module, "Format");

PyObject* FORMAT_CHAR = PyObject_GetAttrString(FormatType, "CHAR");
PyObject* FORMAT_SHORT = PyObject_GetAttrString(FormatType, "SHORT");
PyObject* FORMAT_FLOAT = PyObject_GetAttrString(FormatType, "FLOAT");
PyObject* FORMAT_COMPLEX_FLOAT = PyObject_GetAttrString(FormatType, "COMPLEX_FLOAT");