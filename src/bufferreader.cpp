#include "bufferreader.hpp"
#include "types.hpp"

// TODO hookup method
static int BufferReader_init(BufferReader* reader, PyObject* args, PyObject* kwds) {
    reader->run = true;

    return 0;
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
    } else if (self->readerFormat == FORMAT_COMPLEX_FLOAT) {
        return getBytes<Csdr::complex<float>>(self->reader);
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid format");
        return NULL;
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