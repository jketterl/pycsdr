#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <libcsdr.h>
#include <signal.h>

#include "buffer.h"

//We will pad the FFT at the beginning, with the first value of the input data, COMPRESS_FFT_PAD_N times.
//No, this is not advanced DSP, just the ADPCM codec produces some gabarge samples at the beginning,
//so we just add data to become garbage and get skipped.
//COMPRESS_FFT_PAD_N should be even.
#define COMPRESS_FFT_PAD_N 10

typedef struct {
    PyObject_HEAD
    Buffer* inputBuffer;
    Buffer* outputBuffer;
    uint32_t read_pos;
    bool run;
    pthread_t worker;
    uint16_t fft_size;
} CompressFftAdpcm;

int CompressFftAdpcm_traverse(CompressFftAdpcm* self, visitproc visit, void* arg);
int CompressFftAdpcm_clear(CompressFftAdpcm* self);
void CompressFftAdpcm_dealloc(CompressFftAdpcm* self);
PyObject* CompressFftAdpcm_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
int CompressFftAdpcm_init(CompressFftAdpcm* self, PyObject* args, PyObject* kwds);
PyObject* CompressFftAdpcm_setInput(CompressFftAdpcm* self, PyObject* args, PyObject* kwds);
PyObject* CompressFftAdpcm_setOutput(CompressFftAdpcm* self, PyObject* args, PyObject* kwds);
PyObject* CompressFftAdpcm_start(CompressFftAdpcm* self);
PyObject* CompressFftAdpcm_stop(CompressFftAdpcm* self, PyObject* Py_UNUSED(ignored));

extern PyTypeObject CompressFftAdpcmType;