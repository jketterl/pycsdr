#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "buffer.h"

#define WORKER_MEMBERS \
    Buffer* inputBuffer; \
    Buffer* outputBuffer; \
    uint32_t read_pos; \
    bool run; \
    pthread_t worker;

#define MAKE_WORKER_H(name) \
    int name##_traverse(name* self, visitproc visit, void* arg); \
    int name##_clear(name* self); \
    void name##_dealloc(name* self); \
    PyObject* name##_new(PyTypeObject* type, PyObject* args, PyObject* kwds); \
    int name##_init(name* self, PyObject* args, PyObject* kwds); \
    PyObject* name##_setInput(name* self, PyObject* args, PyObject* kwds); \
    PyObject* name##_setOutput(name* self, PyObject* args, PyObject* kwds); \
    PyObject* name##_start(name* self); \
    PyObject* name##_stop(name* self, PyObject* Py_UNUSED(ignored));

void setThreadName(pthread_t t, char* name);

#define MAKE_WORKER(name, input_size, output_size) \
    void* name##_worker(void* ctx); \
    \
    int name##_traverse(name* self, visitproc visit, void* arg) { \
        Py_VISIT(Py_TYPE(self)); \
        Py_VISIT(self->outputBuffer); \
        Py_VISIT(self->inputBuffer); \
        return 0; \
    } \
    \
    int name##_clear(name* self) { \
        name##_stop(self, Py_None); \
        if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer); \
        self->inputBuffer = NULL; \
        if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer); \
        self->outputBuffer = NULL; \
        return 0; \
    } \
    \
    void name##_dealloc(name* self) { \
        PyObject_GC_UnTrack(self); \
        name##_clear(self); \
        Py_TYPE(self)->tp_free((PyObject*) self); \
    } \
    \
    PyObject* name##_setInput(name* self, PyObject* args, PyObject* kwds) { \
        if (name##_stop(self, Py_None) == NULL) { \
            return NULL; \
        } \
        \
        if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer); \
        \
        static char* kwlist[] = {"buffer", NULL}; \
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, &BufferType, &self->inputBuffer)) { \
            return NULL; \
        } \
        \
        if (Buffer_getItemSize(self->inputBuffer) != input_size) { \
            self->inputBuffer = NULL; \
            PyErr_SetString(PyExc_ValueError, "Input buffer item size mismatch"); \
            return NULL; \
        } \
        \
        Py_INCREF(self->inputBuffer); \
        \
        return name##_start(self); \
    } \
    \
    PyObject* name##_setOutput(name* self, PyObject* args, PyObject* kwds) { \
        if (name##_stop(self, Py_None) == NULL) { \
            return NULL; \
        } \
        \
        if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer); \
        \
        static char* kwlist[] = {"buffer", NULL}; \
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, &BufferType, &self->outputBuffer)) { \
            return NULL; \
        } \
        Py_INCREF(self->outputBuffer); \
        \
        Buffer_setItemSize(self->outputBuffer, output_size); \
        \
        return name##_start(self); \
    } \
    \
    PyObject* name##_start(name* self) { \
        if (self->outputBuffer == NULL || self->inputBuffer == NULL) { \
            Py_RETURN_NONE; \
        } else { \
            self->run = true; \
            \
            if (pthread_create(&self->worker, NULL, name##_worker, self) != 0) { \
                PyErr_SetFromErrno(PyExc_OSError); \
                return NULL; \
            } \
            \
            setThreadName(self->worker, "pycsdr " #name); \
            \
            Py_RETURN_NONE; \
        } \
    } \
    \
    PyObject* name##_stop(name* self, PyObject* Py_UNUSED(ignored)) { \
        self->run = false; \
        if (self->worker != 0) { \
            Buffer_unblock(self->inputBuffer); \
            void* retval = NULL; \
            pthread_join(self->worker, retval); \
        } \
        self->worker = 0; \
        Py_RETURN_NONE; \
    }

#define WORKER_MEMBER_INIT \
    self->outputBuffer = NULL; \
    self->inputBuffer = NULL; \
    self->read_pos = 0; \
    self->run = true; \
    self->worker = 0;

#define WORKER_METHODS(name) \
    {"setInput", (PyCFunction) name##_setInput, METH_VARARGS | METH_KEYWORDS, \
     "set the input buffer" \
    }, \
    {"setOutput", (PyCFunction) name##_setOutput, METH_VARARGS | METH_KEYWORDS, \
     "set the output buffer" \
    }, \
    {"stop", (PyCFunction) name##_stop, METH_NOARGS, \
     "stop processing" \
    },

#define MAKE_WORKER_TYPE(name) \
    PyTypeObject name##Type = { \
        PyVarObject_HEAD_INIT(NULL, 0) \
        .tp_name = "pycsdr.modules." #name, \
        .tp_doc = "Custom objects", \
        .tp_basicsize = sizeof(name), \
        .tp_base = NULL, \
        .tp_itemsize = 0, \
        .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HEAPTYPE | Py_TPFLAGS_HAVE_GC, \
        .tp_new = name##_new, \
        .tp_init = (initproc) name##_init, \
        .tp_dealloc = (destructor) name##_dealloc, \
        .tp_traverse = (traverseproc) name##_traverse, \
        .tp_clear = (inquiry) name##_clear, \
        .tp_methods = name##_methods, \
    };
