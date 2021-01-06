#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <libcsdr.h>

#include "worker_template.h"

typedef struct {
    PyObject base1;
    PyObject base2;
    WORKER_MEMBERS
    int16_t decimation;
    float transition_bw;
    window_t window;
} FirDecimate;

MAKE_WORKER_H(FirDecimate)
