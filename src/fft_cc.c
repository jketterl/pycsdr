#include "fft_cc.h"

int Fft_traverse(Fft* self, visitproc visit, void* arg) {
    Py_VISIT(self->buffer);
    Py_VISIT(self->inputBuffer);
    return 0;
}

int Fft_clear(Fft* self) {
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

void Fft_dealloc(Fft* self) {
    PyObject_GC_UnTrack(self);
    Fft_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* Fft_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    Fft* self;
    self = (Fft*) type->tp_alloc(type, 0);
    if (self != NULL) {
        Py_INCREF(self);
        self->buffer = NULL;
        self->inputBuffer = NULL;
        self->read_pos = 0;
        self->size = 0;
        self->every_n_samples = 0;
        self->run = true;
        self->worker = 0;
    }
    return (PyObject*) self;
}

void* Fft_worker(void* ctx) {
    Fft* self = (Fft*) ctx;
    window_t window = WINDOW_DEFAULT;
    complexf* input = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);
    complexf* windowed = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);
    complexf* output = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);

	fftwf_plan plan = fftwf_plan_dft_1d(self->size, (fftwf_complex*) windowed, (fftwf_complex*) output, FFTW_FORWARD, FFTW_ESTIMATE);

	float* windowt = precalculate_window(self->size, window);

	uint32_t read;

    while (self->run) {
        if (self->every_n_samples > self->size) {
            read = Buffer_read_n(self->inputBuffer, input, &self->read_pos, self->size);
            //skipping samples before next FFT
            read += Buffer_skip_n(self->inputBuffer, &self->read_pos, self->every_n_samples - self->size);
        } else {
            //overlapped FFT
            for (int i = 0; i < self->size - self->every_n_samples; i++) input[i] = input[i + self->every_n_samples];
            read = Buffer_read_n(self->inputBuffer, input + self->size - self->every_n_samples, &self->read_pos, self->every_n_samples);
        }
        if (read == 0) {
            self->run = false;
        } else {
            apply_precalculated_window_c(input, windowed, self->size, windowt);
            fftwf_execute(plan);
            Buffer_write(self->buffer, output, self->size);
        }
    }

    Buffer_shutdown(self->buffer);

    PyGILState_STATE gstate = PyGILState_Ensure();
    Py_DECREF(self->inputBuffer);
    Py_DECREF(self);
    PyGILState_Release(gstate);
    return NULL;
}

int Fft_init(Fft* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"size", "every_n_samples", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii", kwlist,
                                     &self->size, &self->every_n_samples))
        return -1;

    if (log2n(self->size) == -1) {
        PyErr_SetString(PyExc_ValueError, "fft_size should be power of 2");
        return -1;
    }

    // we output 32-bit float IQ samples
    PyObject* bufferArgs = Py_BuildValue("()");
    if (bufferArgs == NULL) return -1;
    PyObject* bufferKwargs = Py_BuildValue("{s:i}", "item_size", 8);
    if (bufferKwargs == NULL) return -1;
    self->buffer = (Buffer*) PyObject_Call((PyObject*) &BufferType, bufferArgs, bufferKwargs);
    Py_DECREF(args);
    Py_DECREF(bufferKwargs);

    if (self->buffer == NULL) return -1;
    Py_INCREF(self->buffer);

    return 0;
}

PyObject* Fft_setInput(Fft* self, PyObject* args, PyObject* kwds) {
    if (self->inputBuffer != NULL) {
        PyErr_SetString(PyExc_ValueError, "cannot set buffer twice");
        return NULL;
    }

    static char* kwlist[] = {"input", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->inputBuffer))
        return NULL;

    if (Buffer_getItemSize(self->inputBuffer) != sizeof(complexf)) {
        self->inputBuffer = NULL;
        PyErr_SetString(PyExc_ValueError, "Input buffer item size mismatch");
        return NULL;
    }

    Py_INCREF(self->inputBuffer);

    Py_INCREF(self);
    if (pthread_create(&self->worker, NULL, Fft_worker, self) != 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    pthread_setname_np(self->worker, "pycsdr Fft");

    Py_RETURN_NONE;
}

PyObject* Fft_getBuffer(Fft* self, PyObject* Py_UNUSED(ignored)) {
    if (self->buffer == NULL) {
        Py_RETURN_NONE;
    }
    Py_INCREF(self->buffer);
    return (PyObject*) self->buffer;
}

PyObject* Fft_stop(Fft* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    Py_RETURN_NONE;
}

PyMethodDef Fft_methods[] = {
    {"getBuffer", (PyCFunction) Fft_getBuffer, METH_NOARGS,
     "get the output buffer"
    },
    {"setInput", (PyCFunction) Fft_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"stop", (PyCFunction) Fft_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject FftType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pycsdr.Fft",
    .tp_doc = "Custom objects",
    .tp_basicsize = sizeof(Fft),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = Fft_new,
    .tp_init = (initproc) Fft_init,
    .tp_dealloc = (destructor) Fft_dealloc,
    .tp_traverse = (traverseproc) Fft_traverse,
    .tp_clear = (inquiry) Fft_clear,
    .tp_methods = Fft_methods,
};
