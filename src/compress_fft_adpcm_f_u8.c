#include "compress_fft_adpcm_f_u8.h"

int CompressFftAdpcm_traverse(CompressFftAdpcm* self, visitproc visit, void* arg) {
    Py_VISIT(self->buffer);
    Py_VISIT(self->inputBuffer);
    return 0;
}

int CompressFftAdpcm_clear(CompressFftAdpcm* self) {
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

void CompressFftAdpcm_dealloc(CompressFftAdpcm* self) {
    PyObject_GC_UnTrack(self);
    CompressFftAdpcm_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* CompressFftAdpcm_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CompressFftAdpcm* self;
    self = (CompressFftAdpcm*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->buffer = NULL;
        self->inputBuffer = NULL;
        self->read_pos = 0;
        self->run = true;
        self->worker = 0;
    }
    return (PyObject*) self;
}

void* CompressFftAdpcm_worker(void* ctx) {
    CompressFftAdpcm* self = (CompressFftAdpcm*) ctx;

    fft_compress_ima_adpcm_t job;
    fft_compress_ima_adpcm_init(&job, self->fft_size);

    uint32_t read;
    while (self->run) {
        read = Buffer_read_n(self->inputBuffer, fft_compress_ima_adpcm_get_write_pointer(&job), &self->read_pos, self->fft_size);
        if (read == 0) {
            self->run = false;
            break;
        }
        fft_compress_ima_adpcm(&job, Buffer_getWritePointer_n(self->buffer, job.real_data_size / 2));
        Buffer_advance(self->buffer, job.real_data_size / 2);
    }

    fft_compress_ima_adpcm_free(&job);

    Buffer_shutdown(self->buffer);

    PyGILState_STATE gstate = PyGILState_Ensure();
    Py_DECREF(self->inputBuffer);
    self->inputBuffer = NULL;
    PyGILState_Release(gstate);
    return NULL;
}

int CompressFftAdpcm_init(CompressFftAdpcm* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"fft_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->fft_size))
        return -1;

    // we output 8-bit unsigned char samples
    PyObject* bufferArgs = Py_BuildValue("()");
    if (bufferArgs == NULL) return -1;
    PyObject* bufferKwargs = Py_BuildValue("{s:B}", "item_size", sizeof(unsigned char));
    if (bufferKwargs == NULL) return -1;
    self->buffer = (Buffer*) PyObject_Call((PyObject*) &BufferType, bufferArgs, bufferKwargs);
    Py_DECREF(args);
    Py_DECREF(bufferKwargs);

    if (self->buffer == NULL) return -1;
    Py_INCREF(self->buffer);

    return 0;
}

PyObject* CompressFftAdpcm_setInput(CompressFftAdpcm* self, PyObject* args, PyObject* kwds) {
    if (self->inputBuffer != NULL) {
        PyErr_SetString(PyExc_ValueError, "cannot set buffer twice");
        return NULL;
    }

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

    if (pthread_create(&self->worker, NULL, CompressFftAdpcm_worker, self) != 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    pthread_setname_np(self->worker, "pycsdr Compress");

    Py_RETURN_NONE;
}

PyObject* CompressFftAdpcm_getBuffer(CompressFftAdpcm* self, PyObject* Py_UNUSED(ignored)) {
    if (self->buffer == NULL) {
        Py_RETURN_NONE;
    }
    Py_INCREF(self->buffer);
    return (PyObject*) self->buffer;
}

PyObject* CompressFftAdpcm_stop(CompressFftAdpcm* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    Py_RETURN_NONE;
}

PyMethodDef CompressFftAdpcm_methods[] = {
    {"getBuffer", (PyCFunction) CompressFftAdpcm_getBuffer, METH_NOARGS,
     "get the output buffer"
    },
    {"setInput", (PyCFunction) CompressFftAdpcm_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"stop", (PyCFunction) CompressFftAdpcm_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject CompressFftAdpcmType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pycsdr.CompressFftAdpcm",
    .tp_doc = "Custom objects",
    .tp_basicsize = sizeof(CompressFftAdpcm),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = CompressFftAdpcm_new,
    .tp_init = (initproc) CompressFftAdpcm_init,
    .tp_dealloc = (destructor) CompressFftAdpcm_dealloc,
    .tp_traverse = (traverseproc) CompressFftAdpcm_traverse,
    .tp_clear = (inquiry) CompressFftAdpcm_clear,
    .tp_methods = CompressFftAdpcm_methods,
};
