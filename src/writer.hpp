#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/writer.hpp>

struct Writer {
    PyObject_HEAD
    Csdr::UntypedWriter* writer;
    PyObject* writerFormat;
};

extern PyType_Spec WriterSpec;