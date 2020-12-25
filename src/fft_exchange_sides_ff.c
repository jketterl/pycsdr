#include "fft_exchange_sides_ff.h"

int FftExchangeSides_traverse(FftExchangeSides* self, visitproc visit, void* arg) {
    Py_VISIT(self->outputBuffer);
    Py_VISIT(self->inputBuffer);
    return 0;
}

int FftExchangeSides_clear(FftExchangeSides* self) {
    FftExchangeSides_stop(self, Py_None);
    if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer);
    self->inputBuffer = NULL;
    if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer);
    self->outputBuffer = NULL;
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
        self->outputBuffer = NULL;
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

    uint32_t available;
    while (self->run) {
        uint32_t half = self->fft_size / 2;
        // get a buffer to write to (ensuring minimum space)
        float* output = Buffer_getWritePointer_n(self->outputBuffer, self->fft_size);
        // wait until minimum amount of samples is available on input buffer
        available = Buffer_wait_n(self->inputBuffer, self->read_pos, &self->run, self->fft_size);
        if (available == 0) {
            self->run = false;
            break;
        }
        // switching copy
        memcpy(output, Buffer_getReadPointer(self->inputBuffer, self->read_pos + half), half * sizeof(float));
        memcpy(output + half, Buffer_getReadPointer(self->inputBuffer, self->read_pos), half * sizeof(float));
        // advance the buffers after consuming and producing data
        Buffer_advanceReadPos(self->inputBuffer, &self->read_pos, self->fft_size);
        Buffer_advance(self->outputBuffer, self->fft_size);
    }

    //Buffer_shutdown(self->outputBuffer);

    return NULL;
}

int FftExchangeSides_init(FftExchangeSides* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"fft_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->fft_size))
        return -1;

    return 0;
}

PyObject* FftExchangeSides_setInput(FftExchangeSides* self, PyObject* args, PyObject* kwds) {
    if (FftExchangeSides_stop(self, Py_None) == NULL) {
        return NULL;
    }

    if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer);

    static char* kwlist[] = {"buffer", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->inputBuffer))
        return NULL;

    if (Buffer_getItemSize(self->inputBuffer) != sizeof(float)) {
        self->inputBuffer = NULL;
        PyErr_SetString(PyExc_ValueError, "Input buffer item size mismatch");
        return NULL;
    }

    Py_INCREF(self->inputBuffer);

    return FftExchangeSides_start(self);
}

PyObject* FftExchangeSides_setOutput(FftExchangeSides* self, PyObject* args, PyObject* kwds) {
    if (FftExchangeSides_stop(self, Py_None) == NULL) {
        return NULL;
    }

    if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer);

    static char* kwlist[] = {"buffer", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->outputBuffer))
        return NULL;

    Py_INCREF(self->outputBuffer);

    Buffer_setItemSize(self->outputBuffer, sizeof(float));

    return FftExchangeSides_start(self);
}

PyObject* FftExchangeSides_start(FftExchangeSides* self) {
    if (self->outputBuffer == NULL || self->inputBuffer == NULL) {
        Py_RETURN_NONE;
    } else {
        self->run = true;

        if (pthread_create(&self->worker, NULL, FftExchangeSides_worker, self) != 0) {
            PyErr_SetFromErrno(PyExc_OSError);
            return NULL;
        }

        pthread_setname_np(self->worker, "pycsdr FftExcha");

        Py_RETURN_NONE;
    }
}

PyObject* FftExchangeSides_stop(FftExchangeSides* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    if (self->worker != 0) {
        Buffer_unblock(self->inputBuffer);
        void* retval = NULL;
        pthread_join(self->worker, retval);
    }
    self->worker = 0;
    Py_RETURN_NONE;
}

PyMethodDef FftExchangeSides_methods[] = {
    {"setInput", (PyCFunction) FftExchangeSides_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) FftExchangeSides_setOutput, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"stop", (PyCFunction) FftExchangeSides_stop, METH_NOARGS,
     "stop processing"
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
