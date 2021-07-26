#include "pycsdr.hpp"
#include "source.hpp"
#include "types.hpp"

#include <csdr/complex.hpp>

static int Source_clear(Source* self) {
    if (self->writer != nullptr) {
        Py_DECREF(self->writer);
        self->writer = nullptr;
    }
    return 0;
}

PyObject* Source_setWriter(Source* self, PyObject* args, PyObject* kwds) {
    Writer* writer;

    static char* kwlist[] = {(char*) "writer", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, WriterType, &writer)) {
        return NULL;
    }

    if ((PyObject*) writer != Py_None && self->outputFormat != writer->writerFormat) {
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    if (self->writer != nullptr) {
        Py_DECREF(self->writer);
        self->writer = nullptr;
    }

    if ((PyObject*) writer != Py_None) {
        self->writer = writer;
        Py_INCREF(self->writer);
    }

    if (self->outputFormat == FORMAT_CHAR) {
        dynamic_cast<Csdr::Source<unsigned char>*>(self->source)->setWriter(
                dynamic_cast<Csdr::Writer<unsigned char>*>(self->writer->writer)
        );
    } else if (self->outputFormat == FORMAT_SHORT) {
        dynamic_cast<Csdr::Source<short>*>(self->source)->setWriter(
                dynamic_cast<Csdr::Writer<short>*>(self->writer->writer)
        );
    } else if (self->outputFormat == FORMAT_FLOAT) {
        dynamic_cast<Csdr::Source<float>*>(self->source)->setWriter(
                dynamic_cast<Csdr::Writer<float>*>(self->writer->writer)
        );
    } else if (self->outputFormat == FORMAT_COMPLEX_FLOAT) {
        dynamic_cast<Csdr::Source<Csdr::complex<float>>*>(self->source)->setWriter(
                dynamic_cast<Csdr::Writer<Csdr::complex<float>>*>(self->writer->writer)
        );
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* Source_getOutputFormat(Source* self) {
    Py_INCREF(self->outputFormat);
    return self->outputFormat;
}

static PyMethodDef Source_methods[] = {
    {"setWriter", (PyCFunction) Source_setWriter, METH_VARARGS | METH_KEYWORDS,
     "set the writer to pass data"
    },
    {"getOutputFormat", (PyCFunction) Source_getOutputFormat, METH_NOARGS,
     "get output format"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot SourceSlots[] = {
    {Py_tp_clear, (void*) Source_clear},
    {Py_tp_methods, Source_methods},
    {0, 0}
};

PyType_Spec SourceSpec = {
    "pycsdr.modules.Source",
    sizeof(Source),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    SourceSlots
};
