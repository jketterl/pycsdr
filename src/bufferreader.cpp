#include "bufferreader.hpp"
#include "types.hpp"
#include "pycsdr.hpp"

#include <csdr/ringbuffer.hpp>

template <typename T>
static Csdr::UntypedReader* createReader(BufferReader* self) {
    auto buffer = dynamic_cast<Csdr::Ringbuffer<T>*>(self->buffer->writer);
    return new Csdr::RingbufferReader<T>(buffer);
}

static int BufferReader_init(BufferReader* self, PyObject* args, PyObject* kwds) {
    char* kwlist[] = {(char*) "buffer", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, BufferType, &self->buffer)) {
        return -1;
    }
    Py_INCREF(self->buffer);

    self->readerFormat = self->buffer->writerFormat;
    Py_INCREF(self->readerFormat);

    if (self->readerFormat == FORMAT_CHAR) {
        self->reader = createReader<unsigned char>(self);
    } else if (self->readerFormat == FORMAT_SHORT) {
        self->reader = createReader<short>(self);
    } else if (self->readerFormat == FORMAT_FLOAT) {
        self->reader = createReader<float>(self);
    } else if (self->readerFormat == FORMAT_COMPLEX_SHORT) {
        self->reader = createReader<Csdr::complex<short>>(self);
    } else if (self->readerFormat == FORMAT_COMPLEX_FLOAT) {
        self->reader = createReader<Csdr::complex<float>>(self);
    } else {
        Py_DECREF(self->buffer);
        self->buffer = nullptr;
        PyErr_SetString(PyExc_ValueError, "invalid buffer format");
        return -1;
    }

    self->run = true;

    return 0;
}

static int BufferReader_finalize(BufferReader* self) {
    if (self->buffer != nullptr) {
        Py_DECREF(self->buffer);
        self->buffer = nullptr;
    }

    Py_DECREF(self->readerFormat);

    return Reader_finalize(self);
}

template <typename T>
static PyObject* getBytes(Csdr::UntypedReader* reader) {
    size_t available = reader->available();
    char* source = (char *) (dynamic_cast<Csdr::Reader<T> *>(reader))->getReadPointer();
    PyObject* bytes = PyMemoryView_FromMemory(source, available * sizeof(T), PyBUF_READ);
    reader->advance(available);
    return bytes;
}

static PyObject* BufferReader_read(BufferReader* self) {
    try {
        while (self->run && self->reader->available() == 0) {
            Py_BEGIN_ALLOW_THREADS
            self->reader->wait();
            Py_END_ALLOW_THREADS
        }

        if (!self->run) {
            Py_RETURN_NONE;
        }

        if (self->readerFormat == FORMAT_CHAR) {
            return getBytes<unsigned char>(self->reader);
        } else if (self->readerFormat == FORMAT_SHORT) {
            return getBytes<short>(self->reader);
        } else if (self->readerFormat == FORMAT_FLOAT) {
            return getBytes<float>(self->reader);
        } else if (self->readerFormat == FORMAT_COMPLEX_SHORT) {
            return getBytes<Csdr::complex<short>>(self->reader);
        } else if (self->readerFormat == FORMAT_COMPLEX_FLOAT) {
            return getBytes<Csdr::complex<float>>(self->reader);
        } else {
            PyErr_SetString(PyExc_ValueError, "invalid format");
            return NULL;
        }
    } catch (const Csdr::BufferError&) {
        Py_RETURN_NONE;
    }
}

static PyObject* BufferReader_stop(BufferReader* self) {
    self->run = false;
    self->reader->unblock();
    Py_RETURN_NONE;
}

static PyMethodDef BufferReader_methods[] = {
    {"read", (PyCFunction) BufferReader_read, METH_NOARGS,
     "read bytes from the buffer"},
    {"stop", (PyCFunction) BufferReader_stop, METH_NOARGS,
     "stop the reader and unblock calls to read()"},
    {NULL}  /* Sentinel */
};

static PyType_Slot BufferReaderSlots[] = {
    {Py_tp_init, (void*) BufferReader_init},
    {Py_tp_finalize, (void*) BufferReader_finalize},
    {Py_tp_methods, BufferReader_methods},
    {0, 0}
};

PyType_Spec BufferReaderSpec = {
    "pycsdr.modules.BufferReader",
    sizeof(BufferReader),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    BufferReaderSlots
};