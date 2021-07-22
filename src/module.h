#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <csdr/module.hpp>
#include <csdr/async.hpp>

#include "buffer.h"

struct Module: PyObject {
    Csdr::UntypedModule* module;
    Csdr::UntypedRunner* runner = nullptr;
    Buffer* input = nullptr;
    Buffer* output = nullptr;
};

template <typename T>
PyObject* getFormat();

template <typename U>
PyObject* Module_getOutputFormat(Module* self);

template <typename T, typename U>
PyObject* Module_setInput(Module* self, PyObject* args, PyObject* kwds);

template <typename T, typename U>
PyObject* Module_setOutput(Module* self, PyObject* args, PyObject* kwds);

PyObject* Module_stop(Module* self);

template <typename T>
int Module_clear(Module* self);
