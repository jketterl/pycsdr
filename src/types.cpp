#include "types.h"

static PyObject* getFormatType() {
    PyObject* module = PyImport_ImportModule("pycsdr.types");
    if (module == NULL) {
        PyErr_Print();
        exit(1);
    }

    PyObject* FormatType = PyObject_GetAttrString(module, "Format");
    if (FormatType == NULL) {
        PyErr_Print();
        exit(1);
    }

    Py_INCREF(FormatType);
    Py_DECREF(module);

    return FormatType;
}

PyObject* FormatType = getFormatType();

static PyObject* getFormat(const char* name) {
    PyObject* format = PyObject_GetAttrString(FormatType, name);
    if (format == NULL) {
        PyErr_Print();
        exit(1);
    }

    Py_INCREF(format);
    return format;
}

PyObject* FORMAT_CHAR = getFormat("CHAR");
PyObject* FORMAT_SHORT = getFormat("SHORT");
PyObject* FORMAT_FLOAT = getFormat("FLOAT");
PyObject* FORMAT_COMPLEX_FLOAT = getFormat("COMPLEX_FLOAT");