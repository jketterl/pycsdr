#include "pycsdr.hpp"
#include "source.hpp"
#include "types.hpp"

#include <csdr/complex.hpp>

int Source_finalize(Source* self) {
    if (self->writer != nullptr) {
        Py_DECREF(self->writer);
        self->writer = nullptr;
    }
    if (self->source != nullptr) {
        delete self->source;
        self->source = nullptr;
    }
    return 0;
}

template <typename T>
static void setWriter(Source* self) {
    Csdr::Writer<T>* w = nullptr;
    if (self->writer != nullptr) w = dynamic_cast<Csdr::Writer<T>*>(self->writer->writer);
    dynamic_cast<Csdr::Source<T>*>(self->source)->setWriter(w);
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

    // do not DECREF the writer yet, it may still be in use by the module in another thread
    auto oldWriter = self->writer;

    if ((PyObject*) writer != Py_None) {
        self->writer = writer;
        Py_INCREF(self->writer);
    } else {
        self->writer = nullptr;
    }

    if (self->outputFormat == FORMAT_CHAR) {
        setWriter<unsigned char>(self);
    } else if (self->outputFormat == FORMAT_SHORT) {
        setWriter<short>(self);
    } else if (self->outputFormat == FORMAT_FLOAT) {
        setWriter<float>(self);
    } else if (self->outputFormat == FORMAT_COMPLEX_SHORT) {
        setWriter<Csdr::complex<short>>(self);
    } else if (self->outputFormat == FORMAT_COMPLEX_FLOAT) {
        setWriter<Csdr::complex<float>>(self);
    } else if (self->outputFormat == FORMAT_COMPLEX_CHAR) {
        setWriter<Csdr::complex<unsigned char>>(self);
    } else {
        if (self->writer != nullptr) {
            Py_DECREF(self->writer);
            self->writer = nullptr;
        }
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    if (oldWriter != nullptr) {
        Py_DECREF(oldWriter);
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
    {Py_tp_finalize, (void*) Source_finalize},
    {Py_tp_methods, Source_methods},
    {0, 0}
};

PyType_Spec SourceSpec = {
    "pycsdr.modules.Source",
    sizeof(Source),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_FINALIZE,
    SourceSlots
};
