#include "api.h"

PyObject* getApiType(const char* name) {
    PyObject* api_module = PyImport_ImportModule("pycsdr.api");
    if (api_module == NULL) {
        PyErr_Print();
        exit(1);
    }

    PyObject* api_type = PyObject_GetAttrString(api_module, name);
    if (api_type == NULL) {
        PyErr_Print();
        exit(1);
    }

    Py_DECREF(api_module);

    Py_INCREF(api_type);

    return api_type;
}

PyObject* getApiTypeTuple(const char* name) {
    PyObject* tuple = PyTuple_Pack(1, getApiType(name));
    if (tuple == NULL) return NULL;
    Py_INCREF(tuple);
    return tuple;
}