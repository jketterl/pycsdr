#include "log_power_cf.h"

int LogPower_traverse(LogPower* self, visitproc visit, void* arg) {
    Py_VISIT(self->buffer);
    Py_VISIT(self->inputBuffer);
    return 0;
}

int LogPower_clear(LogPower* self) {
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

void LogPower_dealloc(LogPower* self) {
    PyObject_GC_UnTrack(self);
    LogPower_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* LogPower_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    LogPower* self;
    self = (LogPower*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->buffer = NULL;
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
        available = Buffer_wait(self->inputBuffer, self->read_pos);
        if (available == 0) {
            self->run = false;
            break;
        }
        logpower_cf(
            (complexf*) Buffer_getReadPointer(self->inputBuffer, self->read_pos),
            Buffer_getWritePointer_n(self->buffer, available),
            available,
            self->add_db
        );
        Buffer_advanceReadPos(self->inputBuffer, &self->read_pos, available);
        Buffer_advance(self->buffer, available);
    }

    Buffer_shutdown(self->buffer);

    PyGILState_STATE gstate = PyGILState_Ensure();
    Py_DECREF(self->inputBuffer);
    self->inputBuffer = NULL;
    PyGILState_Release(gstate);
    return NULL;
}

int LogPower_init(LogPower* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {"add_db", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "f", kwlist,
                                     &self->add_db))
        return -1;

    // we output 32-bit float real samples
    PyObject* bufferArgs = Py_BuildValue("()");
    if (bufferArgs == NULL) return -1;
    PyObject* bufferKwargs = Py_BuildValue("{s:B}", "item_size", sizeof(float));
    if (bufferKwargs == NULL) return -1;
    self->buffer = (Buffer*) PyObject_Call((PyObject*) &BufferType, bufferArgs, bufferKwargs);
    Py_DECREF(args);
    Py_DECREF(bufferKwargs);

    if (self->buffer == NULL) return -1;
    Py_INCREF(self->buffer);

    return 0;
}

PyObject* LogPower_setInput(LogPower* self, PyObject* args, PyObject* kwds) {
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

    if (pthread_create(&self->worker, NULL, LogPower_worker, self) != 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    pthread_setname_np(self->worker, "pycsdr LogPower");

    Py_RETURN_NONE;
}

PyObject* LogPower_getBuffer(LogPower* self, PyObject* Py_UNUSED(ignored)) {
    if (self->buffer == NULL) {
        Py_RETURN_NONE;
    }
    Py_INCREF(self->buffer);
    return (PyObject*) self->buffer;
}

PyObject* LogPower_stop(LogPower* self, PyObject* Py_UNUSED(ignored)) {
    self->run = false;
    Py_RETURN_NONE;
}

PyMethodDef LogPower_methods[] = {
    {"getBuffer", (PyCFunction) LogPower_getBuffer, METH_NOARGS,
     "get the output buffer"
    },
    {"setInput", (PyCFunction) LogPower_setInput, METH_VARARGS | METH_KEYWORDS,
     "set the input buffer"
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
