#include "types.h"

PyTypeObject* getFormatType() {
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

    return (PyTypeObject*) FormatType;
}

PyObject* getFormat(const char* name) {
    PyObject* format = PyObject_GetAttrString((PyObject*) FORMAT_TYPE, name);
    if (format == NULL) {
        PyErr_Print();
        exit(1);
    }

    Py_INCREF(format);
    return format;
}