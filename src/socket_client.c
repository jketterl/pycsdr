#define PY_SSIZE_T_CLEAN
#include <Python.h>

typedef struct {
    PyObject_HEAD
    int port;
} SocketClient;

static int SocketClient_traverse(SocketClient* self, visitproc visit, void* arg) {
    return 0;
}

static int SocketClient_clear(SocketClient* self) {
    return 0;
}

static void SocketClient_dealloc(SocketClient* self) {
    PyObject_GC_UnTrack(self);
    SocketClient_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject* SocketClient_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    SocketClient* self;
    self = (SocketClient*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->port = 0;
    }
    return (PyObject*) self;
}

static int SocketClient_init(SocketClient* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"number", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist,
                                     &self->port))
        return -1;

    return 0;
}

static PyObject* SocketClient_getBytes(SocketClient* self, PyObject* Py_UNUSED(ignored)) {
    //return PyUnicode_FromFormat("%S %S", self->first, self->last);
    Py_RETURN_NONE;
}

static PyMethodDef SocketClient_methods[] = {
    {"getBytes", (PyCFunction) SocketClient_getBytes, METH_NOARGS,
     "Return the number of bytes read"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject SocketClientType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pycsdr.SocketClient",
    .tp_doc = "Custom objects",
    .tp_basicsize = sizeof(SocketClient),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = SocketClient_new,
    .tp_init = (initproc) SocketClient_init,
    .tp_dealloc = (destructor) SocketClient_dealloc,
    .tp_traverse = (traverseproc) SocketClient_traverse,
    .tp_clear = (inquiry) SocketClient_clear,
    .tp_methods = SocketClient_methods,
};

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
