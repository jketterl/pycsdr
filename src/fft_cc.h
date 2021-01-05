#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <libcsdr.h>
#include <fftw3.h>

#include "worker_template.h"

typedef struct {
    PyObject base1;
    PyObject base2;
    WORKER_MEMBERS
    uint16_t size;
    uint16_t every_n_samples;
    // TODO window
} Fft;

MAKE_WORKER_H(Fft)
