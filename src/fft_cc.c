#include "fft_cc.h"

int Fft_traverse(Fft* self, visitproc visit, void* arg) {
    return 0;
}

int Fft_clear(Fft* self) {
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
        self->buffer = NULL;
        self->inputBuffer = NULL;
        self->read_pos = 0;
        self->size = 0;
        self->every_n_samples = 0;
        self->run = true;
    }
    return (PyObject*) self;
}

void* Fft_worker(void* ctx) {
    Fft* self = (Fft*) ctx;
    window_t window = WINDOW_DEFAULT;
    complexf* input = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);
    complexf* windowed = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);
    complexf* output = (complexf*) fftwf_malloc(sizeof(complexf) * self->size);

	fftwf_plan plan = fftwf_plan_dft_1d(self->size, (fftwf_complex*) input, (fftwf_complex*) output, FFTW_FORWARD, FFTW_ESTIMATE);

	float* windowt = precalculate_window(self->size, window);

    while (self->run) {
        if (self->every_n_samples > self->size) {
            self->read_pos = Buffer_read_n(self->inputBuffer, input, self->read_pos, sizeof(complexf) * self->size);
            self->read_pos = Buffer_skip_n(self->inputBuffer, self->read_pos, sizeof(complexf) * self->every_n_samples - self->size);
            //skipping samples before next FFT (but fseek doesn't work for pipes)
            //for (int seek_remain = self->every_n_samples - self->size; seek_remain > 0; seek_remain -= the_bufsize) {
                //fread(temp_f, sizeof(complexf), MIN_M(the_bufsize,seek_remain), stdin);
            //}
        } else {
            //overlapped FFT
            for (int i=0; i < self->size - self->every_n_samples; i++) input[i] = input[i + self->every_n_samples];
            self->read_pos = Buffer_read_n(self->inputBuffer, input + self->size, self->read_pos, sizeof(complexf) * self->every_n_samples);
            //fread(input + self->size - self->every_n_samples, sizeof(complexf), self->every_n_samples, stdin);
        }
        apply_precalculated_window_c(input, windowed, self->size, windowt);
        fftwf_execute(plan);
        Buffer_write(self->buffer, output, self->size);
    }
    return NULL;
}

int Fft_init(Fft* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"size", "every_n_samples", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii", kwlist,
                                     &self->size, &self->every_n_samples))
        return -1;

    self->buffer = (Buffer*) PyObject_CallObject((PyObject*) &BufferType, NULL);
    Py_INCREF(self->buffer);

    return 0;
}

PyObject* Fft_setInput(Fft* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"input", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->inputBuffer))
        return NULL;

    if (pthread_create(&self->worker, NULL, Fft_worker, self) != 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    Py_RETURN_NONE;
}

PyObject* Fft_getBuffer(Fft* self, PyObject* Py_UNUSED(ignored)) {
    if (self->buffer == NULL) {
        Py_RETURN_NONE;
    }
    return (PyObject*) self->buffer;
}

PyMethodDef Fft_methods[] = {
    {"getBuffer", (PyCFunction) Fft_getBuffer, METH_NOARGS,
     "get the output buffer"
    },
    {"setInput", (PyCFunction) Fft_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
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
