#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <libcsdr.h>

#include "worker_template.h"

typedef struct {
    PyObject base1;
    PyObject base2;
    WORKER_MEMBERS
    float low_cut;
    float high_cut;
} BandpassFirFft;

MAKE_WORKER_H(BandpassFirFft)
