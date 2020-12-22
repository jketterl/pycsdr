#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <libcsdr.h>
#include <signal.h>

#include "buffer.h"

typedef struct {
    PyObject_HEAD
    uint16_t fft_size;
    Buffer* inputBuffer;
    Buffer* outputBuffer;
    uint32_t read_pos;
    bool run;
    pthread_t worker;
    float add_db;
    uint16_t avg_number;
} LogAveragePower;

int LogAveragePower_traverse(LogAveragePower* self, visitproc visit, void* arg);
int LogAveragePower_clear(LogAveragePower* self);
void LogAveragePower_dealloc(LogAveragePower* self);
PyObject* LogAveragePower_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int LogAveragePower_init(LogAveragePower* self, PyObject* args, PyObject* kwds);
PyObject* LogAveragePower_setInput(LogAveragePower* self, PyObject* args, PyObject* kwds);
PyObject* LogAveragePower_setOutput(LogAveragePower* self, PyObject* args, PyObject* kwds);
PyObject* LogAveragePower_start(LogAveragePower* self);
PyObject* LogAveragePower_stop(LogAveragePower* self, PyObject* Py_UNUSED(ignored));
PyObject* LogAveragePower_setFftAverages(LogAveragePower* self, PyObject* args, PyObject* kwds);

extern PyTypeObject LogAveragePowerType;