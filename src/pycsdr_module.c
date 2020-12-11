#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "socket_client.h"

static PyModuleDef pycsdrmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pycsdr",
    .m_doc = "Python bindings for the csdr library",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_pycsdr(void) {
    PyObject* m;
    if (PyType_Ready(&SocketClientType) < 0)
        return NULL;

    m = PyModule_Create(&pycsdrmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&SocketClientType);
    PyModule_AddObject(m, "SocketClient", (PyObject*) &SocketClientType);
    return m;
}
