#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <libcsdr.h>

#include "buffer.h"
#include "worker_template.h"

//We will pad the FFT at the beginning, with the first value of the input data, COMPRESS_FFT_PAD_N times.
//No, this is not advanced DSP, just the ADPCM codec produces some gabarge samples at the beginning,
//so we just add data to become garbage and get skipped.
//COMPRESS_FFT_PAD_N should be even.
#define COMPRESS_FFT_PAD_N 10

typedef struct {
    PyObject_HEAD
    WORKER_MEMBERS
    uint16_t fft_size;
} CompressFftAdpcm;

MAKE_WORKER_H(CompressFftAdpcm);

extern PyTypeObject CompressFftAdpcmType;