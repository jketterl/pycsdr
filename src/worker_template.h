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
    extern PyType_Spec name##Spec;

void setThreadName(pthread_t t, char* name);

#define MAKE_WORKER(name, input_size, output_size) \
    static void* name##_worker(void* ctx); \
    \
    static PyObject* name##_start(name* self) { \
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
    static PyObject* name##_stop(name* self, PyObject* Py_UNUSED(ignored)) { \
        self->run = false; \
        if (self->worker != 0) { \
            Buffer_unblock(self->inputBuffer); \
            void* retval = NULL; \
            pthread_join(self->worker, retval); \
        } \
        self->worker = 0; \
        Py_RETURN_NONE; \
    } \
    \
    static int name##_clear(name* self) { \
        name##_stop(self, Py_None); \
        Py_XDECREF(self->inputBuffer); \
        self->inputBuffer = NULL; \
        Py_XDECREF(self->outputBuffer); \
        self->outputBuffer = NULL; \
        return 0; \
    } \
    \
    static PyObject* name##_setInput(name* self, PyObject* args, PyObject* kwds) { \
        if (name##_stop(self, Py_None) == NULL) { \
            return NULL; \
        } \
        \
        if (self->inputBuffer != NULL) Py_DECREF(self->inputBuffer); \
        \
        static char* kwlist[] = {"buffer", NULL}; \
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, getApiType("Buffer"), &self->inputBuffer)) { \
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
    static PyObject* name##_setOutput(name* self, PyObject* args, PyObject* kwds) { \
        if (name##_stop(self, Py_None) == NULL) { \
            return NULL; \
        } \
        \
        if (self->outputBuffer != NULL) Py_DECREF(self->outputBuffer); \
        \
        static char* kwlist[] = {"buffer", NULL}; \
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, getApiType("Buffer"), &self->outputBuffer)) { \
            return NULL; \
        } \
        Py_INCREF(self->outputBuffer); \
        \
        Buffer_setItemSize(self->outputBuffer, output_size); \
        \
        return name##_start(self); \
    }

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

#define MAKE_WORKER_SPEC(name) \
    static PyType_Slot name##Slots[] = { \
        {Py_tp_init, name##_init}, \
        {Py_tp_clear, name##_clear}, \
        {Py_tp_methods, name##_methods}, \
        {0, 0} \
    }; \
    \
    PyType_Spec name##Spec = { \
        "pycsdr.modules." #name, \
        sizeof(name), \
        0, \
        Py_TPFLAGS_DEFAULT, \
        name##Slots \
    };
