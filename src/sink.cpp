#include "pycsdr.hpp"
#include "sink.hpp"
#include "types.hpp"

#include <csdr/complex.hpp>

static int Sink_clear(Sink* self) {
    if (self->reader != nullptr) {
        Py_DECREF(self->reader);
        self->reader = nullptr;
    }
    return 0;
}

PyObject* Sink_setReader(Sink* self, PyObject* args, PyObject* kwds) {
    Reader* reader;

    static char* kwlist[] = {(char*) "reader", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, ReaderType, &reader)) {
        return NULL;
    }

    if ((PyObject*) reader != Py_None && self->inputFormat != reader->readerFormat) {
        PyErr_SetString(PyExc_ValueError, "invalid reader format");
        return NULL;
    }

    if (self->reader != nullptr) {
        Py_DECREF(self->reader);
        self->reader = nullptr;
    }

    if ((PyObject*) reader != Py_None) {
        self->reader = reader;
        Py_INCREF(self->reader);
    }

    if (self->inputFormat == FORMAT_CHAR) {
        dynamic_cast<Csdr::Sink<unsigned char>*>(self->sink)->setReader(
                dynamic_cast<Csdr::Reader<unsigned char>*>(self->reader->reader)
        );
    } else if (self->inputFormat == FORMAT_SHORT) {
        dynamic_cast<Csdr::Sink<short>*>(self->sink)->setReader(
                dynamic_cast<Csdr::Reader<short>*>(self->reader->reader)
        );
    } else if (self->inputFormat == FORMAT_FLOAT) {
        dynamic_cast<Csdr::Sink<float>*>(self->sink)->setReader(
                dynamic_cast<Csdr::Reader<float>*>(self->reader->reader)
        );
    } else if (self->inputFormat == FORMAT_COMPLEX_FLOAT) {
        dynamic_cast<Csdr::Sink<Csdr::complex<float>>*>(self->sink)->setReader(
                dynamic_cast<Csdr::Reader<Csdr::complex<float>>*>(self->reader->reader)
        );
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* Sink_getInputFormat(Sink* self) {
    Py_INCREF(self->inputFormat);
    return self->inputFormat;
}

static PyMethodDef Sink_methods[] = {
    {"setReader", (PyCFunction) Sink_setReader, METH_VARARGS | METH_KEYWORDS,
     "set the reader to pass data"
    },
    {"getInputFormat", (PyCFunction) Sink_getInputFormat, METH_NOARGS,
     "get input format"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot SinkSlots[] = {
    {Py_tp_clear, (void*) Sink_clear},
    {Py_tp_methods, Sink_methods},
    {0, 0}
};

PyType_Spec SinkSpec = {
    "pycsdr.modules.Sink",
    sizeof(Sink),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    SinkSlots
};
