#include "tcpsource.hpp"
#include "types.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int TcpSource_init(TcpSource* self, PyObject* args, PyObject* kwds) {
    unsigned short port = 0;

    static char* kwlist[] = {(char*) "port", (char*) "format", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "HO!", kwlist, &port, FORMAT_TYPE, &self->outputFormat)) {
        return -1;
    }

    try {
        if (self->outputFormat == FORMAT_CHAR) {
            self->source = new Csdr::TcpSource<unsigned char>(inet_addr("127.0.0.1"), port);
        } else if (self->outputFormat == FORMAT_SHORT) {
            self->source = new Csdr::TcpSource<short>(inet_addr("127.0.0.1"), port);
        } else if (self->outputFormat == FORMAT_FLOAT) {
            self->source = new Csdr::TcpSource<float>(inet_addr("127.0.0.1"), port);
        } else if (self->outputFormat == FORMAT_COMPLEX_SHORT) {
            self->source = new Csdr::TcpSource<Csdr::complex<short>>(inet_addr("127.0.0.1"), port);
        } else if (self->outputFormat == FORMAT_COMPLEX_FLOAT) {
            self->source = new Csdr::TcpSource<Csdr::complex<float>>(inet_addr("127.0.0.1"), port);
        } else if (self->outputFormat == FORMAT_COMPLEX_CHAR) {
            self->source = new Csdr::TcpSource<Csdr::complex<unsigned char>>(inet_addr("127.0.0.1"), port);
        } else {
            PyErr_SetString(PyExc_ValueError, "unsupported tcpsource format");
            return -1;
        }
    } catch (const Csdr::NetworkException& e) {
        PyErr_SetString(PyExc_ConnectionError, e.what());
        return -1;
    }

    return 0;
}

static PyObject* TcpSource_stop(TcpSource* self) {
    if (self->outputFormat == FORMAT_CHAR) {
        dynamic_cast<Csdr::TcpSource<unsigned char>*>(self->source)->stop();
    } else if (self->outputFormat == FORMAT_SHORT) {
        dynamic_cast<Csdr::TcpSource<short>*>(self->source)->stop();
    } else if (self->outputFormat == FORMAT_FLOAT) {
        dynamic_cast<Csdr::TcpSource<float>*>(self->source)->stop();
    } else if (self->outputFormat == FORMAT_COMPLEX_SHORT) {
        dynamic_cast<Csdr::TcpSource<Csdr::complex<short>>*>(self->source)->stop();
    } else if (self->outputFormat == FORMAT_COMPLEX_FLOAT) {
        dynamic_cast<Csdr::TcpSource<Csdr::complex<float>>*>(self->source)->stop();
    } else if (self->outputFormat == FORMAT_COMPLEX_CHAR) {
        dynamic_cast<Csdr::TcpSource<Csdr::complex<unsigned char>>*>(self->source)->stop();
    } else {
        PyErr_SetString(PyExc_ValueError, "unsupported tcpsource format");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyMethodDef TcpSource_methods[] = {
    {"stop", (PyCFunction) TcpSource_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot TcpSourceSlots[] = {
    {Py_tp_init, (void*) TcpSource_init},
    {Py_tp_methods, TcpSource_methods},
    {0, 0}
};

PyType_Spec TcpSourceSpec = {
    "pycsdr.modules.TcpSource",
    sizeof(TcpSource),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    TcpSourceSlots
};
