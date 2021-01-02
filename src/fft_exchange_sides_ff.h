#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <libcsdr.h>

#include "buffer.h"
#include "worker_template.h"

typedef struct {
    PyObject_VAR_HEAD;
    WORKER_MEMBERS
    uint16_t fft_size;
} FftExchangeSides;

MAKE_WORKER_H(FftExchangeSides);

extern PyTypeObject FftExchangeSidesType;