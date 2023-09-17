#include <csdr/ringbuffer.hpp>
#include <cstring>

#include "pycsdr.hpp"
#include "buffer.hpp"
#include "types.hpp"
#include "bufferreader.hpp"

template <typename T>
static void createBuffer(Buffer* self, uint32_t size) {
    auto buffer = new Csdr::Ringbuffer<T>(size);
    self->writer = buffer;
}

static int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds) {
    char* kwlist[] = {(char*) "format", (char*) "size", NULL};

    uint32_t size = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!|I", kwlist, FORMAT_TYPE, &self->writerFormat, &size)) {
        return -1;
    }

    if (size == 0) {
        size = DEFAULT_BUFFER_SIZE;
    }

    try {
        if (self->writerFormat == FORMAT_CHAR) {
            createBuffer<unsigned char>(self, size);
        } else if (self->writerFormat == FORMAT_SHORT) {
            createBuffer<short>(self, size);
        } else if (self->writerFormat == FORMAT_FLOAT) {
            createBuffer<float>(self, size);
        } else if (self->writerFormat == FORMAT_COMPLEX_SHORT) {
            createBuffer<Csdr::complex<short>>(self, size);
        } else if (self->writerFormat == FORMAT_COMPLEX_FLOAT) {
            createBuffer<Csdr::complex<float>>(self, size);
        } else if (self->writerFormat == FORMAT_COMPLEX_CHAR) {
            createBuffer<Csdr::complex<unsigned char>>(self, size);
        } else {
            PyErr_SetString(PyExc_ValueError, "invalid buffer format");
            return -1;
        }
    } catch (const Csdr::BufferError& e) {
        PyErr_SetString(PyExc_BufferError, e.what());
        return -1;
    }

    Py_INCREF(self->writerFormat);

    return 0;
}

static int Buffer_finalize(Buffer* self) {
    Py_DECREF(self->writerFormat);
    delete self->writer;
    return 0;
}

static PyObject* Buffer_getReader(Buffer* self) {
    PyObject* args = PyTuple_Pack(1, self);
    if (args == NULL) {
        return NULL;
    }

    BufferReader* reader = (BufferReader*) PyObject_CallObject((PyObject*) BufferReaderType, args);
    Py_DECREF(args);
    if (reader == NULL) {
        return NULL;
    }

    return (PyObject*) reader;
}

static PyObject* Buffer_getFormat(Buffer* self) {
    Py_INCREF(self->writerFormat);
    return self->writerFormat;
}

template <typename T>
static int writeToBuffer(Buffer* self, char* data, size_t len) {
    auto writer = dynamic_cast<Csdr::Writer<T>*>(self->writer);
    if (writer->writeable() < len / sizeof(T)) {
        PyErr_SetString(PyExc_BufferError, "insufficient buffer space");
        return -1;
    }
    std::memcpy(writer->getWritePointer() + self->writeOverhang, data, len);
    writer->advance((len + self->writeOverhang) / sizeof(T));
    self->writeOverhang = (len + self->writeOverhang) % sizeof(T);
    return 0;
}

static PyObject* Buffer_write(Buffer* self, PyObject* args, PyObject* kwds) {
    char* kwlist[] = {(char*) "data", NULL};

    PyObject* bytes;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "S", kwlist, &bytes)) {
        return NULL;
    }

    char* data;
    Py_ssize_t len;
    if (PyBytes_AsStringAndSize(bytes, &data, &len) == -1) {
        return NULL;
    }

    int r = -1;
    if (self->writerFormat == FORMAT_CHAR) {
        r = writeToBuffer<unsigned char>(self, data, len);
    } else if (self->writerFormat == FORMAT_SHORT) {
        r = writeToBuffer<short>(self, data, len);
    } else if (self->writerFormat == FORMAT_FLOAT) {
        r = writeToBuffer<float>(self, data, len);
    } else if (self->writerFormat == FORMAT_COMPLEX_SHORT) {
        r = writeToBuffer<Csdr::complex<short>>(self, data, len);
    } else if (self->writerFormat == FORMAT_COMPLEX_FLOAT) {
        r = writeToBuffer<Csdr::complex<float>>(self, data, len);
    } else if (self->writerFormat == FORMAT_COMPLEX_CHAR) {
        r = writeToBuffer<Csdr::complex<unsigned char>>(self, data, len);
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid buffer format");
        return NULL;
    }

    if (r == -1) {
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyMethodDef Buffer_methods[] = {
    {"getReader", (PyCFunction) Buffer_getReader, METH_NOARGS,
     "get a reader instance"},
    {"getFormat", (PyCFunction) Buffer_getFormat, METH_NOARGS,
     "get the buffer format"},
    {"write", (PyCFunction) Buffer_write, METH_VARARGS | METH_KEYWORDS,
     "write to the buffer"},
    {NULL}  /* Sentinel */
};


static PyType_Slot BufferSlots[] = {
    {Py_tp_init, (void*) Buffer_init},
    {Py_tp_finalize, (void*) Buffer_finalize},
    {Py_tp_methods, Buffer_methods},
    {0, 0}
};

PyType_Spec BufferSpec = {
    "pycsdr.modules.Buffer",
    sizeof(Buffer),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    BufferSlots
};