#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <libcsdr.h>

#include "buffer.h"
#include "worker_template.h"

typedef struct {
    PyObject base1;
    PyObject base2;
    WORKER_MEMBERS
    uint16_t fft_size;
    float add_db;
    uint16_t avg_number;
} LogAveragePower;

MAKE_WORKER_H(LogAveragePower)
