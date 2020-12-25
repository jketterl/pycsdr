#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <libcsdr.h>

#include "buffer.h"
#include "worker_template.h"

typedef struct {
    PyObject_HEAD
    WORKER_MEMBERS
    uint16_t fft_size;
} CompressFftAdpcm;

MAKE_WORKER_H(CompressFftAdpcm);

extern PyTypeObject CompressFftAdpcmType;