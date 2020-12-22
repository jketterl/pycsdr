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
} FftExchangeSides;

int FftExchangeSides_traverse(FftExchangeSides* self, visitproc visit, void* arg);
int FftExchangeSides_clear(FftExchangeSides* self);
void FftExchangeSides_dealloc(FftExchangeSides* self);
PyObject* FftExchangeSides_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int FftExchangeSides_init(FftExchangeSides* self, PyObject* args, PyObject* kwds);
PyObject* FftExchangeSides_setInput(FftExchangeSides* self, PyObject* args, PyObject* kwds);
PyObject* FftExchangeSides_setOutput(FftExchangeSides* self, PyObject* args, PyObject* kwds);
PyObject* FftExchangeSides_start(FftExchangeSides* self);
PyObject* FftExchangeSides_stop(FftExchangeSides* self, PyObject* Py_UNUSED(ignored));

extern PyTypeObject FftExchangeSidesType;