#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <libcsdr.h>

#include "worker_template.h"

typedef struct {
    PyObject_HEAD
    WORKER_MEMBERS
    int16_t decimation;
} FirDecimate;

MAKE_WORKER_H(FirDecimate)

PyObject* FirDecimate_setDecimation(FirDecimate* self, PyObject* args, PyObject* kwds);

extern PyTypeObject FirDecimateType;