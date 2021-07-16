#include "tcpsource.h"
#include "types.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int TcpSource_clear(TcpSource* self) {
    if (self->source != nullptr) delete self->source;
    self->source = nullptr;
    if (self->output != nullptr) {
        Py_DECREF(self->output);
        self->output = nullptr;
    }
    return 0;
}

static int TcpSource_init(TcpSource* self, PyObject* args, PyObject* kwds) {
    unsigned short port = 0;

    static char* kwlist[] = {(char*) "port", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist, &port)) {
        return -1;
    }

    try {
        self->source = new Csdr::TcpSource<Csdr::complex<float>>(inet_addr("127.0.0.1"), port);
    } catch (const Csdr::NetworkException& e) {
        PyErr_SetString(PyExc_ConnectionError, e.what());
        return -1;
    }

    return 0;
}

static PyObject* TcpSource_setOutput(TcpSource* self, PyObject* args, PyObject* kwds) {
    Buffer* buffer;

    static char* kwlist[] = {(char*) "writer", NULL};
    // TODO get the type check "O!" back
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &buffer)) {
        return NULL;
    }

    // we only support complex float here.
    if (buffer->format != FORMAT_COMPLEX_FLOAT) {
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    if (self->output != nullptr) {
        Py_DECREF(self->output);
    }
    self->output = buffer;
    Py_INCREF(self->output);

    self->source->setWriter(dynamic_cast<Csdr::Writer<Csdr::complex<float>>*>(buffer->writer));

    Py_RETURN_NONE;
}

static PyObject* TcpSource_stop(TcpSource* self) {
    self->source->stop();

    Py_RETURN_NONE;
}

static PyMethodDef TcpSource_methods[] = {
    {"setOutput", (PyCFunction) TcpSource_setOutput, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"stop", (PyCFunction) TcpSource_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot TcpSourceSlots[] = {
    {Py_tp_init, (void*) TcpSource_init},
    {Py_tp_clear, (void*) TcpSource_clear},
    {Py_tp_methods, TcpSource_methods},
    {0, 0}
};

PyType_Spec TcpSourceSpec = {
    "pycsdr.modules.TcpSource",
    sizeof(TcpSource),
    0,
    Py_TPFLAGS_DEFAULT,
    TcpSourceSlots
};
