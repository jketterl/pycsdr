#include "fft_exchange_sides_ff.h"

int FftExchangeSides_traverse(FftExchangeSides* self, visitproc visit, void* arg) {
    return 0;
}

int FftExchangeSides_clear(FftExchangeSides* self) {
    self->run = false;
    void* retval = NULL;
    if (self->worker != 0) pthread_join(self->worker, retval);
    self->worker = 0;
    if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer);
    self->inputBuffer = NULL;
    if (self->buffer != NULL) Py_DECREF(self->buffer);
    self->buffer = NULL;
    return 0;
}

void FftExchangeSides_dealloc(FftExchangeSides* self) {
    PyObject_GC_UnTrack(self);
    FftExchangeSides_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* FftExchangeSides_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    FftExchangeSides* self;
    self = (FftExchangeSides*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->buffer = NULL;
        self->inputBuffer = NULL;
        self->read_pos = 0;
        self->fft_size = 0;
        self->run = true;
        self->worker = 0;
    }
    return (PyObject*) self;
}

void* FftExchangeSides_worker(void* ctx) {
    FftExchangeSides* self = (FftExchangeSides*) ctx;

    float* input = malloc(sizeof(float) * self->fft_size);
    uint32_t half = self->fft_size / 2;

    while (self->run) {
        self->read_pos = Buffer_read_n(self->inputBuffer, input + half, self->read_pos, half);
        self->read_pos = Buffer_read_n(self->inputBuffer, input, self->read_pos, half);
        Buffer_write(self->buffer, input, self->fft_size);
    }

    Py_DECREF(self);
    return NULL;
}

int FftExchangeSides_init(FftExchangeSides* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"fft_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist,
                                     &self->fft_size))
        return -1;

    // we output 32-bit float real samples
    PyObject* bufferArgs = Py_BuildValue("()");
    if (bufferArgs == NULL) return -1;
    PyObject* bufferKwargs = Py_BuildValue("{s:i}", "item_size", sizeof(float));
    if (bufferKwargs == NULL) return -1;
    self->buffer = (Buffer*) PyObject_Call((PyObject*) &BufferType, bufferArgs, bufferKwargs);
    Py_DECREF(args);
    Py_DECREF(bufferKwargs);

    if (self->buffer == NULL) return -1;
    Py_INCREF(self->buffer);

    return 0;
}

PyObject* FftExchangeSides_setInput(FftExchangeSides* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"input", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->inputBuffer))
        return NULL;

    if (Buffer_getItemSize(self->inputBuffer) != sizeof(float)) {
        self->inputBuffer = NULL;
        PyErr_SetString(PyExc_ValueError, "Input buffer item size mismatch");
        return NULL;
    }

    Py_INCREF(self->inputBuffer);

    Py_INCREF(self);
    if (pthread_create(&self->worker, NULL, FftExchangeSides_worker, self) != 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    pthread_setname_np(self->worker, "pycsdr FftExcha");

    Py_RETURN_NONE;
}

PyObject* FftExchangeSides_getBuffer(FftExchangeSides* self, PyObject* Py_UNUSED(ignored)) {
    if (self->buffer == NULL) {
        Py_RETURN_NONE;
    }
    Py_INCREF(self->buffer);
    return (PyObject*) self->buffer;
}

PyMethodDef FftExchangeSides_methods[] = {
    {"getBuffer", (PyCFunction) FftExchangeSides_getBuffer, METH_NOARGS,
     "get the output buffer"
    },
    {"setInput", (PyCFunction) FftExchangeSides_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject FftExchangeSidesType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pycsdr.FftExchangeSides",
    .tp_doc = "Custom objects",
    .tp_basicsize = sizeof(FftExchangeSides),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = FftExchangeSides_new,
    .tp_init = (initproc) FftExchangeSides_init,
    .tp_dealloc = (destructor) FftExchangeSides_dealloc,
    .tp_traverse = (traverseproc) FftExchangeSides_traverse,
    .tp_clear = (inquiry) FftExchangeSides_clear,
    .tp_methods = FftExchangeSides_methods,
};
