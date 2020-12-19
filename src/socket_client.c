#include "socket_client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int SocketClient_traverse(SocketClient* self, visitproc visit, void* arg) {
    Py_VISIT(self->buffer);
    return 0;
}

int SocketClient_clear(SocketClient* self) {
    self->run = false;
    void* retval = NULL;
    if (self->worker != 0) pthread_join(self->worker, retval);
    self->worker = 0;
    close(self->socket);
    if (self->buffer != NULL) Py_DECREF(self->buffer);
    self->buffer = NULL;
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
        Py_INCREF(self);
        self->port = 0;
        self->socket = 0;
        self->buffer = NULL;
        self->run = true;
        self->worker = 0;
    }
    return (PyObject*) self;
}

void* SocketClient_worker(void* ctx) {
    SocketClient* self = (SocketClient*) ctx;
    int read_bytes;
    int available;
    int offset = 0;
    while (self->run) {
        available = Buffer_getWriteable(self->buffer);
        if (available > 1024) available = 1024;
        read_bytes = recv(self->socket, Buffer_getWritePointer(self->buffer) + offset, available * SOCKET_ITEM_SIZE - offset, 0);
        if (read_bytes <= 0) {
            self->run = false;
        } else {
            offset = (offset + read_bytes) % SOCKET_ITEM_SIZE;
            Buffer_advance(self->buffer, (offset + read_bytes) / SOCKET_ITEM_SIZE);
        }
    }

    Buffer_shutdown(self->buffer);

    PyGILState_STATE gstate = PyGILState_Ensure();
    Py_DECREF(self);
    PyGILState_Release(gstate);
    return NULL;
}

int SocketClient_init(SocketClient* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"port", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist,
                                     &self->port))
        return -1;

    // we expect 32-bit float IQ samples
    PyObject* bufferArgs = Py_BuildValue("()");
    if (bufferArgs == NULL) return -1;
    PyObject* bufferKwargs = Py_BuildValue("{s:i}", "item_size", SOCKET_ITEM_SIZE);
    if (bufferKwargs == NULL) return -1;
    self->buffer = (Buffer*) PyObject_Call((PyObject*) &BufferType, bufferArgs, bufferKwargs);
    Py_DECREF(args);
    Py_DECREF(bufferKwargs);

    if (self->buffer == NULL) return -1;
    Py_INCREF(self->buffer);

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

    Py_INCREF(self);
    if (pthread_create(&self->worker, NULL, SocketClient_worker, self) != 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        return -1;
    }

    pthread_setname_np(self->worker, "pycsdr SocketCl");

    return 0;
}

PyObject* SocketClient_getBuffer(SocketClient* self, PyObject* Py_UNUSED(ignored)) {
    if (self->buffer == NULL) {
        Py_RETURN_NONE;
    }
    Py_INCREF(self->buffer);
    return (PyObject*) self->buffer;
}

PyObject* SocketClient_stop(SocketClient* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    Py_RETURN_NONE;
}

PyMethodDef SocketClient_methods[] = {
    {"getBuffer", (PyCFunction) SocketClient_getBuffer, METH_NOARGS,
     "get the output buffer"
    },
    {"stop", (PyCFunction) SocketClient_stop, METH_NOARGS,
     "stop processing"
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

