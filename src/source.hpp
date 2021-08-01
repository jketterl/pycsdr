#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/source.hpp>

#include "writer.hpp"

struct Source {
    PyObject_HEAD
    PyObject* outputFormat;
    Writer* writer;
    Csdr::UntypedSource* source;
};

PyObject* Source_setWriter(Source* self, PyObject* args, PyObject* kwds);

extern PyType_Spec SourceSpec;