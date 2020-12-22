#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <libcsdr.h>

#include "buffer.h"

typedef struct {
    PyObject_HEAD
    Buffer* inputBuffer;
    Buffer* buffer;
    uint32_t read_pos;
    bool run;
    pthread_t worker;
    float add_db;
} LogPower;

int LogPower_traverse(LogPower* self, visitproc visit, void* arg);
int LogPower_clear(LogPower* self);
void LogPower_dealloc(LogPower* self);
PyObject* LogPower_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int LogPower_init(LogPower* self, PyObject* args, PyObject* kwds);
PyObject* LogPower_getBuffer(LogPower* self, PyObject* Py_UNUSED(ignored));
PyObject* LogPower_setInput(LogPower* self, PyObject* args, PyObject* kwds);
PyObject* LogPower_stop(LogPower* self, PyObject* Py_UNUSED(ignored));
PyObject* LogPower_setFftAverages(LogPower* self, PyObject* args, PyObject* kwds);

extern PyTypeObject LogPowerType;