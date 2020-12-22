#include "compress_fft_adpcm_f_u8.h"

int CompressFftAdpcm_traverse(CompressFftAdpcm* self, visitproc visit, void* arg) {
    Py_VISIT(self->outputBuffer);
    Py_VISIT(self->inputBuffer);
    return 0;
}

int CompressFftAdpcm_clear(CompressFftAdpcm* self) {
    CompressFftAdpcm_stop(self, Py_None);
    if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer);
    self->inputBuffer = NULL;
    if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer);
    self->outputBuffer = NULL;
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
        self->outputBuffer = NULL;
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
        read = Buffer_read_n(self->inputBuffer, fft_compress_ima_adpcm_get_write_pointer(&job), &self->read_pos, &self->run, self->fft_size);
        if (read == 0) {
            self->run = false;
            break;
        }
        fft_compress_ima_adpcm(&job, Buffer_getWritePointer_n(self->outputBuffer, job.real_data_size / 2));
        Buffer_advance(self->outputBuffer, job.real_data_size / 2);
    }

    fft_compress_ima_adpcm_free(&job);

    //Buffer_shutdown(self->outputBuffer);

    return NULL;
}

int CompressFftAdpcm_init(CompressFftAdpcm* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"fft_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->fft_size))
        return -1;

    return 0;
}

PyObject* CompressFftAdpcm_setInput(CompressFftAdpcm* self, PyObject* args, PyObject* kwds) {
    if (CompressFftAdpcm_stop(self, Py_None) == NULL) {
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

    return CompressFftAdpcm_start(self);
}

PyObject* CompressFftAdpcm_setOutput(CompressFftAdpcm* self, PyObject* args, PyObject* kwds) {
    if (CompressFftAdpcm_stop(self, Py_None) == NULL) {
        return NULL;
    }

    static char* kwlist[] = {"output", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->outputBuffer))
        return NULL;

    Py_INCREF(self->outputBuffer);

    Buffer_setItemSize(self->outputBuffer, sizeof(unsigned char));

    return CompressFftAdpcm_start(self);
}

PyObject* CompressFftAdpcm_start(CompressFftAdpcm* self) {
    if (self->outputBuffer == NULL || self->inputBuffer == NULL) {
        Py_RETURN_NONE;
    } else {
        self->run = true;

        if (pthread_create(&self->worker, NULL, CompressFftAdpcm_worker, self) != 0) {
            PyErr_SetFromErrno(PyExc_OSError);
            return NULL;
        }

        pthread_setname_np(self->worker, "pycsdr Compress");

        Py_RETURN_NONE;
    }
}

PyObject* CompressFftAdpcm_stop(CompressFftAdpcm* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    if (self->worker != 0) {
        Buffer_unblock(self->inputBuffer);
        void* retval = NULL;
        pthread_join(self->worker, retval);
    }
    self->worker = 0;
    Py_RETURN_NONE;
}

PyMethodDef CompressFftAdpcm_methods[] = {
    {"setInput", (PyCFunction) CompressFftAdpcm_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) CompressFftAdpcm_setOutput, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
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
