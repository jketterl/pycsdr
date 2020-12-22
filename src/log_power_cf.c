#include "log_power_cf.h"

int LogPower_traverse(LogPower* self, visitproc visit, void* arg) {
    Py_VISIT(self->outputBuffer);
    Py_VISIT(self->inputBuffer);
    return 0;
}

int LogPower_clear(LogPower* self) {
    LogPower_stop(self, Py_None);
    if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer);
    self->inputBuffer = NULL;
    if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer);
    self->outputBuffer = NULL;
    return 0;
}

void LogPower_dealloc(LogPower* self) {
    PyObject_GC_UnTrack(self);
    LogPower_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* LogPower_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    LogPower* self;
    self = (LogPower*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->outputBuffer = NULL;
        self->inputBuffer = NULL;
        self->read_pos = 0;
        self->run = true;
        self->worker = 0;
        self->add_db = 0;
    }
    return (PyObject*) self;
}

void* LogPower_worker(void* ctx) {
    LogPower* self = (LogPower*) ctx;

    uint32_t available;
    while (self->run) {
        available = Buffer_wait(self->inputBuffer, self->read_pos, &self->run);
        if (available == 0) {
            self->run = false;
            break;
        }
        logpower_cf(
            (complexf*) Buffer_getReadPointer(self->inputBuffer, self->read_pos),
            Buffer_getWritePointer_n(self->outputBuffer, available),
            available,
            self->add_db
        );
        Buffer_advanceReadPos(self->inputBuffer, &self->read_pos, available);
        Buffer_advance(self->outputBuffer, available);
    }

    //Buffer_shutdown(self->outputBuffer);

    return NULL;
}

int LogPower_init(LogPower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"add_db", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist,
                                     &self->add_db))
        return -1;

    return 0;
}

PyObject* LogPower_setInput(LogPower* self, PyObject* args, PyObject* kwds) {
    if (LogPower_stop(self, Py_None) == NULL) {
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

    return LogPower_start(self);
}

PyObject* LogPower_setOutput(LogPower* self, PyObject* args, PyObject* kwds) {
    if (LogPower_stop(self, Py_None) == NULL) {
        return NULL;
    }

    static char* kwlist[] = {"output", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist,
                                     &BufferType, &self->outputBuffer))
        return NULL;

    Py_INCREF(self->outputBuffer);

    Buffer_setItemSize(self->outputBuffer, sizeof(float));

    return LogPower_start(self);
}

PyObject* LogPower_start(LogPower* self) {
    if (self->outputBuffer == NULL || self->inputBuffer == NULL) {
        Py_RETURN_NONE;
    } else {
        self->run = true;

        if (pthread_create(&self->worker, NULL, LogPower_worker, self) != 0) {
            PyErr_SetFromErrno(PyExc_OSError);
            return NULL;
        }

        pthread_setname_np(self->worker, "pycsdr LogPower");

        Py_RETURN_NONE;
    }
}

PyObject* LogPower_stop(LogPower* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    if (self->worker != 0) {
        Buffer_unblock(self->inputBuffer);
        void* retval = NULL;
        pthread_join(self->worker, retval);
    }
    self->worker = 0;
    Py_RETURN_NONE;
}

PyMethodDef LogPower_methods[] = {
    {"setInput", (PyCFunction) LogPower_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
    },
    {"setOutput", (PyCFunction) LogPower_setOutput, METH_VARARGS | METH_KEYWORDS,
     "set the output buffer"
    },
    {"stop", (PyCFunction) LogPower_stop, METH_NOARGS,
     "stop processing"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject LogPowerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pycsdr.LogPower",
    .tp_doc = "Custom objects",
    .tp_basicsize = sizeof(LogPower),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = LogPower_new,
    .tp_init = (initproc) LogPower_init,
    .tp_dealloc = (destructor) LogPower_dealloc,
    .tp_traverse = (traverseproc) LogPower_traverse,
    .tp_clear = (inquiry) LogPower_clear,
    .tp_methods = LogPower_methods,
};
