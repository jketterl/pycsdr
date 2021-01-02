#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <libcsdr.h>

#include "buffer.h"
#include "worker_template.h"

typedef struct {
    PyObject_VAR_HEAD;
    WORKER_MEMBERS
    float add_db;
} LogPower;

MAKE_WORKER_H(LogPower)

PyObject* LogPower_setFftAverages(LogPower* self, PyObject* args, PyObject* kwds);

extern PyTypeObject LogPowerType;