#include "log_average_power_cf.h"

int LogAveragePower_traverse(LogAveragePower* self, visitproc visit, void* arg) {
    Py_VISIT(self->outputBuffer);
    Py_VISIT(self->inputBuffer);
    return 0;
}

int LogAveragePower_clear(LogAveragePower* self) {
    LogAveragePower_stop(self, Py_None);
    if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer);
    self->inputBuffer = NULL;
    if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer);
    self->outputBuffer = NULL;
    return 0;
}

void LogAveragePower_dealloc(LogAveragePower* self) {
    PyObject_GC_UnTrack(self);
    LogAveragePower_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* LogAveragePower_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    LogAveragePower* self;
    self = (LogAveragePower*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->outputBuffer = NULL;
        self->inputBuffer = NULL;
        self->read_pos = 0;
        self->fft_size = 0;
        self->run = true;
        self->worker = 0;
        self->add_db = 0;
        self->avg_number = 0;
    }
    return (PyObject*) self;
}

void* LogAveragePower_worker(void* ctx) {
    LogAveragePower* self = (LogAveragePower*) ctx;

    float add_db = self->add_db - 10.0 * log10(self->avg_number);
    uint32_t available;
    while (self->run) {
        // get an area to write to from the output buffer
        float* output = Buffer_getWritePointer_n(self->outputBuffer, self->fft_size);
        // zero it
        memset(output, 0, sizeof(float) * self->fft_size);
        for (int i = 0; i < self->avg_number; i++) {
            // wait for input samples
            available = Buffer_wait_n(self->inputBuffer, self->read_pos, &self->run, self->fft_size);
            if (available == 0) {
                self->run = false;
                break;
            }
            accumulate_power_cf((complexf*) Buffer_getReadPointer(self->inputBuffer, self->read_pos), output, self->fft_size);
            Buffer_advanceReadPos(self->inputBuffer, &self->read_pos, self->fft_size);
        }
        if (available == 0) {
            self->run = false;
            break;
        }
        log_ff(output, output, self->fft_size, add_db);
        // inform output buffer that data is ready
        Buffer_advance(self->outputBuffer, self->fft_size);
    }

    //Buffer_shutdown(self->outputBuffer);

    return NULL;
}

int LogAveragePower_init(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"add_db", "fft_size", "avg_number", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "fHH", kwlist,
                                     &self->add_db, &self->fft_size, &self->avg_number))
        return -1;

    return 0;
}

PyObject* LogAveragePower_setInput(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    if (LogAveragePower_stop(self, Py_None) == NULL) {
        return NULL;
    }

    if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer);

    static char* kwlist[] = {"buffer", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->inputBuffer))
        return NULL;

    if (Buffer_getItemSize(self->inputBuffer) != sizeof(complexf)) {
        self->inputBuffer = NULL;
        PyErr_SetString(PyExc_ValueError, "Input buffer item size mismatch");
        return NULL;
    }

    Py_INCREF(self->inputBuffer);

    return LogAveragePower_start(self);
}

PyObject* LogAveragePower_setOutput(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    if (LogAveragePower_stop(self, Py_None) == NULL) {
        return NULL;
    }

    if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer);

    static char* kwlist[] = {"buffer", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->outputBuffer))
        return NULL;

    Py_INCREF(self->outputBuffer);

    Buffer_setItemSize(self->outputBuffer, sizeof(float));

    return LogAveragePower_start(self);
}

PyObject* LogAveragePower_start(LogAveragePower* self) {
    if (self->outputBuffer == NULL || self->inputBuffer == NULL) {
        Py_RETURN_NONE;
    } else {
        self->run = true;

        if (pthread_create(&self->worker, NULL, LogAveragePower_worker, self) != 0) {
            PyErr_SetFromErrno(PyExc_OSError);
            return NULL;
        }

        pthread_setname_np(self->worker, "pycsdr LogAvera");

        Py_RETURN_NONE;
    }
}

PyObject* LogAveragePower_stop(LogAveragePower* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    if (self->worker != 0) {
        Buffer_unblock(self->inputBuffer);
        void* retval = NULL;
        pthread_join(self->worker, retval);
    }
    self->worker = 0;
    Py_RETURN_NONE;
}

PyObject* LogAveragePower_setFftAverages(LogAveragePower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"avg_number", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "H", kwlist,
                                     &self->avg_number))
        return NULL;

    Py_RETURN_NONE;
}

PyMethodDef LogAveragePower_methods[] = {
    {"setInput", (PyCFunction) LogAveragePower_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) LogAveragePower_setOutput, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"stop", (PyCFunction) LogAveragePower_stop, METH_NOARGS,
     "stop processing"
    },
    {"setFftAverages", (PyCFunction) LogAveragePower_setFftAverages, METH_VARARGS | METH_KEYWORDS,
     "set fft averageing factor"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject LogAveragePowerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pycsdr.LogAveragePower",
    .tp_doc = "Custom objects",
    .tp_basicsize = sizeof(LogAveragePower),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = LogAveragePower_new,
    .tp_init = (initproc) LogAveragePower_init,
    .tp_dealloc = (destructor) LogAveragePower_dealloc,
    .tp_traverse = (traverseproc) LogAveragePower_traverse,
    .tp_clear = (inquiry) LogAveragePower_clear,
    .tp_methods = LogAveragePower_methods,
};
