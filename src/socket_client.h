#define PY_SSIZE_T_CLEAN
#include <Python.h>

typedef struct {
    PyObject_HEAD
    int port;
} SocketClient;


int SocketClient_traverse(SocketClient* self, visitproc visit, void* arg);
int SocketClient_clear(SocketClient* self);
void SocketClient_dealloc(SocketClient* self);
PyObject* SocketClient_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int SocketClient_init(SocketClient* self, PyObject* args, PyObject* kwds);
PyObject* SocketClient_getBytes(SocketClient* self, PyObject* Py_UNUSED(ignored));

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

