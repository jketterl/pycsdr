#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/source.hpp>
#include <csdr/complex.hpp>

typedef struct {
    PyObject base1;
    PyObject base2;
    Csdr::TcpSource<Csdr::complex<float>>* source;
} TcpSource;

extern PyType_Spec TcpSourceSpec;