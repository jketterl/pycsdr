#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <libcsdr.h>

#include "worker_template.h"

typedef struct {
    PyObject_HEAD
    WORKER_MEMBERS
    float low_cut;
    float high_cut;
} BandpassFirFft;

MAKE_WORKER_H(BandpassFirFft)

PyObject* BandpassFirFft_setLowCut(BandpassFirFft* self, PyObject* args, PyObject* kwds);
PyObject* BandpassFirFft_setHighCut(BandpassFirFft* self, PyObject* args, PyObject* kwds);

extern PyTypeObject BandpassFirFftType;