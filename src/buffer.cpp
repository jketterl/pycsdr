#include "buffer.h"

static int Buffer_clear(Buffer* self) {
    if (self->buffer != nullptr) delete self->buffer;
    self->buffer = NULL;
    if (self->reader != nullptr) delete self->reader;
    self->reader = NULL;
    return 0;
}

static int Buffer_init(Buffer* self, PyObject* args, PyObject* kwds) {
    char* kwlist[] = {(char*) "size", NULL};

    uint32_t size = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|I", kwlist,
                                     &size))
        return -1;

    if (size == 0) {
        size = DEFAULT_BUFFER_SIZE;
    }

    self->buffer = new Csdr::Ringbuffer<Csdr::complex<float>>(size);
    self->reader = new Csdr::RingbufferReader<Csdr::complex<float>>(self->buffer);

    return 0;
}

static PyObject* Buffer_read(Buffer* self, PyObject* Py_UNUSED(ignored)) {
    uint32_t available;
    while ((available = self->reader->available()) == 0) {
        Py_BEGIN_ALLOW_THREADS
        self->reader->wait();
        Py_END_ALLOW_THREADS
    }
    PyObject* bytes = PyMemoryView_FromMemory((char*) self->reader->getReadPointer(), available * sizeof(short), PyBUF_READ);
    self->reader->advance(available);
    return bytes;
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