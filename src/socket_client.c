#include "socket_client.h"

int SocketClient_traverse(SocketClient* self, visitproc visit, void* arg) {
    return 0;
}

int SocketClient_clear(SocketClient* self) {
    return 0;
}

void SocketClient_dealloc(SocketClient* self) {
    PyObject_GC_UnTrack(self);
    SocketClient_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* SocketClient_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    SocketClient* self;
    self = (SocketClient*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->port = 0;
    }
    return (PyObject*) self;
}

int SocketClient_init(SocketClient* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"number", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist,
                                     &self->port))
        return -1;

    return 0;
}

PyObject* SocketClient_getBytes(SocketClient* self, PyObject* Py_UNUSED(ignored)) {
    //return PyUnicode_FromFormat("%S %S", self->first, self->last);
    Py_RETURN_NONE;
}
