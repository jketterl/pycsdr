#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/ringbuffer.hpp>
#include <csdr/complex.hpp>

typedef struct {
    PyObject base1;
    PyObject base2;
    Csdr::Ringbuffer<Csdr::complex<float>>* buffer;
    Csdr::RingbufferReader<Csdr::complex<float>>* reader;
} Buffer;

// 256kb
#define DEFAULT_BUFFER_SIZE 262144

extern PyType_Spec BufferSpec;