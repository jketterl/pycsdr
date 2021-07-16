#include "buffer.h"
#include "types.h"

static int Buffer_clear(Buffer* self) {
    if (self->writer != nullptr) delete self->writer;
    self->writer = NULL;
    if (self->reader != nullptr) delete self->reader;
    self->reader = NULL;
    Py_DECREF(self->format);
    return 0;
}

template <typename T>
static void createBuffer(Buffer* self, uint32_t size) {
    auto buffer = new Csdr::Ringbuffer<T>(size);
    self->writer = buffer;
    self->reader = new Csdr::RingbufferReader<T>(buffer);
}

static int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds) {
    char* kwlist[] = {(char*) "format", (char*) "size", NULL};

    uint32_t size = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!|I", kwlist, FormatType, &self->format, &size)) {
        return -1;
    }

    Py_INCREF(self->format);

    if (size == 0) {
        size = DEFAULT_BUFFER_SIZE;
    }

    if (self->format == FORMAT_CHAR) {
        createBuffer<unsigned char>(self, size);
    } else if (self->format == FORMAT_SHORT) {
        createBuffer<short>(self, size);
    } else if (self->format == FORMAT_FLOAT) {
        createBuffer<float>(self, size);
    } else if (self->format == FORMAT_COMPLEX_FLOAT) {
        createBuffer<Csdr::complex<float>>(self, size);
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid format");
        return -1;
    }

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

static PyObject* Buffer_read(Buffer* self, PyObject* Py_UNUSED(ignored)) {
    while (self->reader->available() == 0) {
        Py_BEGIN_ALLOW_THREADS
        self->reader->wait();
        Py_END_ALLOW_THREADS
    }

    if (self->format == FORMAT_CHAR) {
        return getBytes<unsigned char>(self->reader);
    } else if (self->format == FORMAT_SHORT) {
        return getBytes<short>(self->reader);
    } else if (self->format == FORMAT_FLOAT) {
        return getBytes<float>(self->reader);
    } else if (self->format == FORMAT_COMPLEX_FLOAT) {
        return getBytes<Csdr::complex<float>>(self->reader);
    } else {
        PyErr_SetString(PyExc_ValueError, "invalid format");
        return NULL;
    }
}

static PyObject* Buffer_stop(Buffer* self, PyObject* Py_UNUSED(ignored)) {
    Py_RETURN_NONE;
}

static PyMethodDef Buffer_methods[] = {
    {"read", (PyCFunction) Buffer_read, METH_NOARGS,
     "read bytes from the buffer"},
    {"stop", (PyCFunction) Buffer_stop, METH_NOARGS,
     "stop the buffer and unblock calls to read()"},
    {NULL}  /* Sentinel */
};

static PyType_Slot BufferSlots[] = {
    {Py_tp_init, (void*) Buffer_init},
    {Py_tp_clear, (void*) Buffer_clear},
    {Py_tp_methods, Buffer_methods},
    {0, 0}
};

PyType_Spec BufferSpec = {
    "pycsdr.Buffer",
    sizeof(Buffer),
    0,
    Py_TPFLAGS_DEFAULT,
    BufferSlots
};