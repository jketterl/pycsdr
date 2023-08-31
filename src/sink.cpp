#include "pycsdr.hpp"
#include "sink.hpp"
#include "types.hpp"

#include <csdr/complex.hpp>

int Sink_finalize(Sink* self) {
    if (self->reader != nullptr) {
        Py_DECREF(self->reader);
        self->reader = nullptr;
    }
    if (self->sink != nullptr) {
        delete self->sink;
        self->sink = nullptr;
    }
    return 0;
}

template <typename T>
static void setReader(Sink* self) {
    Csdr::Reader<T>* r = nullptr;
    if (self->reader != nullptr) r = dynamic_cast<Csdr::Reader<T>*>(self->reader->reader);
    dynamic_cast<Csdr::Sink<T>*>(self->sink)->setReader(r);
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

    auto oldReader = self->reader;

    if ((PyObject*) reader != Py_None) {
        self->reader = reader;
        Py_INCREF(self->reader);
    } else {
        self->reader = nullptr;
    }

    if (self->inputFormat == FORMAT_CHAR) {
        setReader<unsigned char>(self);
    } else if (self->inputFormat == FORMAT_SHORT) {
        setReader<short>(self);
    } else if (self->inputFormat == FORMAT_FLOAT) {
        setReader<float>(self);
    } else if (self->inputFormat == FORMAT_COMPLEX_SHORT) {
        setReader<Csdr::complex<short>>(self);
    } else if (self->inputFormat == FORMAT_COMPLEX_FLOAT) {
        setReader<Csdr::complex<float>>(self);
    } else if (self->inputFormat == FORMAT_COMPLEX_CHAR) {
        setReader<Csdr::complex<unsigned char>>(self);
    } else {
        if (self->reader != nullptr) {
            Py_DECREF(self->reader);
            self->reader = nullptr;
        }
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    if (oldReader != nullptr) {
        Py_DECREF(oldReader);
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
    {Py_tp_finalize, (void*) Sink_finalize},
    {Py_tp_methods, Sink_methods},
    {0, 0}
};

PyType_Spec SinkSpec = {
    "pycsdr.modules.Sink",
    sizeof(Sink),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_FINALIZE,
    SinkSlots
};
