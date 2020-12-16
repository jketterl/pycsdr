#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <libcsdr.h>
#include <fftw3.h>

#include "buffer.h"

typedef struct {
    PyObject_HEAD
    uint16_t size;
    uint16_t every_n_samples;
    // TODO window
    Buffer* inputBuffer;
    Buffer* buffer;
    uint32_t read_pos;
    bool run;
    pthread_t worker;
} Fft;

int Fft_traverse(Fft* self, visitproc visit, void* arg);
int Fft_clear(Fft* self);
void Fft_dealloc(Fft* self);
PyObject* Fft_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int Fft_init(Fft* self, PyObject* args, PyObject* kwds);
PyObject* Fft_getBuffer(Fft* self, PyObject* Py_UNUSED(ignored));
PyObject* Fft_setInput(Fft* self, PyObject* args, PyObject* kwds);
PyObject* Fft_stop(Fft* self, PyObject* Py_UNUSED(ignored));

extern PyTypeObject FftType;