#include <csdr/ringbuffer.hpp>

#include "pycsdr.hpp"
#include "buffer.hpp"
#include "types.hpp"
#include "bufferreader.hpp"

static int Buffer_finalize(Buffer* self) {
    Py_DECREF(self->writerFormat);
    return 0;
}

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

    Py_INCREF(self->writerFormat);

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
        } else if (self->writerFormat == FORMAT_COMPLEX_FLOAT) {
            createBuffer<Csdr::complex<float>>(self, size);
        } else {
            PyErr_SetString(PyExc_ValueError, "invalid buffer format");
            return -1;
        }
    } catch (const Csdr::BufferError& e) {
        PyErr_SetString(PyExc_BufferError, e.what());
        return -1;
    }

    return 0;
}

template <typename T>
static Csdr::UntypedReader* createReader(Buffer* self) {
    auto buffer = dynamic_cast<Csdr::Ringbuffer<T>*>(self->writer);
    return new Csdr::RingbufferReader<T>(buffer);
}

static PyObject* Buffer_getReader(Buffer* self) {
    BufferReader* reader = PyObject_New(BufferReader, BufferReaderType);
    if (reader == NULL) {
        return NULL;
    }

    if (self->writerFormat == FORMAT_CHAR) {
        reader->reader = createReader<unsigned char>(self);
    } else if (self->writerFormat == FORMAT_SHORT) {
        reader->reader = createReader<short>(self);
    } else if (self->writerFormat == FORMAT_FLOAT) {
        reader->reader = createReader<float>(self);
    } else if (self->writerFormat == FORMAT_COMPLEX_FLOAT) {
        reader->reader = createReader<Csdr::complex<float>>(self);
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid buffer format");
        return NULL;
    }

    Py_INCREF(self->writerFormat);
    reader->readerFormat = self->writerFormat;

    return reader;
}

static PyMethodDef Buffer_methods[] = {
    {"getReader", (PyCFunction) Buffer_getReader, METH_NOARGS,
     "get a reader instance"},
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