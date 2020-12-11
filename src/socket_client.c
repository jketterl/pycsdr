#include "socket_client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "stdio.h"

int SocketClient_traverse(SocketClient* self, visitproc visit, void* arg) {
    return 0;
}

int SocketClient_clear(SocketClient* self) {
    close(self->socket);
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
        self->socket = 0;
    }
    return (PyObject*) self;
}

int SocketClient_init(SocketClient* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"port", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist,
                                     &self->port))
        return -1;

    struct sockaddr_in remote;

    if ((self->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        return -1;
    }

    memset(&remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(self->port);
    remote.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(self->socket, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        return -1;
    }

    return 0;
}

PyObject* SocketClient_getBytes(SocketClient* self, PyObject* Py_UNUSED(ignored)) {
    //return PyUnicode_FromFormat("%S %S", self->first, self->last);
    Py_RETURN_NONE;
}

PyMethodDef SocketClient_methods[] = {
    {"getBytes", (PyCFunction) SocketClient_getBytes, METH_NOARGS,
     "Return the number of bytes read"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject SocketClientType = {
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

